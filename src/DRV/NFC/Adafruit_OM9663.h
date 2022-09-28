/*!
 * @file Adafruit_OM9663.h
 */
#ifndef __OM9663_H__
#define __OM9663_H__

#include "Arduino.h"
#include <Wire.h>

extern uint8_t antcfg_iso14443a_106[18];
extern uint8_t antcfg_iso14443a_212[18];
extern uint8_t antcfg_iso14443a_424[18];
extern uint8_t antcfg_iso14443a_848[18];

extern uint8_t protcfg_iso14443a_106[24];
extern uint8_t protcfg_iso14443a_212[24];
extern uint8_t protcfg_iso14443a_424[24];
extern uint8_t protcfg_iso14443a_848[24];

/*!
 * @brief OM9663 Command Set
 */
enum om9663reg {
  OM9663_REG_COMMAND = 0x00,
  OM9663_REG_HOST_CTRL = 0x01,
  OM9663_REG_FIFO_CONTROL = 0x02,
  OM9663_REG_WATER_LEVEL = 0x03,
  OM9663_REG_FIFO_LENGTH = 0x04,
  OM9663_REG_FIFO_DATA = 0x05,
  OM9663_REG_IRQ0 = 0x06,
  OM9663_REG_IRQ1 = 0x07,
  OM9663_REG_IRQOEN = 0x08,
  OM9663_REG_IRQ1EN = 0x09,
  OM9663_REG_ERROR = 0x0A,
  OM9663_REG_STATUS = 0x0B,
  OM9663_REG_RX_BIT_CTRL = 0x0C,
  OM9663_REG_RX_COLL = 0x0D,
  OM9663_REG_T_CONTROL = 0x0E,
  OM9663_REG_T0_CONTROL = 0x0F,
  OM9663_REG_T0_RELOAD_HI = 0x10,
  OM9663_REG_TO_RELOAD_LO = 0x11,
  OM9663_REG_T0_COUNTER_VAL_HI = 0x12,
  OM9663_REG_T0_COUNTER_VAL_LO = 0x13,
  OM9663_REG_T1_CONTROL = 0x14,
  OM9663_REG_T1_RELOAD_HI = 0x15,
  OM9663_REG_T1_RELOAD_LO = 0x16,
  OM9663_REG_T1_COUNTER_VAL_HI = 0x17,
  OM9663_REG_T1_COUNTER_VAL_LO = 0x18,
  OM9663_REG_T2_CONTROL = 0x19,
  OM9663_REG_T2_RELOAD_HI = 0x1A,
  OM9663_REG_T2_RELOAD_LO = 0x1B,
  OM9663_REG_T2_COUNTER_VAL_HI = 0x1C,
  OM9663_REG_T2_COUNTER_VAL_LO = 0x1D,
  OM9663_REG_T3_CONTROL = 0x1E,
  OM9663_REG_T3_RELOAD_HI = 0x1F,
  OM9663_REG_T3_RELOAD_LO = 0x20,
  OM9663_REG_T3_COUNTER_VAL_HI = 0x21,
  OM9663_REG_T3_COUNTER_VAL_LO = 0x22,
  OM9663_REG_T4_CONTROL = 0x23,
  OM9663_REG_T4_RELOAD_HI = 0x24,
  OM9663_REG_T4_RELOAD_LO = 0x25,
  OM9663_REG_T4_COUNTER_VAL_HI = 0x26,
  OM9663_REG_T4_COUNTER_VAL_LO = 0x27,

  /* 0x28..0x39 = Antenna Configuration */
  OM9663_REG_DRV_MOD = 0x28,             /**<  (ISO/IEC14443-A 106 = 0x8E) */
  OM9663_REG_TX_AMP = 0x29,              /**<  (ISO/IEC14443-A 106 = 0x12) */
  OM9663_REG_DRV_CON = 0x2A,             /**<  (ISO/IEC14443-A 106 = 0x39) */
  OM9663_REG_TXL = 0x2B,                 /**<  (ISO/IEC14443-A 106 = 0x0A) */
  OM9663_REG_TX_CRC_PRESET = 0x2C,       /**<  (ISO/IEC14443-A 106 = 0x18) */
  OM9663_REG_RX_CRC_CON = 0x2D,          /**<  (ISO/IEC14443-A 106 = 0x18) */
  OM9663_REG_TX_DATA_NUM = 0x2E,         /**<  (ISO/IEC14443-A 106 = 0x0F) */
  OM9663_REG_TX_MOD_WIDTH = 0x2F,        /**<  (ISO/IEC14443-A 106 = 0x21) */
  OM9663_REG_TX_SYM_10_BURST_LEN = 0x30, /**<  (ISO/IEC14443-A 106 = 0x00) */
  OM9663_REG_TX_WAIT_CTRL = 0x31,        /**<  (ISO/IEC14443-A 106 = 0xC0) */
  OM9663_REG_TX_WAIT_LO = 0x32,          /**<  (ISO/IEC14443-A 106 = 0x12) */
  OM9663_REG_FRAME_CON = 0x33,           /**<  (ISO/IEC14443-A 106 = 0xCF) */
  OM9663_REG_RX_SOFD = 0x34,             /**<  (ISO/IEC14443-A 106 = 0x00) */
  OM9663_REG_RX_CTRL = 0x35,             /**<  (ISO/IEC14443-A 106 = 0x04) */
  OM9663_REG_RX_WAIT = 0x36,             /**<  (ISO/IEC14443-A 106 = 0x90) */
  OM9663_REG_RX_THRESHOLD = 0x37,        /**<  (ISO/IEC14443-A 106 = 0x5C) */
  OM9663_REG_RCV = 0x38,                 /**<  (ISO/IEC14443-A 106 = 0x12) */
  OM9663_REG_RX_ANA = 0x39,              /**<  (ISO/IEC14443-A 106 = 0x0A) */

  OM9663_REG_RFU_LPCD = 0x3A,
  OM9663_REG_SERIAL_SPEED = 0x3B,
  OM9663_REG_LFO_TRIMM = 0x3C,
  OM9663_REG_PLL_CTRL = 0x3D,
  OM9663_REG_PLL_DIVOUT = 0x3E,
  OM9663_REG_LPCD_QMIN = 0x3F,
  OM9663_REG_LPCD_QMAX = 0x40,
  OM9663_REG_LPCD_IMIN = 0x41,
  OM9663_REG_LPCD_I_RESULT = 0x42,
  OM9663_REG_LPCD_Q_RESULT = 0x43,
  OM9663_REG_PADEN = 0x44,
  OM9663_REG_PADOUT = 0x45,
  OM9663_REG_PADIN = 0x46,
  OM9663_REG_SIGOUT = 0x47,
  OM9663_REG_VERSION = 0x7F
};

/*! See Table 7.10.2: Command Set */
enum om9663cmd {
  OM9663_CMD_IDLE = 0x00, /**< Cancels current command */
  OM9663_CMD_LPCD = 0x01, /**< Low power card detection */
  OM9663_CMD_LOADKEY = 0x02, /**< Reads a 6 byte MIFARE key and puts it into KEY BUFFER */
  OM9663_CMD_MFAUTHENT = 0x03, /**< Performs Mifare Classic authentication */
  OM9663_CMD_RECEIVE = 0x05,   /**< Activates the receive circuit */
  OM9663_CMD_TRANSMIT = 0x06,  /**< Transmits data from the FIFO buffer */
  OM9663_CMD_TRANSCEIVE = 0x07, /**< Transmits data from the FIFO buffer and automatically activates the receive buffer when finished */
  OM9663_CMD_WRITEE2 = 0x08, /**< Gets 1 byte from FIFO and writes to EEPROM */
  OM9663_CMD_WRITEE2PAGE = 0x09, /**< Gets up to 64 bytes from FIFO and writes to EEPROM */
  OM9663_CMD_READE2 = 0x0A, /**< Reads data from EEPROM and copies it into the FIFO buffer */
  OM9663_CMD_LOADREG = 0x0C, /**< Reads data from the internal EEPROM and initializes the OM9663 registers. EEPROM address needs to be within EEPROM sector 2 */
  OM9663_CMD_LOADPROTOCOL = 0x0D, /**< Reads data from the internal EEPROM and initializes the OM9663 registers needed for a protocol change. */
  OM9663_CMD_LOADKEYE2 = 0x0E, /**< Copies a key from EEPROM into the key buffer */
  OM9663_CMD_STOREKEYE2 = 0x0F, /**< Stores a MIFARE key (6 bytes) into EEPROM */
  OM9663_CMD_READRNR = 0x1C, /**< Copies bytes from the random number generator into the FIFO buffer until the FIFO is full */
  OM9663_CMD_SOFTRESET = 0x1F /**< SW resets the OM9663 */
};

/*! ISO14443 Commands (see ISO-14443-3) */
enum iso14443_cmd {
  ISO14443_CMD_REQA = 0x26,    /**< Request command. */
  ISO14443_CMD_WUPA = 0x52,    /**< Wakeup command. */
  ISO14443_CAS_LEVEL_1 = 0x93, /**< Anticollision cascade level 1. */
  ISO14443_CAS_LEVEL_2 = 0x95, /**< Anticollision cascade level 2. */
  ISO14443_CAS_LEVEL_3 = 0x97  /**< Anticollision cascade level 3. */
};

/*! Mifare Commands */
enum mifare_cmd {
  MIFARE_CMD_AUTH_A = 0x60,
  MIFARE_CMD_AUTH_B = 0x61,
  MIFARE_CMD_READ = 0x30,
  MIFARE_CMD_WRITE = 0xA0,
  MIFARE_CMD_TRANSFER = 0xB0,
  MIFARE_CMD_DECREMENT = 0xC0,
  MIFARE_CMD_INCREMENT = 0xC1,
  MIFARE_CMD_STORE = 0xC2,
  MIFARE_ULTRALIGHT_CMD_WRITE = 0xA2
};

/*! NTAG Commands */
enum ntag_cmd {
  NTAG_CMD_READ = 0x30,      /**> NTAG page read. */
  NTAG_CMD_WRITE = 0xA2,     /**< NTAG-specfiic 4 byte write. */
  NTAG_CMD_COMP_WRITE = 0xA0 /**< Mifare Classic 16-byte compat. write. */
};

/*! 'ComState' values for for the OM9663_REG_STATUS register (0x0B) */
enum om9663comstat {
  OM9663_COMSTAT_IDLE = 0b000,         /**< IDLE */
  OM9663_COMSTAT_TXWAIT = 0b001,       /**< TX Wait */
  OM9663_COMSTAT_TRANSMITTING = 0b011, /**< Transmitting */
  OM9663_COMSTAT_RXWAIT = 0b101,       /**< RX Wait */
  OM9663_COMSTAT_WAITFORDATA = 0b110,  /**< Waiting for DATA */
  OM9663_COMSTAT_RECEIVING = 0b111     /**< Receiving */
};

/*! Radio config modes */
enum om9663radiocfg {
  OM9663_RADIOCFG_ISO1443A_106 = 1, /**< ISO1443A 106 Mode */
  OM9663_LAST
};

/*! MFRC360 errors */
enum om9663errors {
  OM9663_ERROR_EEPROM = (1 << 7),   /**< EEPROM error. */
  OM9663_ERROR_FIFOWR = (1 << 6),   /**< FIFO write error. */
  OM9663_ERROR_FIFOOVL = (1 << 5),  /**< FIFO already full! */
  OM9663_ERROR_MINFRAME = (1 << 4), /**< Not enough data in frame. */
  OM9663_ERROR_NODATA = (1 << 3),   /**< FIFO empty! */
  OM9663_ERROR_COLLDET = (1 << 2),  /**< Collision detection, see RxColl. */
  OM9663_ERROR_PROT = (1 << 1),     /**< Protocol error. */
  OM9663_ERROR_INTEG = (1 << 0)     /**< Data integrity error. */
};

/*! OM9663 interrupt requests 0 */
enum om9663irq0 {
  OM9663IRQ0_SET = (1 << 7),        /**< Sets/Clears interrupt. */
  OM9663IRQ0_HIALERTIRQ = (1 << 6), /**< FIFO has reached top level. */
  OM9663IRQ0_LOALERTIRQ = (1 << 5), /**< FIFO has reached bottom level. */
  OM9663IRQ0_IDLEIRQ = (1 << 4),    /**< Command terminated by itself. */
  OM9663IRQ0_TXIRQ = (1 << 3),      /**< Data transmission complete */
  OM9663IRQ0_RXIRQ = (1 << 2),      /**< Receiver detected end of stream */
  OM9663IRQ0_ERRIRQ = (1 << 1),     /**< FifoWrErr, FiFoOvl, ProtErr, NoDataErr, IntegErr. */
  OM9663IRQ0_RXSOF = (1 << 0)       /**< RX start of frame detected. */
};

/*! OM9663 interrupt requests 1 */
enum om9663irq1 {
  OM9663IRQ1_SET = (1 << 7),       /**< Sets/Clears interrupt. */
  OM9663IRQ1_GLOBALIRQ = (1 << 6), /**< Set if an enabled IRQ occured */
  OM9663IRQ1_LPCDIRQ = (1 << 5),   /**< Card detected in low power mode */
  OM9663IRQ1_TIMER4IRQ = (1 << 4), /**< Timer 4 underflow */
  OM9663IRQ1_TIMER3IRQ = (1 << 3), /**< Timer 3 underflow */
  OM9663IRQ1_TIMER2IRQ = (1 << 2), /**< Timer 2 underflow */
  OM9663IRQ1_TIMER1IRQ = (1 << 1), /**< Timer 1 underflow */
  OM9663IRQ1_TIMER0IRQ = (1 << 0), /**< Timer 0 underflow */
};

/*! OM9663 crypto engine status */
enum om9663status {
  OM9663STATUS_CRYPTO1ON = (1 << 5) /**< Mifare Classic Crypto engine on */
};

/*!
 * @brief OM9663 I2C Address
 */
#define OM9663_I2C_ADDR (0x29)

/*!
 * @brief Different cases used for I2C, SPI, and SERIAL data transfer
 */
enum om9663_transport {
  OM9663_TRANSPORT_I2C = 0,
  OM9663_TRANSPORT_SPI = 1,
  OM9663_TRANSPORT_SERIAL = 2
};

/**
 * Driver for the Adafruit OM9663 RFID front-end.
 */
class Adafruit_OM9663 {
public:
  /**
   * Default I2C bus constructor
   *
   * @param i2c_addr      The I2C address to use (default value is empty)
   * @param pdown_pin     The power down pin number (required)/
   */
  Adafruit_OM9663(uint8_t i2c_addr, int8_t pdown_pin = -1);

  /**
   * Custom I2C bus constructor with user-defined I2C bus
   *
   * @param wireBus       The I2C bus to use
   * @param i2c_addr      The I2C address to use (default value is empty)
   * @param pdown_pin     The power down pin number (required)/
   */
//  Adafruit_OM9663(TwoWire *wireBus, uint8_t i2c_addr, int8_t pdown_pin = -1);
  Adafruit_OM9663(uint8_t wireBus, uint8_t i2c_addr, int8_t pdown_pin = -1);

  /**
   * HW SPI bus constructor
   *
   * @param transport     The transport to use when communicating with the IC
   * @param cs            The CS/Sel pin for HW SPI access.
   * @param pdown_pin     The power down pin number (required)/
   *
   * @note This instance of the constructor requires the 'transport'
   *       parameter to distinguish is from the default I2C version.
   */
//  Adafruit_OM9663(enum om9663_transport transport, int8_t cs, int8_t pdown_pin = -1);

  /**
   * SW serial bus constructor
   *
   * @param serial        The Serial instance to use
   * @param pdown_pin     The power down pin number (required)/
   */
//  Adafruit_OM9663(Stream *serial, int8_t pdown_pin = -1);

  /**
   * Initialises the IC and performs some simple system checks.
   *
   * @return True if init succeeded, otherwise false.
   */
  bool begin(void);

  /* FIFO helpers (see section 7.5) */
  /**
   * Returns the number of bytes current in the FIFO buffer.
   *
   * @return The number of bytes in the FIFO buffer.
   */
  int16_t readFIFOLen(void);

  /**
   * Reads data from the FIFO buffer.
   *
   * @param len       The number of bytes to read
   * @param buffer    The buffer to write data into.
   *
   * @return The actual number of bytes read from the FIFO buffer.
   */
  int16_t readFIFO(uint16_t len, uint8_t *buffer);

  /**
   * Write sdata into the FIFO buffer.
   *
   * @param len       The number of bytes to write.
   * @param buffer    The data to write into the FIFO buffer.
   *
   * @return The actual number of bytes written.
   */
  int16_t writeFIFO(uint16_t len, uint8_t *buffer);

  /**
   * Clears the contents of the FIFO buffer.
   */
  void clearFIFO(void);

  /* Command wrappers */
  /**
   * Sends an unparameterized command to the IC.
   *
   * @param command   The command register to send.
   */
  void writeCommand(byte command);

  /**
   * Sends a parametrized command to the IC.
   *
   * @param command   The command register to send.
   * @param paramlen  The number of parameter bytes.
   * @param params    The paramater values to send.
   */
  void writeCommand(byte command, uint8_t paramlen, uint8_t *params);

  /* Radio config. */
  /**
   * Configures the radio for the specified protocol.
   *
   * @param cfg   The radio config setup to use.
   *
   * @return True if succeeded, otherwise false.
   */
  bool configRadio(om9663radiocfg cfg);

  /* General helpers */
  /**
   * Returns the current 'comm status' of the IC's internal state machine.
   *
   * @return The 8-bit state ID.
   */
  uint8_t getComStatus(void);

  /**
   * Performs a soft-reset to put the IC into a known state.
   */
  void softReset(void);

  /* Generic ISO14443a commands (common to any supported card variety). */
  /**
   * Sends the REQA command, requesting an ISO14443A-106 tag.
   *
   * @return The ATQA value if a card was detected.
   */
  uint16_t iso14443aRequest(void);

  /**
   * Sends the WUPA wakeup command.
   *
   * @return The ATQA value if a card was detected.
   */
  uint16_t iso14443aWakeup(void);

  /**
   * Selects a detected ISO14443A card, retrieving the UID and SAK.
   *
   * @param uid   Pointer to the buffer where the uid should be written.
   * @param sak   Pointer to the placeholder for the SAK value.
   *
   * @return True if init succeeded, otherwise false.
   */
  uint8_t iso14443aSelect(uint8_t *uid, uint8_t *sak);

  /* Mifare commands. */
  /**
   * Loads the specified authentication keys on the IC.
   *
   * @param key   Pointer to the buffer containing the key values.
   */
  void mifareLoadKey(uint8_t *key);

  /**
   * Authenticates the selected card using the previously supplied key/
   *
   * @param key_type  Whether to use KEYA or KEYB for authentication.
   * @param blocknum  The block number to authenticate.
   * @param uid       The UID of the card to authenticate.
   *
   * @return True if init succeeded, otherwise false.
   */
  bool mifareAuth(uint8_t key_type, uint8_t blocknum, uint8_t *uid);

  /**
   * Reads the contents of the specified (and previously authenticated)
   * memory block.
   *
   * @param blocknum  The block number to read.
   * @param buf       The buffer the data should be written into.
   *
   * @return The number of bytes read.
   */
  uint16_t mifareReadBlock(uint8_t blocknum, uint8_t *buf);

  /**
   * Writes the supplied data to the previously authenticated
   * memory block.
   *
   * @param blocknum  The block number to read.
   * @param buf       The buffer holding the data to write.
   *
   * @return The number of bytes written.
   */
  uint16_t mifareWriteBlock(uint16_t blocknum, uint8_t *buf);

  /**
   * The default key for fresh Mifare cards.
   */
  uint8_t mifareKeyGlobal[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  /**
   * The default key for NDEF formatted cards.
   */
  uint8_t mifareKeyNDEF[6] = {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7};

  /* NTAG commands */
  /**
   * Reads the contents of the specified page.
   *
   * @param pagenum   The page number to read.
   * @param buf       The buffer the data should be written into.
   *
   * @return The number of bytes read.
   */
  uint16_t ntagReadPage(uint16_t pagenum, uint8_t *buf);

  /**
   * Writes the supplied content of the specified page.
   *
   * @param pagenum   The page number to write to.
   * @param buf       The data to write to the card.
   *
   * @return The number of bytes written.
   */
  uint16_t ntagWritePage(uint16_t pagenum, uint8_t *buf);

private:
  int8_t _pdown;
  uint8_t _i2c_addr;
//  TwoWire *_wire;
  int8_t  _wire_bus;
//  Stream *_serial;
  int8_t _cs;
  enum om9663_transport _transport;

  void write8(byte reg, byte value);
  void writeBuffer(byte reg, uint16_t len, uint8_t *buffer);
  byte read8(byte reg);

  void printError(enum om9663errors err);

  uint16_t iso14443aCommand(enum iso14443_cmd cmd);
};

#endif
