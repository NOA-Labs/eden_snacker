/*!
 * @file Adafruit_OM9663.cpp porting from Adafruit MFRC630 Library
 *
 * @mainpage Adafruit MFRC630 Library
 *
 * @section intro_sec Introduction
 *
 * This is a library for the Adafruit MFRC630 Breakout
 *
 * Designed specifically to work with the Adafruit MFRC630 Breakout:
 * http://www.adafruit.com/products/xxx
 *
 * These boards use I2C to communicate, 2 pins are required to interface.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit andopen-source hardware by purchasing products
 * from Adafruit!
 *
 * @section author Author
 *
 * Written by Kevin Townsend for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text above must be included in any redistribution
 */
#include "..\..\LIB\PUB\NOA_public.h"

#include "Adafruit_OM9663.h"

/* Debug output level */
/*
 * NOTE: Setting this macro above RELEASE may require more SRAM than small
 *       MCUs like the Atmel 32u4 can provide!
 */

#define OM9663_VERBOSITY_RELEASE (0) //!< No debug output
#define OM9663_VERBOSITY_DEBUG (1)   //!< Debug message output
#define OM9663_VERBOSITY  (OM9663_VERBOSITY_RELEASE) //!< Sets verbosity variable
// #define OM9663_VERBOSITY  (OM9663_VERBOSITY_DEBUG) //!< Sets verbosity variable

#if MFRC630_VERBOSITY >= OM9663_VERBOSITY_DEBUG
#define CPRINTF(format, args...)  DBGLOG(Info, format, ## args)
#define CPRINTS(format, args...)  DBGLOG(Debug, format, ## args)
#else
#define CPRINTF(format, args...)
#define CPRINTS(format, args...)
#endif

/* ANTENNA CONFIGURATION SETTINGS (registers 0x28..0x39) */
/* -------------------------------------------------------- */
/* ISO/IEC14443-A 106 */
uint8_t antcfg_iso14443a_106[18] = {0x8E, 0x12, 0x39, 0x0A, 0x18, 0x18,
                                    0x0F, 0x21, 0x00, 0xC0, 0x12, 0xCF,
                                    0x00, 0x04, 0x90, 0x5C, 0x12, 0x0A};
/* ISO/IEC14443-A 212 */
uint8_t antcfg_iso14443a_212[18] = {0x8E, 0xD2, 0x11, 0x0A, 0x18, 0x18,
                                    0x0F, 0x10, 0x00, 0xC0, 0x12, 0xCF,
                                    0x00, 0x05, 0x90, 0x3C, 0x12, 0x0B};
/* ISO/IEC14443-A 424 */
uint8_t antcfg_iso14443a_424[18] = {0x8F, 0xDE, 0x11, 0x0F, 0x18, 0x18,
                                    0x0F, 0x07, 0x00, 0xC0, 0x12, 0xCF,
                                    0x00, 0x06, 0x90, 0x2B, 0x12, 0x0B};
/* ISO/IEC14443-A 848 */
uint8_t antcfg_iso14443a_848[18] = {0x8F, 0xDB, 0x21, 0x0F, 0x18, 0x18,
                                    0x0F, 0x02, 0x00, 0xC0, 0x12, 0xCF,
                                    0x00, 0x07, 0x90, 0x3A, 0x12, 0x0B};

/* PROTOCOL CONFIGURATION SETTINGS */
/* -------------------------------------------------------- */
/* ISO/IEC14443-A 106/ MIFARE */
uint8_t protcfg_iso14443a_106[24] = {
    0x20, 0x00, 0x04, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x50, 0x02, 0x00, 0x00, 0x01, 0x00, 0x08, 0x80, 0xB2};
/* ISO/IEC14443-A 212/ MIFARE */
uint8_t protcfg_iso14443a_212[24] = {
    0x20, 0x00, 0x05, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x50, 0x22, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x80, 0xB2};
/* ISO/IEC14443-A 424/ MIFARE */
uint8_t protcfg_iso14443a_424[24] = {
    0x20, 0x00, 0x06, 0x50, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x50, 0x22, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x80, 0xB2};
/* ISO/IEC14443-A 848/ MIFARE */
uint8_t protcfg_iso14443a_848[24] = {
    0x20, 0x00, 0x07, 0x50, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x50, 0x22, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x80, 0xB2};

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/

static unsigned char rev8_lookup[16] = {0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
                                        0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf};

/*!
 * @brief Uses the lookup table above to reverse a single byte.
 * @param n Input byte
 * @return uint8_t. A byte
 */
uint8_t reverse8(uint8_t n) {
  return (rev8_lookup[n & 0b1111] << 4) | rev8_lookup[n >> 4];
}

/**************************************************************************/
/*!
    @brief  Write a byte to the specified register
*/
/**************************************************************************/
void Adafruit_OM9663::write8(byte reg, byte value) {
  CPRINTF("Writing 0x%02X value to 0x%02X", value, reg);

  switch (_transport) {
  case OM9663_TRANSPORT_I2C:
    /* I2C */
//    _wire->beginTransmission(_i2c_addr);
//    _wire->write(reg);
//    _wire->write(value);
//    _wire->endTransmission();
/*    if (_wire_bus == 0) {
      Wire.beginTransmission(_i2c_addr);
      Wire.write(reg);
      Wire.write(value);
      Wire.endTransmission();
    } else {
      Wire1.beginTransmission(_i2c_addr);
      Wire1.write(reg);
      Wire1.write(value);
      Wire1.endTransmission();
    } */
    if (_wire_bus == 0) {
      Wire.beginTransmission(_i2c_addr);
      Wire.write(reg);
      Wire.write(value);
      Wire.endTransmission(true);
    } else {
      Wire1.beginTransmission(_i2c_addr);
      Wire1.write(reg);
      Wire1.write(value);
      Wire1.endTransmission(true);
    }
    break;
  case OM9663_TRANSPORT_SPI:
    /* SPI */
/*    digitalWrite(_cs, LOW);
    SPI.transfer((reg << 1) | 0x00);
    SPI.transfer(value);
    digitalWrite(_cs, HIGH); */
    break;
  case OM9663_TRANSPORT_SERIAL:
    /* TODO: Adjust for 10-bit protocol! */
//    _serial->write((reg << 1) | 0x00);
//    _serial->write(value);
    break;
  }
}

/**************************************************************************/
/*!
    @brief  Write a buffer to the specified register
*/
/**************************************************************************/
void Adafruit_OM9663::writeBuffer(byte reg, uint16_t len, uint8_t *buffer) {
  CPRINTF("Writing %d byte(s) to 0x%02X", len, reg);

  switch (_transport) {
  case OM9663_TRANSPORT_I2C:
    /* I2C */
//    _wire->beginTransmission(_i2c_addr);
//    _wire->write(reg);
//    for (uint16_t i = 0; i < len; i++) {
//      _wire->write(buffer[i]);
//      TRACE_PRINT(F("0x"));
//      TRACE_PRINT(buffer[i], HEX);
//      TRACE_PRINT(F(" "));
//    }
//    _wire->endTransmission();
/*    if (_wire_bus == 0) {
      Wire.beginTransmission(_i2c_addr);
      Wire.write(reg);
      for (uint16_t i = 0; i < len; i++) {
        Wire.write(buffer[i]);
      }
      Wire.endTransmission();
    } else {
      Wire1.beginTransmission(_i2c_addr);
      Wire1.write(reg);
      for (uint16_t i = 0; i < len; i++) {
        Wire1.write(buffer[i]);
      }
      Wire1.endTransmission();
    } */
    if (_wire_bus == 0) {
      Wire.beginTransmission(_i2c_addr);
      Wire.write(reg);
      for (uint16_t i = 0; i < len; i++) {
        Wire.write(buffer[i]);
      }
      Wire.endTransmission(true);
    } else {
      Wire1.beginTransmission(_i2c_addr);
      Wire1.write(reg);
      for (uint16_t i = 0; i < len; i++) {
        Wire1.write(buffer[i]);
      }
      Wire1.endTransmission(true);
    }
//    NOA_PUB_Print_Buf_Hex(buffer, len);
    break;
  case OM9663_TRANSPORT_SPI:
    /* SPI */
/*    digitalWrite(_cs, LOW);
    SPI.transfer((reg << 1) | 0x00);
    for (uint8_t i = 0; i < len; i++) {
      SPI.transfer(buffer[i]);
      TRACE_PRINT(F("0x"));
      TRACE_PRINT(buffer[i], HEX);
      TRACE_PRINT(F(" "));
    }
    digitalWrite(_cs, HIGH); */
    break;
  case OM9663_TRANSPORT_SERIAL:
/*    _serial->write((reg << 1) | 0x00);
    for (uint16_t i = 0; i < len; i++) {
      _serial->write(buffer[i]);
      TRACE_PRINT(F("0x"));
      TRACE_PRINT(buffer[i], HEX);
      TRACE_PRINT(F(" "));
    } */
    break;
  }
//  TRACE_PRINTLN("");
}

/**************************************************************************/
/*!
    @brief  Read a byte from the specified register
*/
/**************************************************************************/
byte Adafruit_OM9663::read8(byte reg) {
  uint8_t resp = 0;
//  uint8_t tx[2] = {0};
//  uint8_t rx[2] = {0};
//  uint8_t timeout = 0xFF;

  CPRINTF("Requesting 1 byte from 0x%02X", reg);

  switch (_transport) {
  case OM9663_TRANSPORT_I2C:
/* I2C */
//#ifdef __SAM3X8E__
//    /* http://forum.arduino.cc/index.php?topic=385377.msg2947227#msg2947227 */
//    _wire->requestFrom(_i2c_addr, 1, reg, 1, true);
//#else
//    _wire->beginTransmission(_i2c_addr);
//    _wire->write(reg);
//    _wire->endTransmission();
////    _wire->requestFrom((uint8_t)_i2c_addr, (uint8_t)1);   // mike 20211125
//    _wire->requestFrom((uint8_t)_i2c_addr, (uint8_t)1, true);
//#endif
//    /* Dump the response into the supplied buffer */
//    resp = _wire->read();
/*    if (_wire_bus == 0) {
      Wire.beginTransmission(_i2c_addr);
      Wire.write(reg);
      Wire.endTransmission();
      Wire.requestFrom(_i2c_addr, (uint8_t)1); 
      resp = Wire.read();
    } else {
      Wire1.beginTransmission(_i2c_addr);
      Wire1.write(reg);
      Wire1.endTransmission();
      Wire1.requestFrom((uint8_t)_i2c_addr, (uint8_t)1);
      resp = Wire1.read();
    } */
    if (_wire_bus == 0) {
      Wire.beginTransmission(_i2c_addr);
      Wire.write(reg);
      Wire.endTransmission(false);
      Wire.requestFrom((uint16_t)_i2c_addr, (uint8_t)1, true); 
      resp = Wire.read();
    } else {
      Wire1.beginTransmission(_i2c_addr);
      Wire1.write(reg);
      Wire1.endTransmission(false);
      Wire1.requestFrom((uint16_t)_i2c_addr, (uint8_t)1, true);
      resp = Wire1.read();
    }
    break;
  case OM9663_TRANSPORT_SPI:
    /* SPI */
/*    digitalWrite(_cs, LOW);
    tx[0] = (reg << 1) | 0x01;
    tx[1] = 0;
    rx[0] = SPI.transfer(tx[0]);
    rx[1] = SPI.transfer(tx[1]);
    digitalWrite(_cs, HIGH);
    resp = rx[1]; */
    break;
  case OM9663_TRANSPORT_SERIAL:
/*    tx[0] = (reg << 1) | 0x01;
    _serial->write(tx[0]);
    while (!_serial->available()) {
//      delay(1);
      vTaskDelay(1/portTICK_PERIOD_MS);
      timeout--;
      if (timeout == 0) {
        return 0;
      }
    }
    resp = _serial->read();
//    delay(1);
    vTaskDelay(1/portTICK_PERIOD_MS);
    // Check for stray byte(s)
    while (_serial->available()) {
      _serial->read();
//      delay(1);
      vTaskDelay(1/portTICK_PERIOD_MS);
    } */
    break;
  }

  CPRINTF("Response = 0x%02X", resp);

  return resp;
}

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Instantiates a new instance of the Adafruit_OM9663 class
            using the default I2C bus.
*/
/**************************************************************************/
Adafruit_OM9663::Adafruit_OM9663(uint8_t i2c_addr, int8_t pdown_pin) {
  /* Set the transport */
  _transport = OM9663_TRANSPORT_I2C;

  /* Set the PDOWN pin */
  _pdown = pdown_pin;

  /* Set the I2C address */
  _i2c_addr = i2c_addr;

  /* Set the I2C bus instance */
//  _wire = &Wire;
  _wire_bus = 0;

  /* Disable SPI access. */
  _cs = -1;

  /* Disable SW serial access */
//  _serial = NULL;
}

/**************************************************************************/
/*!
    @brief  Instantiates a new instance of the Adafruit_OM9663 class
            using the specified I2C bus.
*/
/**************************************************************************/
// Adafruit_OM9663::Adafruit_OM9663(TwoWire *wireBus, uint8_t i2c_addr, int8_t pdown_pin) {
Adafruit_OM9663::Adafruit_OM9663(uint8_t wireBus, uint8_t i2c_addr, int8_t pdown_pin) {
  /* Set the transport */
  _transport = OM9663_TRANSPORT_I2C;

  /* Set the PDOWN pin */
  _pdown = pdown_pin;

  /* Set the I2C address */
  _i2c_addr = i2c_addr;

  /* Set the I2C bus instance */
//  _wire = wireBus;
  _wire_bus = wireBus;

  /* Disable SPI access. */
  _cs = -1;

  /* Disable SW serial access */
//  _serial = NULL;
}

/**************************************************************************/
/*!
    @brief  Instantiates a new instance of the Adafruit_OM9663 class
            using the HW SPI bus.
*/
/**************************************************************************/
//Adafruit_OM9663::Adafruit_OM9663(enum om9663_transport transport, int8_t cs, int8_t pdown_pin) {
//  /* Set the transport */
//  _transport = transport;
//
//  /* Set the PDOWN pin */
//  _pdown = pdown_pin;
//
//  /* Set the CS/SSEL pin */
//  _cs = cs;
//  pinMode(_cs, OUTPUT);
//
//  /* Disable I2C access */
////  _wire = NULL;
//  _wire_bus = 0;
//  _i2c_addr = 0;
//
//  /* Disable SW serial access */
////  _serial = NULL;
//}

/**************************************************************************/
/*!
    @brief  Instantiates a new instance of the Adafruit_OM9663 class
            using the specified Serial block.
*/
/**************************************************************************/
//Adafruit_OM9663::Adafruit_OM9663(Stream *serial, int8_t pdown_pin) {
//  /* Set the transport */
//  _transport = OM9663_TRANSPORT_SERIAL;
//
//  /* Set the PDOWN pin */
//  _pdown = pdown_pin;
//
//  /* Set the Serial instance */
//  _serial = serial;
//
//  /* Disable I2C access */
////  _wire = NULL;
//  _wire_bus = 0;
//  _i2c_addr = 0;
//
//  /* Disable SPI access. */
//  _cs = -1;
//}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Sets up the HW
*/
/**************************************************************************/
bool Adafruit_OM9663::begin() {
  /* Display alert for DEBUG and TRACE output. */
  CPRINTF("Debug NFC output enabled: Message %ld ms", millis()/1000);

  /* Enable I2C, SPI or SW serial */
  switch (_transport) {
  case OM9663_TRANSPORT_I2C:
    CPRINTF("Initialising I2C pdown %d i2c 0x%02X I2C bus %d", _pdown, _i2c_addr, _wire_bus);
//    _wire->begin();
    break;
  case OM9663_TRANSPORT_SPI:
    CPRINTF("Initialising SPI (Mode 0, MSB, DIV16)");
/*    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
#ifdef SPI_CLOCK_DIV16
    SPI.setClockDivider(SPI_CLOCK_DIV16);
#endif */
    break;
  case OM9663_TRANSPORT_SERIAL:
    /* NOTE: 'Serial' has to be initialised in the calling sketch! */
    break;
  }

  /* Reset the OM9663 if possible */
  if (_pdown != -1) {
    CPRINTF("Resetting OM9663");
//    pinMode(_pdown, OUTPUT);
    digitalWrite(_pdown, HIGH);
    digitalWrite(_pdown, LOW);
//    vTaskDelay(5/portTICK_PERIOD_MS);
    digitalWrite(_pdown, HIGH);
    digitalWrite(_pdown, LOW);
    /* Typical 2.5ms startup delay */
    // delay(5);
    vTaskDelay(5/portTICK_PERIOD_MS);
  }

  /* Check device ID for bus response */
  CPRINTF("Checking transport layer");

  /* Read the VERSION register */
  byte ver = read8(OM9663_REG_VERSION);
  CPRINTF("Version 0x%02X!", ver);

  /* If ver == 0xFF or 0x0 likely a bus failure */
  if ((ver == 0xFF) || (ver == 0)) {
    CPRINTF("Transport failure!");
    return false;
  }

  /* If !1.8, there was a problem */
  if (ver != 0x18) {
    CPRINTF("FAILED!");
    return false;
  }

  CPRINTF("IC Version = %X.%X", ((ver & 0xF0) >> 4), ver & 0x0F);

  return true;
}

/**************************************************************************/
/*!
    @brief  Determines the number of bytes in the HW FIFO buffer (max 512)

    @returns The number of bytes available in the HW FIFO buffer
*/
/**************************************************************************/
int16_t Adafruit_OM9663::readFIFOLen(void) {
/*  DEBUG_TIMESTAMP();
  DEBUG_PRINTLN(F("Checking FIFO length")); */
  CPRINTF("Checking FIFO length");

  /* Give FIFO a chance to fill up. */
  /* TODO: Why do we need a delay between reads?!? */
//  delay(10);
  vTaskDelay(10/portTICK_PERIOD_MS);

  /* Read the OM9663_REG_FIFO_LENGTH register */
  /* In 512 byte mode, the upper two bits are stored in FIFO_CONTROL */
  byte hi = read8(OM9663_REG_FIFO_CONTROL);
  byte lo = read8(OM9663_REG_FIFO_LENGTH);

  /* Determine len based on FIFO size (255 byte or 512 byte mode) */
  int16_t l = (hi & 0x80) ? lo : (((hi & 0x3) << 8) | lo);

/*  DEBUG_TIMESTAMP();
  DEBUG_PRINT(F("FIFO contains "));
  DEBUG_PRINT(l);
  DEBUG_PRINTLN(F(" byte(s)")); */
  CPRINTF("FIFO contains %d byte(s)", l);
  
  return l;
}

/**************************************************************************/
/*!
    @brief  Read 'len' bytes from the HW FIFO buffer (max 512 bytes)

    @returns The number of bytes read from the FIFO, or -1 if an error occured.
*/
/**************************************************************************/
int16_t Adafruit_OM9663::readFIFO(uint16_t len, uint8_t *buffer) {
  int16_t counter = 0;

  /* Check for 512 byte overflow */
  if (len > 512) {
    return -1;
  }

  CPRINTF("Reading %d byte(s) from FIFO", len);

  /* Read len bytes from the FIFO */
  for (uint16_t i = 0; i < len; i++) {
    buffer[i] = read8(OM9663_REG_FIFO_DATA);
    counter++;
  }

  return counter;
}

/**************************************************************************/
/*!
    @brief  Writes the specified number of bytes to the HW FIFO

    @returns The number of bytes written to the FIFO, -1 if an error occured.
*/
/**************************************************************************/
int16_t Adafruit_OM9663::writeFIFO(uint16_t len, uint8_t *buffer) {
  int counter = 0;

  /* Check for 512 byte overflow */
  if (len > 512) {
    return -1;
  }
  CPRINTF("Writing %d byte(s) to FIFO", len);
  /* Write len bytes to the FIFO */
  for (uint16_t i = 0; i < len; i++) {
    write8(OM9663_REG_FIFO_DATA, buffer[i]);
    counter++;
  }

  return counter;
}

/**************************************************************************/
/*!
    @brief  Flushes the contents of the FIFo buffer
*/
/**************************************************************************/
void Adafruit_OM9663::clearFIFO(void) {
  CPRINTF("Clearing FIFO buffer ");
  uint8_t ctrl = read8(OM9663_REG_FIFO_CONTROL);
  write8(OM9663_REG_FIFO_CONTROL, ctrl | (1 << 4));
}

/**************************************************************************/
/*!
    @brief  Writes a parameter-less command to the internal state machine
*/
/**************************************************************************/
void Adafruit_OM9663::writeCommand(byte command) {
  uint8_t buff[1] = {command};
  CPRINTF("Sending CMD 0x%02X", command);
  writeBuffer(OM9663_REG_COMMAND, 1, buff);
}

/**************************************************************************/
/*!
    @brief  Writes a parameterised command to the internal state machine
*/
/**************************************************************************/
void Adafruit_OM9663::writeCommand(byte command, uint8_t paramlen, uint8_t *params) {
  /* Arguments and/or data necessary to process a command are exchanged via
     the FIFO buffer:

     - Each command that needs a certain number of arguments will start
       processing only when it has received the correct number of arguments
       via the FIFO buffer.
     - The FIFO buffer is not cleared automatically at command start. It is
       recommended to write the command arguments and/or the data bytes into
       the FIFO buffer and start the command afterwards.
     - Each command may be stopped by the host by writing a new command code
       into the command register e.g.: the Idle-Command. */
  CPRINTF("Sending Command");

  /* Cancel any current command. */
  write8(OM9663_REG_COMMAND, OM9663_CMD_IDLE);

  /* Flush the FIFO */
  clearFIFO();

  /* Write data to the FIFO */
  writeFIFO(paramlen, params);

  /* Send the command */
  write8(OM9663_REG_COMMAND, command);
}

/**************************************************************************/
/*!
    @brief  Gets the three bit COM status for the IC
*/
/**************************************************************************/
uint8_t Adafruit_OM9663::getComStatus(void) {
  return (read8(OM9663_REG_STATUS) & 0b111);
}

/**************************************************************************/
/*!
    @brief  Performs a soft reset of the IC
*/
/**************************************************************************/
void Adafruit_OM9663::softReset(void) {
  writeCommand(OM9663_CMD_SOFTRESET);
//  delay(100);
  vTaskDelay(100/portTICK_PERIOD_MS);
}
/**************************************************************************/
/*!
    @brief  Prints a human readable error code.
*/
/**************************************************************************/
void Adafruit_OM9663::printError(enum om9663errors err) {
/*  ERROR_TIMESTAMP();
  ERROR_PRINT(F("ERROR! Danger, Will Robinson: "));

  switch (err) {
  case OM9663_ERROR_INTEG:
    ERROR_PRINTLN(F("Data integrity!"));
    break;
  case OM9663_ERROR_PROT:
    ERROR_PRINTLN(F("Protocol error!"));
    break;
  case OM9663_ERROR_COLLDET:
    ERROR_PRINTLN(F("Collision detected!"));
    break;
  case OM9663_ERROR_NODATA:
    ERROR_PRINTLN(F("No data!"));
    break;
  case OM9663_ERROR_MINFRAME:
    ERROR_PRINTLN(F("Frame data too small!"));
    break;
  case OM9663_ERROR_FIFOOVL:
    ERROR_PRINTLN(F("FIFO full!"));
    break;
  case OM9663_ERROR_FIFOWR:
    ERROR_PRINTLN(F("Couldn't write to FIFO!"));
    break;
  case OM9663_ERROR_EEPROM:
    ERROR_PRINTLN(F("EEPROM access!"));
    break;
  default:
    ERROR_PRINT(F("Unhandled error code: "));
    ERROR_PRINTLN(err, HEX);
    break;
  }

  // Halt execution here if we're not in release mode!
#if OM9663_VERBOSITY > OM9663_VERBOSITY_RELEASE
  while (1) {
    delay(1);
  }
#endif */
  CPRINTF("ERROR! Danger, Will Robinson: ");
  switch (err) {
  case OM9663_ERROR_INTEG:
    CPRINTF("Data integrity!");
    break;
  case OM9663_ERROR_PROT:
    CPRINTF("Protocol error!");
    break;
  case OM9663_ERROR_COLLDET:
    CPRINTF("Collision detected!");
    break;
  case OM9663_ERROR_NODATA:
    CPRINTF("No data!");
    break;
  case OM9663_ERROR_MINFRAME:
    CPRINTF("Frame data too small!");
    break;
  case OM9663_ERROR_FIFOOVL:
    CPRINTF("FIFO full!");
    break;
  case OM9663_ERROR_FIFOWR:
    CPRINTF("Couldn't write to FIFO!");
    break;
  case OM9663_ERROR_EEPROM:
    CPRINTF("EEPROM access!");
    break;
  default:
    CPRINTF("Unhandled error code: %02X", err);
    break;
  }
}

/**************************************************************************/
/*!
    @brief  Configures the radio for the specified protocol
*/
/**************************************************************************/
bool Adafruit_OM9663::configRadio(om9663radiocfg cfg) {
  CPRINTF("Configuring the radio for ");

  switch (cfg) {
  case OM9663_RADIOCFG_ISO1443A_106:
    CPRINTF("ISO1443A-106");
    writeBuffer(OM9663_REG_DRV_MOD, sizeof(antcfg_iso14443a_106), antcfg_iso14443a_106);

    CPRINTF("Setting driver mode");
    write8(OM9663_REG_DRV_MOD, 0x8E); /* Driver mode register */

    CPRINTF("Setting transmitter amplifier (residual carrier %)");
    write8(OM9663_REG_TX_AMP, 0x12); /* Transmiter amplifier register */

    CPRINTF("Setting driver configuration");
    write8(OM9663_REG_DRV_CON, 0x39); /* Driver configuration register */

    CPRINTF("Configuring transmitter (overshoot/TX load)");
    write8(OM9663_REG_TXL, 0x06); /* Transmitter register */
    break;
  default:
    CPRINTF("[UNKNOWN!]");
    return false;
    break;
  }
  return true;
}

uint16_t Adafruit_OM9663::iso14443aRequest(void) {
  return iso14443aCommand(ISO14443_CMD_REQA);
}

uint16_t Adafruit_OM9663::iso14443aWakeup(void) {
  return iso14443aCommand(ISO14443_CMD_WUPA);
}

uint16_t Adafruit_OM9663::iso14443aCommand(enum iso14443_cmd cmd) {
  uint16_t atqa = 0; /* Answer to request (2 bytes). */

  CPRINTF("Checking for an ISO14443A tag");

  /* Cancel any current command */
  writeCommand(OM9663_CMD_IDLE);

  /* Flush the FIFO */
  clearFIFO();

  /*
   * Define the number of bits from the last byte should be sent. 000 means
   * that all bits of the last data byte are sent, 1..7 causes the specified
   * number of bits to be sent. Also set the DataEn bit to enable data xfer.
   */
  write8(OM9663_REG_TX_DATA_NUM, 0x07 | (1 << 3));

  /* Disable CRC. */
  CPRINTF("A. Disabling CRC checks.");
  write8(OM9663_REG_TX_CRC_PRESET, 0x18);
  write8(OM9663_REG_RX_CRC_CON, 0x18);

  /* Clear the receiver control register. */
  CPRINTF("B. Clearing the receiver control register.");
  write8(OM9663_REG_RX_BIT_CTRL, 0);

  /* Clear the interrupts. */
  CPRINTF("C. Clearing and configuring interrupts.");
  write8(OM9663_REG_IRQ0, 0b01111111);
  write8(OM9663_REG_IRQ1, 0b00111111);
  /* Allow the receiver and Error IRQs to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQOEN, OM9663IRQ0_RXIRQ | OM9663IRQ0_ERRIRQ);
  /* Allow Timer0 IRQ to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQ1EN, OM9663IRQ1_TIMER0IRQ);

  /* Configure the frame wait timeout using T0 (5ms max). */
  CPRINTF("D. Configuring Timer0 @ 211.875kHz, post TX, 5ms timeout.");
  write8(OM9663_REG_T0_CONTROL, 0b10001);
  write8(OM9663_REG_T0_RELOAD_HI, 1100 >> 8);
  write8(OM9663_REG_TO_RELOAD_LO, 0xFF);
  write8(OM9663_REG_T0_COUNTER_VAL_HI, 1100 >> 8);
  write8(OM9663_REG_T0_COUNTER_VAL_LO, 0xFF);

  /* Send the ISO14443 command. */
  CPRINTF("E. Sending ISO14443 command.");
  uint8_t send_req[] = {(uint8_t)cmd};
  writeCommand(OM9663_CMD_TRANSCEIVE, 1, send_req);

  /* Wait here until we're done reading, get an error, or timeout. */
  /* TODO: Update to use timeout parameter! */
  CPRINTF("F. Waiting for a response or timeout.");
  uint8_t irqval = 0;
  while (!(irqval & OM9663IRQ1_TIMER0IRQ)) {
    irqval = read8(OM9663_REG_IRQ1);
    /* Check for a global interrrupt, which can only be ERR or RX. */
    if (irqval & OM9663IRQ1_GLOBALIRQ) {
      break;
    }
  }

  /* Cancel the current command (in case we timed out or error occurred). */
  writeCommand(OM9663_CMD_IDLE);

  /* Check the RX IRQ, and exit appropriately if it has fired (error). */
  irqval = read8(OM9663_REG_IRQ0);
  if ((!(irqval & OM9663IRQ0_RXIRQ) || (irqval & OM9663IRQ0_ERRIRQ))) {
    CPRINTF("ERROR: No RX flag set, transceive failed or timed out.");
    /* Display the error message if ERROR IRQ is set. */
    if (irqval && OM9663IRQ0_ERRIRQ) {
      uint8_t error = read8(OM9663_REG_ERROR);
      /* Only display the error if it isn't a timeout. */
      if (error) {
        printError((enum om9663errors)error);
      }
    }
    return 0;
  }

  /* Read the response */
  uint16_t rxlen = readFIFOLen();
  CPRINTF("G. Reading response from FIFO buffer.");
  if (rxlen == 2) {
    /*
     * If we have 2 bytes for the response, it's the ATQA.
     *
     * See ISO14443-3 6.3.2 for help in interpretting the ATQA value.
     *
     * "After a REQA Command is transmitted by the PCD, all
     * PICCs in the IDLE State shall respond synchronously with ATQA."
     *
     * 0x44 = 4 bit frame anticollision
     *        UID size = double
     */
    readFIFO(rxlen, (uint8_t *)&atqa);
    CPRINTF("Received response (ATQA): 0x%02X", atqa);
    return atqa;
  }

  return 0;
}

/*
 * For high level details on the selection and anti-collision protocols see
 * "Chip Type Identification Procedure" in
 * https://www.nxp.com/docs/en/application-note/AN10833.pdf
 */
uint8_t Adafruit_OM9663::iso14443aSelect(uint8_t *uid, uint8_t *sak) {
  CPRINTF("Selecting an ISO14443A tag");

  /* Cancel any current command */
  writeCommand(OM9663_CMD_IDLE);

  /* Flush the FIFO */
  clearFIFO();

  /* Allow the receiver and Error IRQs to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQOEN, OM9663IRQ0_RXIRQ | OM9663IRQ0_ERRIRQ);

  /* Allow Timer0 IRQ to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQ1EN, OM9663IRQ1_TIMER0IRQ);

  /* Configure the frame wait timeout using T0 (5ms max). */
  /* 1 'tick' 4.72us, so 1100 = 5.2ms */
  CPRINTF("A. Configuring Timer0 @ 211.875kHz, post TX, 5ms timeout.");
  write8(OM9663_REG_T0_CONTROL, 0b10001);
  write8(OM9663_REG_T0_RELOAD_HI, 1100 >> 8);
  write8(OM9663_REG_TO_RELOAD_LO, 0xFF);
  write8(OM9663_REG_T0_COUNTER_VAL_HI, 1100 >> 8);
  write8(OM9663_REG_T0_COUNTER_VAL_LO, 0xFF);

  /* Set the cascade level (collision detection loop) */
  CPRINTF("B. Checking cascade level (collision detection).");
  uint8_t cascadelvl;
  for (cascadelvl = 1; cascadelvl <= 3; cascadelvl++) {
    uint8_t cmd;
    uint8_t kbits = 0;                        /* Bits known in UID so far. */
    uint8_t send_req[7] = {0};                /* TX buffer */
    uint8_t *uid_this_level = &(send_req[2]); /* UID pointer */
    uint8_t message_length;

    CPRINTF("Cascade level %d", cascadelvl);

    switch (cascadelvl) {
    case 1:
      cmd = ISO14443_CAS_LEVEL_1;
      break;
    case 2:
      cmd = ISO14443_CAS_LEVEL_2;
      break;
    case 3:
      cmd = ISO14443_CAS_LEVEL_3;
      break;
    }

    /* Disable CRC. */
    CPRINTF("a. Disabling CRC checks.");
    write8(OM9663_REG_TX_CRC_PRESET, 0x18);
    write8(OM9663_REG_RX_CRC_CON, 0x18);

    /* As per ISO14443-3, limit coliision checks to 32 attempts. */
    uint8_t cnum;
    CPRINTF("b. Collision detection (max 32 attempts).");
    for (cnum = 0; cnum < 32; cnum++) {
      CPRINTF("Attempt = %d, known bits = %d ", cnum, kbits);
#if MFRC630_VERBOSITY >= OM9663_VERBOSITY_DEBUG
      NOA_PUB_Print_Buf_Hex(uid_this_level, (kbits + 8 - 1) / 8);
#endif
      /* Clear the interrupts. */
      write8(OM9663_REG_IRQ0, 0b01111111);
      write8(OM9663_REG_IRQ1, 0b00111111);

      /* Send the current collision level command */
      send_req[0] = cmd;
      send_req[1] = 0x20 + kbits;

      /* Limit OM9663_REG_TX_DATA_NUM to the correct number of bits. */
      write8(OM9663_REG_TX_DATA_NUM, (kbits % 8) | (1 << 3));

      // ValuesAfterColl: If cleared, every received bit after a collision is
      // replaced by a zero. This function is needed for ISO/IEC14443
      // anticollision (0<<7). We want to shift the bits with RxAlign
      uint8_t rxalign = kbits % 8;
      write8(OM9663_REG_RX_BIT_CTRL, (0 << 7) | (rxalign << 4));

      /* Determine the message length */
      if ((kbits % 8) == 0) {
        message_length = ((kbits / 8)) + 2;
      } else {
        message_length = ((kbits / 8) + 1) + 2;
      }

      /* Send the command. */
      writeCommand(OM9663_CMD_TRANSCEIVE, message_length, send_req);

      /* Wait until the command execution is complete. */
      uint8_t irq1_value = 0;
      while (!(irq1_value & OM9663IRQ1_TIMER0IRQ)) {
        irq1_value = read8(OM9663_REG_IRQ1);
        /* Check for a global interrrupt, which can only be ERR or RX. */
        if (irq1_value & OM9663IRQ1_GLOBALIRQ) {
          break;
        }
      }

      /* Cancel any current command */
      writeCommand(OM9663_CMD_IDLE);

      /* Parse results */
      uint8_t irq0_value = read8(OM9663_REG_IRQ0);
      uint8_t error = read8(OM9663_REG_ERROR);
      uint8_t coll = read8(OM9663_REG_RX_COLL);
      uint8_t coll_p = 0;

      /* Check if an error occured */
      if (irq0_value & OM9663IRQ0_ERRIRQ) {
        /* Display the error code in human-readable format. */
        printError((enum om9663errors)error);
        if (error & OM9663_ERROR_COLLDET) {
          /* Collision error, check if the collision position is valid */
          if (coll & (1 << 7)) {
            /* Valid, so check the collision position (bottom 7 bits). */
            coll_p = coll & (~(1 << 7));
            CPRINTF("Bit collision detected at bit %d", coll_p);

            uint8_t choice_pos = kbits + coll_p;
            uint8_t selection = (uid[((choice_pos + (cascadelvl - 1) * 3) / 8)] >> ((choice_pos) % 8)) & 1;
            uid_this_level[((choice_pos) / 8)] |= selection << ((choice_pos) % 8);
            kbits++;

            CPRINTF("'uid_this_level' is now %d:", kbits);
#if MFRC630_VERBOSITY >= OM9663_VERBOSITY_DEBUG
            NOA_PUB_Print_Buf_Hex(uid_this_level, 10);
#endif
          } else {
            /* Invalid collision position (bit 7 = 0) */
            CPRINTF("Bit collision detected, but no valid position.");
            coll_p = 0x20 - kbits;
          } /* End: if (coll & (1 << 7)) */
        } else {
          CPRINTF("Unhandled error.");
          coll_p = 0x20 - kbits;
        } /* End: if (error & OM9663_ERROR_COLLDET) */
      } else if (irq0_value & OM9663IRQ0_RXIRQ) {
        /* We have data and no collision, all is well in the world! */
        coll_p = 0x20 - kbits;
        CPRINTF("Received data, no bit collision!");
      } else {
        /* Probably no card */
        CPRINTF("No error and no data = No card");
        return 0;
      } /* End: if (irq0_value & (1 << 1)) */

      /* Read the UID so far */
      uint16_t rxlen = readFIFOLen();
      uint8_t buf[5]; /* UID = 4 bytes + BCC */
      readFIFO(rxlen < 5 ? rxlen : 5, buf);

      /*
       * Move current buffer contents into the UID placeholder, OR'ing the
       * results so that we don't lose the bit we set if you have a collision.
       */
      uint8_t rbx;
      for (rbx = 0; (rbx < rxlen); rbx++) {
        uid_this_level[(kbits / 8) + rbx] |= buf[rbx];
      }
      kbits += coll_p;

      if ((kbits >= 32)) {
        CPRINTF("Leaving collision loop: uid %d bits long", kbits);
#if MFRC630_VERBOSITY >= OM9663_VERBOSITY_DEBUG
        NOA_PUB_Print_Buf_Hex(uid_this_level, kbits / 8);
#endif
        break; /* Exit the collision loop */
      }
    } /* End: for (cnum = 0; cnum < 32; cnum++) */

    /* Check if the BCC matches ... */
    CPRINTF("C. Checking BCC for data integrity.");
    uint8_t bcc_val = uid_this_level[4];
    uint8_t bcc_calc = uid_this_level[0] ^ uid_this_level[1] ^
                       uid_this_level[2] ^ uid_this_level[3];
    if (bcc_val != bcc_calc) {
      CPRINTF("ERROR: BCC mistmatch!");
      return 0;
    }

    /* Clear the interrupts. */
    CPRINTF("D. Clearing and configuring interrupts.");
    write8(OM9663_REG_IRQ0, 0b01111111);
    write8(OM9663_REG_IRQ1, 0b00111111);

    send_req[0] = cmd;
    send_req[1] = 0x70;
    send_req[6] = bcc_calc;
    message_length = 7;

    /* Re-enable CRCs. */
    write8(OM9663_REG_TX_CRC_PRESET, 0x18 | 1);
    write8(OM9663_REG_RX_CRC_CON, 0x18 | 1);

    /* Reset the TX and RX registers (disable alignment, transmit full bytes) */
    write8(OM9663_REG_TX_DATA_NUM, (kbits % 8) | (1 << 3));
    uint8_t rxalign = 0;
    write8(OM9663_REG_RX_BIT_CTRL, (0 << 7) | (rxalign << 4));

    /* Send the command. */
    CPRINTF("E. Sending collision command");
    writeCommand(OM9663_CMD_TRANSCEIVE, message_length, send_req);

    /* Wait until the command execution is complete. */
    uint8_t irq1_value = 0;
    while (!(irq1_value & OM9663IRQ1_TIMER0IRQ)) {
      irq1_value = read8(OM9663_REG_IRQ1);
      /* Check for a global interrrupt, which can only be ERR or RX. */
      if (irq1_value & OM9663IRQ1_GLOBALIRQ) {
        break;
      }
    }
    writeCommand(OM9663_CMD_IDLE);

    /* Check the source of exiting the loop. */
    CPRINTF("F. Command complete, verifying proper exit.");
    uint8_t irq0_value = read8(OM9663_REG_IRQ0);
    /* Check the ERROR IRQ */
    if (irq0_value & OM9663IRQ0_ERRIRQ) {
      /* Check what kind of error. */
      uint8_t error = read8(OM9663_REG_ERROR);
      if (error & OM9663_ERROR_COLLDET) {
        /* Collision detecttion. */
        printError(OM9663_ERROR_COLLDET);
        return 0;
      }
    }

    /* Read SAK answer from fifo. */
    CPRINTF("G. Checking SAK in response payload.");
    uint8_t sak_len = readFIFOLen();
    if (sak_len != 1) {
      CPRINTF("ERROR: NO SAK in response!");
      return 0;
    }
    uint8_t sak_value;
    readFIFO(sak_len, &sak_value);

    CPRINTF("SAK answer: %d", sak_value);

    /* Check if there is more data to read. */
    if (sak_value & (1 << 2)) {
      /* UID not yet complete, continue to next cascade. */
      CPRINTF("UID not complete ... looping to next cascade level.");
      uint8_t UIDn;
      for (UIDn = 0; UIDn < 3; UIDn++) {
        // uid_this_level[UIDn] = uid_this_level[UIDn + 1];
        uid[(cascadelvl - 1) * 3 + UIDn] = uid_this_level[UIDn + 1];
      }
    } else {
      CPRINTF("DONE! UID fully parsed, exiting.");
      /* Done! */
      /* Add current bytes at this level to the UID. */
      uint8_t UIDn;
      for (UIDn = 0; UIDn < 4; UIDn++) {
        uid[(cascadelvl - 1) * 3 + UIDn] = uid_this_level[UIDn];
      }

      /* Finally, return the length of the UID that's now at the uid pointer. */
      return cascadelvl * 3 + 1;
    }

    CPRINTF("Exiting cascade loop");
  } /* End: for (cascadelvl = 1; cascadelvl <= 3; cascadelvl++) */

  /* Return 0 for UUID length if nothing was found. */
  return 0;
}

void Adafruit_OM9663::mifareLoadKey(uint8_t *key) {
//  DEBUG_TIMESTAMP();
//  DEBUG_PRINTLN(F("Loading Mifare key into crypto unit."));
  CPRINTF("Loading Mifare key into crypto unit.");

  writeCommand(OM9663_CMD_IDLE);
  clearFIFO();
  writeFIFO(6, key);
  writeCommand(OM9663_CMD_LOADKEY);
}

bool Adafruit_OM9663::mifareAuth(uint8_t key_type, uint8_t blocknum,
                                  uint8_t *uid) {
/*  DEBUG_TIMESTAMP();
  DEBUG_PRINT(F("Authenticating Mifare block "));
  DEBUG_PRINTLN(blocknum); */
  CPRINTF("Authenticating Mifare block %d", blocknum);

  writeCommand(OM9663_CMD_IDLE);
  clearFIFO();

  /* Allow the IDLE and Error IRQs to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQOEN, OM9663IRQ0_IDLEIRQ | OM9663IRQ0_ERRIRQ);
  /* Allow Timer0 IRQ to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQ1EN, OM9663IRQ1_TIMER0IRQ);

  /* Configure the frame wait timeout using T0 (10ms max). */
  /* 1 'tick' 4.72us, so 2000 = ~10ms */
//  DEBUG_TIMESTAMP();
//  DEBUG_PRINTLN(F("Configuring Timer0 @ 211.875kHz, post TX, 10ms timeout."));
  CPRINTF("Configuring Timer0 @ 211.875kHz, post TX, 10ms timeout.");
  write8(OM9663_REG_T0_CONTROL, 0b10001);
  write8(OM9663_REG_T0_RELOAD_HI, 2000 >> 8);
  write8(OM9663_REG_TO_RELOAD_LO, 0xFF);
  write8(OM9663_REG_T0_COUNTER_VAL_HI, 2000 >> 8);
  write8(OM9663_REG_T0_COUNTER_VAL_LO, 0xFF);

  /* Clear interrupts. */
  write8(OM9663_REG_IRQ0, 0b01111111);
  write8(OM9663_REG_IRQ1, 0b00111111);

  /* Start of AUTH procedure. */
  writeCommand(OM9663_CMD_IDLE);
  clearFIFO();

  /*
   * MFAUTHENT command has the following parameters:
   * [0]    Key type (0x60 = KEYA, 0x61 = KEYB)
   * [1]    Block address
   * [2]    UID byte 0
   * [3]    UID byte 1
   * [4]    UID byte 2
   * [5]    UID byte 3
   *
   * NOTE: When the MFAuthent command is active, any FIFO access is blocked!
   */
  uint8_t params[6] = {key_type, blocknum, uid[0], uid[1], uid[2], uid[3]};
  writeFIFO(6, params);
  writeCommand(OM9663_CMD_MFAUTHENT);

  /*
   * This command terminates automatically when the MIFARE Classic card is
   * authenticated and the bit MFCrypto1On is set to logic 1.
   *
   * This command does not terminate automatically when the card does not
   * answer, therefore the timer should be initialized to automatic mode. In
   * this case, beside the bit IdleIRQ the bit TimerIRQ can be used as
   * termination criteria. During authentication processing the bits RxIRQ
   * and TxIRQ are blocked. The Crypto1On shows if the authentication was
   * successful. The Crypto1On is always valid.
   *
   * In case there is an error during authentication, the bit ProtocolErr in
   * the Error register is set to logic 1 and the bit Crypto1On in register
   * Status2Reg is set to logic 0.
   */

  /* Wait until the command execution is complete. */
  uint8_t irq1_value = 0;
  while (!(irq1_value & OM9663IRQ1_TIMER0IRQ)) {
    irq1_value = read8(OM9663_REG_IRQ1);
    /* Check for a global interrrupt, which can only be ERR or RX. */
    if (irq1_value & OM9663IRQ1_GLOBALIRQ) {
      break;
    }
  }

#if 0
  uint8_t irq0_value = read8(OM9663_REG_IRQ0);
  uint8_t error = read8(OM9663_REG_ERROR);
  uint8_t status = read8(OM9663_REG_STATUS);
  Serial.print(F("ERROR : ")); Serial.println(error, HEX);
  Serial.print(F("IRQ0  : ")); Serial.println(irq0_value, HEX);
  Serial.print(F("IRQ1  : ")); Serial.println(irq1_value, HEX);
  Serial.print(F("STATUS: ")); Serial.println(status, HEX);
#endif

  /* Check the error flag (OM9663_ERROR_PROT, etc.) */
  uint8_t error = read8(OM9663_REG_ERROR);
  if (error) {
    printError((enum om9663errors)error);
    return false;
  }

  /* Check if we timed out or got a response. */
  if (irq1_value & OM9663IRQ1_TIMER0IRQ) {
    /* Timed out, no auth! :( */
    return false;
  }

  /* Check the status register for CRYPTO1 flag (Mifare AUTH). */
  uint8_t status = read8(OM9663_REG_STATUS);
  return (status & OM9663STATUS_CRYPTO1ON) ? true : false;
}

uint16_t Adafruit_OM9663::mifareReadBlock(uint8_t blocknum, uint8_t *buf) {
  clearFIFO();

  /* Enable CRC. */
//  DEBUG_TIMESTAMP();
//  DEBUG_PRINTLN(F("A. Disabling CRC checks."));
  CPRINTF("A. Disabling CRC checks.");
  write8(OM9663_REG_TX_CRC_PRESET, 0x18 | 1);
  write8(OM9663_REG_RX_CRC_CON, 0x18 | 1);

  /* Allow the IDLE and Error IRQs to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQOEN, OM9663IRQ0_IDLEIRQ | OM9663IRQ0_ERRIRQ);
  /* Allow Timer0 IRQ to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQ1EN, OM9663IRQ1_TIMER0IRQ);

  /* Configure the frame wait timeout using T0 (10ms max). */
  /* 1 'tick' 4.72us, so 2000 = ~10ms */
//  DEBUG_TIMESTAMP();
//  DEBUG_PRINTLN(F("Configuring Timer0 @ 211.875kHz, post TX, 10ms timeout."));
  CPRINTF("Configuring Timer0 @ 211.875kHz, post TX, 10ms timeout.");
  write8(OM9663_REG_T0_CONTROL, 0b10001); /* Start at end of TX, 211kHz */
  write8(OM9663_REG_T0_RELOAD_HI, 0xFF);
  write8(OM9663_REG_TO_RELOAD_LO, 0xFF);
  write8(OM9663_REG_T0_COUNTER_VAL_HI, 0xFF);
  write8(OM9663_REG_T0_COUNTER_VAL_LO, 0xFF);

  /* Clear interrupts. */
  write8(OM9663_REG_IRQ0, 0b01111111);
  write8(OM9663_REG_IRQ1, 0b00111111);

  /* Transceive the command. */
  uint8_t req[2] = {MIFARE_CMD_READ, blocknum};
  writeCommand(OM9663_CMD_TRANSCEIVE, 2, req);

  /* Wait until the command execution is complete. */
  uint8_t irq1_value = 0;
  while (!(irq1_value & OM9663IRQ1_TIMER0IRQ)) {
    irq1_value = read8(OM9663_REG_IRQ1);
    /* Check for a global interrrupt, which can only be ERR or RX. */
    if (irq1_value & OM9663IRQ1_GLOBALIRQ) {
      break;
    }
  }
  writeCommand(OM9663_CMD_IDLE);

  /* Check if we timed out or got a response. */
  if (irq1_value & OM9663IRQ1_TIMER0IRQ) {
    /* Timed out, no auth :( */
//    DEBUG_PRINTLN(F("TIMED OUT!"));
    CPRINTF("TIMED OUT!");
    return 0;
  }

  /* Read the size and contents of the FIFO, and return the results. */
  uint16_t buffer_length = readFIFOLen();
  uint16_t rx_len = (buffer_length <= 16) ? buffer_length : 16;
  readFIFO(rx_len, buf);

  return rx_len;
}

uint16_t Adafruit_OM9663::ntagReadPage(uint16_t pagenum, uint8_t *buf) {
  clearFIFO();

  /* Enable CRC. */
//  DEBUG_TIMESTAMP();
//  DEBUG_PRINTLN(F("A. Disabling CRC checks."));
  CPRINTF("A. Disabling CRC checks.");
  write8(OM9663_REG_TX_CRC_PRESET, 0x18 | 1);
  write8(OM9663_REG_RX_CRC_CON, 0x18 | 1);

  /* Allow the IDLE and Error IRQs to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQOEN, OM9663IRQ0_IDLEIRQ | OM9663IRQ0_ERRIRQ);
  /* Allow Timer0 IRQ to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQ1EN, OM9663IRQ1_TIMER0IRQ);

  /* Configure the frame wait timeout using T0 (10ms max). */
  /* 1 'tick' 4.72us, so 2000 = ~10ms */
//  DEBUG_TIMESTAMP();
//  DEBUG_PRINTLN(F("Configuring Timer0 @ 211.875kHz, post TX, 10ms timeout."));
  CPRINTF("Configuring Timer0 @ 211.875kHz, post TX, 10ms timeout.");
  write8(OM9663_REG_T0_CONTROL, 0b10001); /* Start at end of TX, 211kHz */
  write8(OM9663_REG_T0_RELOAD_HI, 0xFF);
  write8(OM9663_REG_TO_RELOAD_LO, 0xFF);
  write8(OM9663_REG_T0_COUNTER_VAL_HI, 0xFF);
  write8(OM9663_REG_T0_COUNTER_VAL_LO, 0xFF);

  /* Clear interrupts. */
  write8(OM9663_REG_IRQ0, 0b01111111);
  write8(OM9663_REG_IRQ1, 0b00111111);

  /* Transceive the command. */
  uint8_t req[2] = {(uint8_t)NTAG_CMD_READ, (uint8_t)pagenum};
  writeCommand(OM9663_CMD_TRANSCEIVE, 2, req);

  /* Wait until the command execution is complete. */
  uint8_t irq1_value = 0;
  while (!(irq1_value & OM9663IRQ1_TIMER0IRQ)) {
    irq1_value = read8(OM9663_REG_IRQ1);
    /* Check for a global interrrupt, which can only be ERR or RX. */
    if (irq1_value & OM9663IRQ1_GLOBALIRQ) {
      break;
    }
  }
  writeCommand(OM9663_CMD_IDLE);

  /* Check if we timed out or got a response. */
  if (irq1_value & OM9663IRQ1_TIMER0IRQ) {
    /* Timed out, no auth :( */
//    DEBUG_PRINTLN(F("TIMED OUT!"));
    CPRINTF("TIMED OUT!");
    return 0;
  }

  /* Read the size and contents of the FIFO, and return the results. */
  uint16_t buffer_length = readFIFOLen();
  uint16_t rx_len = (buffer_length <= 4) ? buffer_length : 4;
  readFIFO(rx_len, buf);

  return rx_len;
}

uint16_t Adafruit_OM9663::mifareWriteBlock(uint16_t blocknum, uint8_t *buf) {
  clearFIFO();

/*  DEBUG_TIMESTAMP();
  DEBUG_PRINT(F("Writing data to card @ 0x"));
  DEBUG_PRINTLN(blocknum); */
  CPRINTF("Writing data to card @ 0x%02X", blocknum);

  /* Enable CRC for TX (RX off!). */
//  DEBUG_TIMESTAMP();
//  DEBUG_PRINTLN(F("A. Disabling CRC checks."));
  CPRINTF("A. Disabling CRC checks.");
  write8(OM9663_REG_TX_CRC_PRESET, 0x18 | 1);
  write8(OM9663_REG_RX_CRC_CON, 0x18 | 0);

  /* Allow the IDLE and Error IRQs to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQOEN, OM9663IRQ0_IDLEIRQ | OM9663IRQ0_ERRIRQ);
  /* Allow Timer0 IRQ to be propagated to the GlobalIRQ. */
  write8(OM9663_REG_IRQ1EN, OM9663IRQ1_TIMER0IRQ);

  /* Configure the frame wait timeout using T0 (10ms max). */
  /* 1 'tick' 4.72us, so 2000 = ~10ms */
//  DEBUG_TIMESTAMP();
//  DEBUG_PRINTLN(F("Configuring Timer0 @ 211.875kHz, post TX, 10ms timeout."));
  CPRINTF("Configuring Timer0 @ 211.875kHz, post TX, 10ms timeout.");
  write8(OM9663_REG_T0_CONTROL, 0b10001); /* Start at end of TX, 211kHz */
  write8(OM9663_REG_T0_RELOAD_HI, 0xFF);
  write8(OM9663_REG_TO_RELOAD_LO, 0xFF);
  write8(OM9663_REG_T0_COUNTER_VAL_HI, 0xFF);
  write8(OM9663_REG_T0_COUNTER_VAL_LO, 0xFF);

  /* Clear interrupts. */
  write8(OM9663_REG_IRQ0, 0b01111111);
  write8(OM9663_REG_IRQ1, 0b00111111);

  /* Transceive the WRITE command. */
  uint8_t req1[2] = {(uint8_t)MIFARE_CMD_WRITE, (uint8_t)blocknum};
  writeCommand(OM9663_CMD_TRANSCEIVE, sizeof(req1), req1);

  /* Wait until the command execution is complete. */
  uint8_t irq1_value = 0;
  while (!(irq1_value & OM9663IRQ1_TIMER0IRQ)) {
    irq1_value = read8(OM9663_REG_IRQ1);
    /* Check for a global interrrupt, which can only be ERR or RX. */
    if (irq1_value & OM9663IRQ1_GLOBALIRQ) {
      break;
    }
  }
  writeCommand(OM9663_CMD_IDLE);

  /* Check if we timed out or got a response. */
  if (irq1_value & OM9663IRQ1_TIMER0IRQ) {
    /* Timed out, no auth :( */
//    DEBUG_PRINTLN(F("TIMED OUT!"));
    CPRINTF("TIMED OUT!");
    return 0;
  }

  /* Check if an error occured */
  uint8_t error = read8(OM9663_REG_ERROR);
  uint8_t irq0_value = read8(OM9663_REG_IRQ0);
  if (irq0_value & OM9663IRQ0_ERRIRQ) {
    printError((enum om9663errors)error);
    return 0;
  }

  /* We should have a single ACK byte in buffer at this point. */
  uint16_t buffer_length = readFIFOLen();
  if (buffer_length != 1) {
/*    DEBUG_TIMESTAMP();
    DEBUG_PRINT(F("Unexpected response buffer len: "));
    DEBUG_PRINTLN(buffer_length); */
    CPRINTF("Unexpected response buffer len: %d", buffer_length);
    return 0;
  }

  uint8_t ack = 0;
  readFIFO(1, &ack);
  if (ack != 0x0A) {
    /* Missing valid ACK response! */
/*    DEBUG_TIMESTAMP();
    DEBUG_PRINT(F("Invalid ACK response: "));
    DEBUG_PRINTLN(ack, HEX); */
    CPRINTF("Invalid ACK response: 0x%02X", ack);
    return 0;
  }

  /* TODO: Verift values! */
  /* Clear the interrupts. */
  write8(OM9663_REG_IRQ0, 0b01111111);
  write8(OM9663_REG_IRQ1, 0b00111111);

  /* Transfer the page data. */
  writeCommand(OM9663_CMD_TRANSCEIVE, 16, buf);

  /* Wait until the command execution is complete. */
  irq1_value = 0;
  while (!(irq1_value & OM9663IRQ1_TIMER0IRQ)) {
    irq1_value = read8(OM9663_REG_IRQ1);
    /* Check for a global interrrupt, which can only be ERR or RX. */
    if (irq1_value & OM9663IRQ1_GLOBALIRQ) {
      break;
    }
  }
  writeCommand(OM9663_CMD_IDLE);

  /* Check if we timed out or got a response. */
  if (irq1_value & OM9663IRQ1_TIMER0IRQ) {
    /* Timed out, no auth :( */
//    DEBUG_PRINTLN(F("TIMED OUT!"));
    CPRINTF("TIMED OUT!");
    return 0;
  }

  /* Check if an error occured */
  error = read8(OM9663_REG_ERROR);
  irq0_value = read8(OM9663_REG_IRQ0);
  if (irq0_value & OM9663IRQ0_ERRIRQ) {
    printError((enum om9663errors)error);
    return 0;
  }

  /* We should have a single ACK byte in buffer at this point. */
  buffer_length = readFIFOLen();
  if (buffer_length != 1) {
/*    DEBUG_TIMESTAMP();
    DEBUG_PRINT(F("Unexpected response buffer len: "));
    DEBUG_PRINTLN(buffer_length); */
    CPRINTF("Unexpected response buffer len: %d", buffer_length);
    return 0;
  }

  ack = 0;
  readFIFO(1, &ack);
  if (ack != 0x0A) {
    /* Missing valid ACK response! */
/*    DEBUG_TIMESTAMP();
    DEBUG_PRINT(F("Invalid ACK response: "));
    DEBUG_PRINTLN(ack, HEX); */
    CPRINTF("Invalid ACK response: %02X", ack);
    return 0;
  }

  return 16;
}

uint16_t Adafruit_OM9663::ntagWritePage(uint16_t pagenum, uint8_t *buf) {
  /*
   * For now, protect pages 0..3 and 40..44, and restrict writes to the safe
   * 'user memory' range (see docs/NTAG.md for further details).
   */
  if ((pagenum < 4) || (pagenum > 44)) {
/*    DEBUG_TIMESTAMP();
    DEBUG_PRINT(F("Page number out of range for NTAG213: "));
    DEBUG_PRINTLN(pagenum); */
    CPRINTF("Page number out of range for NTAG213: %d", pagenum);
    return 0;
  }

  /* Use the Mifare write, which is compatible with the NTAG cards. */
  return mifareWriteBlock(pagenum, buf) == 16 ? 4 : 0;
}
