#pragma once

#include "LIB/PUB/NOA_public.h"
#include "DRV/bsp_gpio_config.h"
#include <Arduino.h>

#ifdef __cplusplus
extern "C"{
#endif

#define PRINTF(format, args...)                  

#define SC8721XX_IIC_DEV_ADDR                                           0X62

#define SC8721XX_DEV_COUNT                              2

#define SC8721XX_IIC_DEV_ADDR                                           0X62

#define SC87XX_DRV_1_CE_PIN                             BSP_CFG_SC8721_1_CE_PIN     //for pd output
#define SC87XX_DRV_1_CSO_PIN                            BSP_CFG_SC8721_1_CSO_PIN
#define SC87XX_DRV_1_DIS_PIN                            BSP_CFG_SC8721_1_DIS_PIN 
#define SC87XX_DRV_2_CE_PIN                             BSP_CFG_SC8721_2_CE_PIN      //for wireless charge output(V4.2 12V)
#define SC87XX_DRV_2_CSO_PIN                            BSP_CFG_SC8721_2_CSO_PIN

#define SC8721XX_DRV_I2C_SCL_PIN                         BSP_CFG_NCA9545_SCL_PIN
#define SC8721XX_DRV_I2C_SDA_PIN                         BSP_CFG_NCA9545_SDA_PIN

#define SC8721XX_IIC_INIT()                                               
#define SC8721XX_IIC_WRITE(port, dev_addr, reg, val)                            NOA_PUB_I2C_SendBytes(port, dev_addr, reg, val, 1) 
#define SC8721XX_IIC_READ(port, dev_addr, reg, val)                             NOA_PUB_I2C_ReceiveBytes(port, dev_addr, reg, val, 1)
#define SC8721XX_IIC_READS(port, dev_addr, reg, val, len)                       NOA_PUB_I2C_ReceiveBytes(port, dev_addr, reg, val, len) 
#define SC8721XX_IIC_WRITE16(port, dev_addr, reg, val, length)                  NOA_PUB_I2C_SendBytes(port, dev_addr, reg, val, length) 
#define SC8721XX_IIC_READ16(port, dev_addr, reg, val, length)                   NOA_PUB_I2C_ReceiveBytes(port, dev_addr, reg, val, length)


#define SC87XX_REG_CSO                                          0X01

#define SC87XX_REG_SLOPE_COMP                                   0X02
typedef union{
struct{
uint8_t slope_comp: 2;
uint8_t reserved: 6;
}_reg;
uint8_t reg;
}reg_slope_comp;

enum sc87xx_comp{
SC87XX_SLOPE_COMP_DEFAULT = 0,
SC87XX_SLOPE_COMP_50MVA,
SC87XX_SLOPE_COMP_100MVA,
SC87XX_SLOPE_COMP_150MVA
};

enum sc87xx_output_type{
    SC87XX_OUTPUT_TYPE_IDLE = 0,
    SC87XX_OUTPUT_TYPE_5V,
    SC87XX_OUTPUT_TYPE_9V,
    SC87XX_OUTPUT_TYPE_12V,
    SC87XX_OUTPUT_TYPE_15V,
    SC87XX_OUTPUT_TYPE_20V
};

//OFFSET= [Reg03<7:0> x 4 + Reg04<1:0>] x 20mV
#define SC87XX_VOUT_5V                  0,0
#define SC87XX_VOUT_9V                  50,0
#define SC87XX_VOUT_12V                 87,2
#define SC87XX_VOUT_15V                 125,0
#define SC87XX_VOUT_20V                 187,2

#define SC87XX_REG_VOUT_SET_MSB                                 0X03

#define SC87XX_REG_VOUT_SET_LSB                                 0X04
typedef union{
struct{
uint8_t vout_set_lsb: 2;
uint8_t fb_dir: 1;
uint8_t fb_on: 1;
uint8_t fb_sel: 1;
uint8_t reserved: 3;
}_reg;
uint8_t reg;
}reg_vout_set_lsb;

#define SC87XX_REG_GLOBAL_CTRL                                  0X05
typedef union{
    struct{
        uint8_t reserved0:1;
        uint8_t load: 1;
        uint8_t dis_dcdc:1;
        uint8_t reserved1:5;
    }_reg;
    uint8_t reg;
}reg_global_ctrl;

#define SC87XX_REG_SYS_SET                                      0X06
typedef union{
    struct{
        uint8_t reserved0:4;
        uint8_t en_vinreg:1;
        uint8_t reserved1:1;
        uint8_t ext_dt:1;
        uint8_t en_pwm:1;
    }_reg;
    uint8_t reg;
}reg_sys_set;

#define SC87XX_REG_RESERVED                                     0X07

#define SC87XX_REG_FREQ_SET                                     0X08
typedef union{
    struct{
        uint8_t freq_set:2;
        uint8_t reserved:6;
    }_reg;
    uint8_t reg;
}reg_freq_set;

enum sc87xx_freq_sel{
    SC87XX_FREQ_260K = 0,
    SC87XX_FREQ_500K,
    SC87XX_FREQ_720K,
    SC87XX_FREQ_920K
};

#define SC87XX_REG_STATUS1                                      0X09
typedef union{
    struct{
        uint8_t ocp:1;
        uint8_t reserved0:1;
        uint8_t reserved1:1;
        uint8_t thd:1;
        uint8_t reserved2:1;
        uint8_t reserved3:1;
        uint8_t vout_vout_h:1;
        uint8_t vout_short:1;
    }_reg;
    uint8_t reg;
}reg_status1;

#define SC87XX_REG_STATUS2                                      0X0A
typedef union{
    struct{
        uint8_t reserved0:1;
        uint8_t ibus_flag:1;
        uint8_t vinreg_flag:1;
        uint8_t reserved1:1;
        uint8_t reserved2:1;
        uint8_t reserved3:1;
        uint8_t reserved4:1;
        uint8_t vinovp:1;
    }_reg;
    uint8_t reg;
}reg_status2;

void sc87xx_drv_init(void);
void sc87xx_drv_set_cso(uint8_t port, uint8_t cso);
void sc87xx_drv_set_slope_comp(uint8_t port, uint8_t comp);
void sc87xx_drv_set_vout(uint8_t port, reg_vout_set_lsb set, uint8_t msb, uint8_t lsb);
void sc87xx_drv_set_global_ctrl(uint8_t port, reg_global_ctrl ctrl);
void sc87xx_drv_set_sys(uint8_t port, reg_sys_set set);
void sc87xx_drv_set_frequency(uint8_t port, uint8_t freq);
uint8_t sc87xx_drv_get_status1(uint8_t port);
uint8_t sc87xx_drv_get_status2(uint8_t port);
uint16_t sc87xx_drv_get_status(uint8_t port);
void sc87xx_drv_set_ce_pin(uint8_t port, uint8_t level);
void sc87xx_drv_set_dis_pin(uint8_t port, uint8_t level);
void sc87xx_drv_print_register(uint8_t port);
void sc87xx_drv_set_output_vol(uint8_t port, uint8_t out);
#ifdef __cplusplus
}
#endif