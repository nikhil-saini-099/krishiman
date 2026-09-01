#ifndef I2C_H
#define I2C_H
#include "stdint.h"
#include"esp32s3.h"



#define IO_MUX_MCU_SEL_S   12
#define IO_MUX_MCU_SEL_M   (0x7 << IO_MUX_MCU_SEL_S)
#define IO_MUX_FUN_IE_M    (1 << 9)
#define IO_MUX_FUN_WPU_M   (1 << 8)
#define IO_MUX_FUN_WPD_M   (1 << 7)
#define GPIO_PIN_PAD_DRIVER_M (1 << 2)

#define I2CEXT0_SCL_IN_IDX   89
#define I2CEXT0_SCL_OUT_IDX  89
#define I2CEXT0_SDA_IN_IDX   90
#define I2CEXT0_SDA_OUT_IDX  90

#define GPIO_SIG_IN_SEL_M (1 << 7)



// some values 
// #define N 4 // no of bytes to be recieve
#define SLAVE_ADDR  0X040u
#define temp_comnd 0xE3
#define humid_comnd 0xE5 
typedef enum 
{
      TEMP ,
      HUMID
} HTU_mode ;

// BIT map for I2C registers
#define SYSTEM_I2C_EXT0_CLK_EN (1<< 7)
// I2C_CTR_REG
#define I2C_SDA_FORCE_OUT_shift 0
#define I2C_SCL_FORCE_OUT_shift 1
#define I2C_SAMPLE_SCL_LEVEL (1 << 2)
#define I2C_RX_FULL_ACK_LEVEL (1 << 3)
#define I2C_MS_MODE (1 << 4)
#define I2C_TRANS_START (1 << 5)
#define I2C_TX_LSB_FIRST (1 << 6)
#define I2C_RX_LSB_FIRST (1 << 7)
#define I2C_CLK_EN (1 << 8)
#define I2C_ARBITRATION_EN (1 << 9)
#define I2C_FSM_RST (1 << 10)
#define I2C_CONF_UPGATE (1 << 11)
#define I2C_SLV_TX_AUTO_START_EN (1 << 12)
#define I2C_ADDR_10BIT_RW_CHECK_EN (1 << 13)
#define I2C_ADDR_BROADCASTING_EN (1 << 14)
//  bit map I2C_SLAVE_ADDR_REG_off
#define I2C_SLAVE_ADDR_shift 0
#define I2C_ADDR_10BIT_EN (1 << 31)
// I2C_TO_REG
#define I2C_TIME_OUT_EN (1 << 4)
#define I2C_TIME_OUT_VALUE_shift 0
// I2C_FIFO_CONF_REG_off
#define I2C_RXFIFO_WM_THRHD_SHIFT  0
#define I2C_TXFIFO_WM_THRHD_SHIFT  5
#define I2C_NONFIFO_EN (1 << 10)
#define I2C_FIFO_ADDR_CFG_EN (1 << 11)
#define I2C_RX_FIFO_RST (1 << 12)
#define I2C_TX_FIFO_RST (1 << 13)
#define I2C_FIFO_PRT_EN (1 << 14)

// I2C_CLK_CONF_REG
#define I2C_SCLK_DIV_NUM_shift 0
#define I2C_SCLK_DIV_A_shift 8
#define I2C_SCLK_DIV_B_shift 14
#define I2C_SCLK_SEL_shift 20
#define I2C_SCLK_ACTIVE_shift 21
// clk values 
#define I2C_SCL_LOW_PERIOD_val       190
#define I2C_SCL_HIGH_PERIOD_val     190
#define I2C_SCL_WAIT_HIGH_PERIOD_val (39<< 9)
#define I2C_SDA_HOLD_TIME_val        9
#define I2C_SDA_SAMPLE_TIME_val      100
//  I2C_SCL_STRETCH_CONF_REG 
// #define I2C_STRETCH_PROTECT_NUM
#define I2C_SLAVE_SCL_STRETCH_EN (1<<10)
// #define I2C_SLAVE_SCL_STRETCH_CLR 
// #define I2C_SLAVE_BYTE_ACK_CTL_EN 
// #define I2C_SLAVE_BYT

// I2C_SR_REG 
#define I2C_RESP_REC_mask (1 << 0)
#define I2C_SLAVE_RW_shift
#define I2C_SLAVE_RW_mask (1 << I2C_SLAVE_RW_BIT)
#define I2C_ARB_LOST_shift 3
#define I2C_ARB_LOST_MASK (1<<I2C_ARB_LOST_bit)
#define I2C_BUS_BUSY_shift 4
#define I2C_BUS_BUSY_mask (1<<I2C_BUS_BUSY_bit)
#define I2C_SLAVE_ADDRESSED_shift 5
#define I2C_SLAVE_ADDRESSED_mask (1<<5)
#define I2C_RXFIFO_CNT_shift 8
#define I2C_RXFIFO_CNT_mask (0x3f<< 8)
#define I2C_STRETCH_CAUSE_shift 14 
#define I2C_STRETCH_CAUSE_mask (3<<4)
#define I2C_TXFIFO_CNT_shift 16 
#define I2C_TXFIFO_CNT_mask (0x3f<<16)
#define I2C_SCL_MAIN_STATE_LAST_shift 24 
#define I2C_SCL_MAIN_STATE_LAST_mask ((1<<24))
#define I2C_SCL_STATE_LAST_shift 26 
#define I2C_SCL_STATE_LAST_mask (1<<26 )
// I2C_FIFO_ST_REG_off 
#define I2C_RXFIFO_RADDR_shift 0
#define I2C_RXFIFO_RADDR_mask (0x1f <<0)
#define I2C_RXFIFO_WADDR_shift 5 
#define I2C_RXFIFO_WADDR_mask (0xf<<5)
#define I2C_TXFIFO_RADD_shift 10 
#define I2C_TXFIFO_RADD_mask (0x1f <<10)
#define I2C_TXFIFO_WADDR_shift 15 
#define I2C_TXFIFO_WADDR_mask (0x1f<< 15)
#define I2C_SLAVE_RW_POINT_shift 22
#define I2C_SLAVE_RW_POINT_mask (0xff <<22)
//I2C_DATA_REG_of
#define I2C_FIFO_RDATA_shift 0 
#define I2C_FIFO_RDATA_mask (0xffU <<0)
// I2C_INT_RAW_REG 
#define I2C_RXFIFO_WM_INT_RAW_shift  0
#define I2C_RXFIFO_WM_INT_RAW_mask (1<<0)
// command register 
#define WRITE (1<<11) 
#define READ (3<<11) 
#define RSTART (6<<11) 
#define STOP (2<<11) 
#define ack_value_shift 10
#define ack_exp_shift 9
#define ack_exp_en_shift 8
uint16_t  I2C_RX_RAW(HTU_mode MODE) ;
void TEMP_COMND_SET() ;
void HUMID_COMND_SET() ;
void pin_config_I2C(void);
void I2C_MASTER_INIT(void);

#endif