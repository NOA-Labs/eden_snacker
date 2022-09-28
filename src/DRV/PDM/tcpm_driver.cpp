/*
 * tcpm_driver.c
 *
 * Created: 11/11/2017 18:42:26
 *  Author: jason
 */ 

#include "tcpm_driver.h"
#include "Arduino.h"
#include <Wire.h>

extern const struct tcpc_config_t tcpc_config[CONFIG_USB_PD_PORT_COUNT];

extern "C" {
  void WirebeginTransmission(int bus, int addr) {
    if (bus == 0) {
      Wire.beginTransmission(addr);
    } else {
      Wire1.beginTransmission(addr);
    }
  }

  void Wirewrite(int bus, int value) {
    if (bus == 0) {
      Wire.write(value);
    } else {
      Wire1.write(value);
    }
  }
  
  void WireendTransmission(int bus, int stopBit) {
    if (bus == 0) {
      Wire.endTransmission(stopBit);
    } else {
      Wire1.endTransmission(stopBit);
    }
  }

  void WirerequestFrom(int bus, int addr, int quantity, int stopBit) {
    if (bus == 0) {
      Wire.requestFrom(addr, quantity, stopBit);
    } else {
      Wire1.requestFrom(addr, quantity, stopBit);
    }
  }

  int Wireread(int bus) {
    if (bus == 0) {
      return Wire.read();
    } else {
      return Wire1.read();
    }
  }
}

/* I2C wrapper functions - get I2C port / slave addr from config struct. */
int tcpc_write(int port, int reg, int val) {
#ifdef NOA_PD_SNACKER
  if(port < 0 || port > 1) {  // support 0 - 1 port only
    return -1;
  }
#else
  if(port < 0 || port > 3) {  // support 0 - 3 port only
    return -1;
  }
#endif
  WirebeginTransmission(tcpc_config[port].i2c_host_port, tcpc_config[port].i2c_slave_addr);
  Wirewrite(tcpc_config[port].i2c_host_port, reg & 0xFF);
  Wirewrite(tcpc_config[port].i2c_host_port, val & 0xFF);
  WireendTransmission(tcpc_config[port].i2c_host_port, true);

  return 0;
}

int tcpc_write16(int port, int reg, int val) {
#ifdef NOA_PD_SNACKER
  if(port < 0 || port > 1) {  // support 0 - 1 port only
    return -1;
  }
#else
  if(port < 0 || port > 3) {  // support 0 - 3 port only
    return -1;
  }
#endif
  WirebeginTransmission(tcpc_config[port].i2c_host_port, tcpc_config[port].i2c_slave_addr);
  Wirewrite(tcpc_config[port].i2c_host_port, reg & 0xFF);
  Wirewrite(tcpc_config[port].i2c_host_port, val & 0xFF);
  Wirewrite(tcpc_config[port].i2c_host_port, (val >> 8) & 0xFF);
  WireendTransmission(tcpc_config[port].i2c_host_port, true);

  return 0;
}

int tcpc_read(int port, int reg, int *val) {
#ifdef NOA_PD_SNACKER
  if(port < 0 || port > 1) {  // support 0 - 1 port only
    return -1;
  }
#else
  if(port < 0 || port > 3) {  // support 0 - 3 port only
    return -1;
  }
#endif
  WirebeginTransmission(tcpc_config[port].i2c_host_port, tcpc_config[port].i2c_slave_addr);
  Wirewrite(tcpc_config[port].i2c_host_port, reg & 0xFF);
  WireendTransmission(tcpc_config[port].i2c_host_port, false);
  WirerequestFrom(tcpc_config[port].i2c_host_port, tcpc_config[port].i2c_slave_addr, 1, true);
  *val = Wireread(tcpc_config[port].i2c_host_port);

  return 0;
}

int tcpc_read16(int port, int reg, int *val) {
#ifdef NOA_PD_SNACKER
  if(port < 0 || port > 1) {  // support 0 - 1 port only
    return -1;
  }
#else
  if(port < 0 || port > 3) {  // support 0 - 3 port only
    return -1;
  }
#endif
  WirebeginTransmission(tcpc_config[port].i2c_host_port, tcpc_config[port].i2c_slave_addr);
  Wirewrite(tcpc_config[port].i2c_host_port, reg & 0xFF);
  WireendTransmission(tcpc_config[port].i2c_host_port, false);
  WirerequestFrom(tcpc_config[port].i2c_host_port, tcpc_config[port].i2c_slave_addr, 1, true);
  *val  = Wireread(tcpc_config[port].i2c_host_port);
  *val |= (Wireread(tcpc_config[port].i2c_host_port) << 8);

  return 0;
}

int tcpc_xfer(int port, const uint8_t *out, int out_size, uint8_t *in, int in_size, int flags) {
#ifdef NOA_PD_SNACKER
  if(port < 0 || port > 1) {  // support 0 - 1 port only
    return -1;
  }
#else
  if(port < 0 || port > 3) {  // support 0 - 3 port only
    return -1;
  }
#endif

  WirebeginTransmission(tcpc_config[port].i2c_host_port, tcpc_config[port].i2c_slave_addr);
  for (; out_size>0; out_size--) {
    Wirewrite(tcpc_config[port].i2c_host_port, *out);
    out++;
  }
  if (in_size) {
    WireendTransmission(tcpc_config[port].i2c_host_port, false);
    WirerequestFrom(tcpc_config[port].i2c_host_port, tcpc_config[port].i2c_slave_addr, (int)in_size, (flags & I2C_XFER_STOP));
    for (; in_size>0; in_size--) {
      *in = Wireread(tcpc_config[port].i2c_host_port);
      in++;
    }
  } else {
    WireendTransmission(tcpc_config[port].i2c_host_port, flags & I2C_XFER_STOP);
  }
  return 0;
}
