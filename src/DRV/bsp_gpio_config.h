#pragma once

#define BSP_CFG_IIC0_SDA_PIN                                    26          //for pd snk, sc8721 output(wireless charge output). nfc
#define BSP_CFG_IIC0_SCL_PIN                                    27
#define BSP_CFG_IIC1_SDA_PIN                                    21         //for pd src, sc8721 output(pd) 
#define BSP_CFG_IIC1_SCL_PIN                                    22
#define BSP_CFG_IIC_FREQ_HZ                                     400000UL 

#define BSP_CFG_SC8721_1_CE_PIN                                 12          //for pd output
#define BSP_CFG_SC8721_1_CSO_PIN                                36 
#define BSP_CFG_SC8721_1_DIS_PIN                                14 
#define BSP_CFG_SC8721_2_CE_PIN                                 13          //for wireless charge output(V4.2 12V)
#define BSP_CFG_SC8721_2_CSO_PIN                                39

#define BSP_CFG_LED_PIN                                         18          
#define BSP_CFG_REG_CTL_PIN                                     19          //for ws2812b reg led

#define BSP_CFG_PD_SNK_INT_PIN                                  32
#define BSP_CFG_PD_SRC_INT_PIN                                  23

#define BSP_CFG_NFC_PDOWN_PIN                                   4
#define BSP_CFG_NFC_IRQ_PIN                                     5
#define BSP_CFG_NFC_W_PIN                                       15
