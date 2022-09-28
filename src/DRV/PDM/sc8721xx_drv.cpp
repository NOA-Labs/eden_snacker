#include "sc8721xx_drv.h"

void sc87xx_drv_init(void)
{
    //for PD output.
    pinMode(SC87XX_DRV_1_CSO_PIN, INPUT); 
    pinMode(SC87XX_DRV_1_CE_PIN, OUTPUT); 
    digitalWrite(SC87XX_DRV_1_CE_PIN, 1);
    
    pinMode(SC87XX_DRV_1_DIS_PIN, OUTPUT); 
    digitalWrite(SC87XX_DRV_1_DIS_PIN, 1);
    
    //for wireless charge.
    pinMode(SC87XX_DRV_2_CSO_PIN, INPUT); 
    pinMode(SC87XX_DRV_2_CE_PIN, OUTPUT); 
    digitalWrite(SC87XX_DRV_2_CE_PIN, 1);

    // sc87xx_drv_print_register();

    // reg_vout_set_lsb lsb;
    // lsb.reg = 0;
    // lsb._reg.fb_on = 1; //通过寄存器配置vout。
    // lsb._reg.fb_sel = 1;
    // lsb._reg.fb_dir = 0;
    // sc87xx_drv_set_vout(lsb, SC87XX_VOUT_20V);

    // reg_global_ctrl ctrl;
    // ctrl.reg = 0;
    // ctrl._reg.load = 1;

    // sc87xx_drv_set_global_ctrl(ctrl);

    // uint8_t cso = 255;
    // sc87xx_drv_set_cso(cso);
}

void sc87xx_drv_set_cso(uint8_t port, uint8_t cso)
{
    SC8721XX_IIC_WRITE(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_CSO, &cso);
}

void sc87xx_drv_set_slope_comp(uint8_t port, uint8_t comp)
{
    SC8721XX_IIC_WRITE(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_SLOPE_COMP, &comp);
}
/**
 * @brief 
 * 
 * @param set 
 * @param vout : mv, cal: OFFSET= [Reg03<7:0> x 4 + Reg04<1:0>] x 20mV
 */
void sc87xx_drv_set_vout(uint8_t port, reg_vout_set_lsb set, uint8_t msb, uint8_t lsb)
{
    uint8_t val[2] = {0};

    val[0] = msb & 0xff;
    val[1] = lsb;
    val[1] |= set.reg;

    // PRINTF("\n"
    //         "vout_set_lsb: %d\n"
    //         "fb_dir:       %d\n"
    //         "fb_on:        %d\n"
    //         "fb_sel:       %d\n"
    //         "msb:          %d\n"
    //         "lsb:          %d\n"
    //         "reg3:         %X\n"
    //         "reg4:         %X\n",
    //         set._reg.vout_set_lsb,
    //         set._reg.fb_dir,
    //         set._reg.fb_on,
    //         set._reg.fb_sel,
    //         msb,
    //         lsb,
    //         val[0],
    //         val[1]);
    SC8721XX_IIC_WRITE16(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_VOUT_SET_MSB, val, 2);
}
void sc87xx_drv_set_global_ctrl(uint8_t port, reg_global_ctrl ctrl)
{
    // PRINTF("\n"
    //         "load: %d\n"
    //         "dcdc: %d\n"
    //         "reg:  %d\n",
    //         ctrl._reg.load,
    //         ctrl._reg.dis_dcdc,
    //         ctrl.reg
    //         );
    SC8721XX_IIC_WRITE(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_GLOBAL_CTRL, &ctrl.reg);
}
void sc87xx_drv_set_sys(uint8_t port, reg_sys_set set)
{
    SC8721XX_IIC_WRITE(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_SYS_SET, &set.reg);
}
void sc87xx_drv_set_frequency(uint8_t port, uint8_t freq)
{
    SC8721XX_IIC_WRITE(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_FREQ_SET, &freq);
}
uint8_t sc87xx_drv_get_status1(uint8_t port)
{
    uint8_t status = 0;
    SC8721XX_IIC_READ(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_STATUS1, &status);
    return status;
}
uint8_t sc87xx_drv_get_status2(uint8_t port)
{
    uint8_t status = 0;
    SC8721XX_IIC_READ(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_STATUS2, &status);
    return status;
}

uint16_t sc87xx_drv_get_status(uint8_t port)
{
    uint8_t status[2] = {0};
    
    SC8721XX_IIC_READ16(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_STATUS1, status, 2);
    uint16_t sta = status[0];
    sta <<= 8;
    sta |= status[1];

    return sta;
}

void sc87xx_drv_set_ce_pin(uint8_t port, uint8_t level)
{
    const uint8_t pos[2] = {SC87XX_DRV_2_CE_PIN, SC87XX_DRV_1_CE_PIN};

    digitalWrite(pos[port % 2], level);
}

void sc87xx_drv_set_dis_pin(uint8_t port, uint8_t level)
{
    const uint8_t dis[2] = {0, SC87XX_DRV_1_DIS_PIN};

    digitalWrite(dis[port % 2], level);
}

void sc87xx_drv_print_register(uint8_t port)
{
    uint8_t reg[12] = {0};
    SC8721XX_IIC_READS(port, SC8721XX_IIC_DEV_ADDR, SC87XX_REG_CSO, reg, 10);

    PRINTF("\n"
            "reg1: %X\n"
            "reg2: %X\n"
            "reg3: %X\n"
            "reg4: %X\n"
            "reg5: %X\n"
            "reg6: %X\n"
            "reg7: %X\n"
            "reg8: %X\n"
            "reg9: %X\n"
            "reg10: %X\n", 
            reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7], reg[8], reg[9]);
}

void sc87xx_drv_set_output_vol(uint8_t port, uint8_t out)
{
    reg_vout_set_lsb lsb;
    lsb.reg = 0;
    lsb._reg.fb_on = 1; //通过寄存器配置vout。
    lsb._reg.fb_sel = 1;
    lsb._reg.fb_dir = 0;
    switch(out){
        case SC87XX_OUTPUT_TYPE_5V:
            sc87xx_drv_set_vout(port, lsb, SC87XX_VOUT_5V);
        break;
        case SC87XX_OUTPUT_TYPE_9V:
            sc87xx_drv_set_vout(port, lsb, SC87XX_VOUT_9V);
        break;
        case SC87XX_OUTPUT_TYPE_12V:
            sc87xx_drv_set_vout(port, lsb, SC87XX_VOUT_12V);
        break;
        case SC87XX_OUTPUT_TYPE_15V:
            sc87xx_drv_set_vout(port, lsb, SC87XX_VOUT_15V);
        break;
        case SC87XX_OUTPUT_TYPE_20V:
            sc87xx_drv_set_vout(port, lsb, SC87XX_VOUT_20V);
        break;
        default: return;
    }
    

    reg_global_ctrl ctrl;
    ctrl.reg = 0;
    ctrl._reg.load = 1;

    sc87xx_drv_set_global_ctrl(port, ctrl);
}
