/*
 * SPI_AD_IO.c
 *
 *  Created on: 2022年6月5日
 *      Author: 62728
 */
#include "xil_io.h"
#include "stdio.h"
#include "xgpio_l.h"
#include "xintc_l.h"
#include "xtmrctr_l.h"
#include "xspi_l.h"
#include "xgpio.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xparameters.h"
#include "xintc.h"
#include "xuartlite.h"
#include "xuartlite_l.h"

void My_ISR() __attribute__ ((interrupt_handler));
u16 volt, lastvolt;

int main() {
	/** SPI是主设备，CPOL = 1，CPHA = 0，自动方式，高位优先传送 */
	Xil_Out32(XPAR_AXI_QUAD_SPI_1_BASEADDR + XSP_CR_OFFSET, XSP_CR_ENABLE_MASK | XSP_CR_MASTER_MODE_MASK | XSP_CR_CLK_POLARITY_MASK);
	/** 设定SSR寄存器 */
	Xil_Out32(XPAR_AXI_QUAD_SPI_1_BASEADDR + XSP_SSR_OFFSET, 0Xfffffffe);
	/** 开放SPI发送寄存器空中断 */
	Xil_Out32(XPAR_AXI_QUAD_SPI_1_BASEADDR + XSP_IIER_OFFSET, XSP_INTR_TX_EMPTY_MASK);
	Xil_Out32(XPAR_AXI_QUAD_SPI_1_BASEADDR + XSP_DGIER_OFFSET, XSP_GINTR_ENABLE_MASK);
	/** 中断控制器intr0中断源使能 */
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET, XPAR_AXI_QUAD_SPI_1_IP2INTC_IRPT_MASK);
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET, XIN_INT_MASTER_ENABLE_MASK | XIN_INT_HARDWARE_ENABLE_MASK);
	/** 处理器中断使能 */
	microblaze_enable_interrupts();
	/** 自动传输，发送数据0 */
	Xil_Out16(XPAR_AXI_QUAD_SPI_1_BASEADDR + XSP_DTR_OFFSET, 0);
}

void My_ISR() {
	int temp;
	volt = Xil_In16(XPAR_AXI_QUAD_SPI_1_BASEADDR + XSP_DRR_OFFSET) & 0xfff;
	if (volt != lastvolt) {
		temp = volt * 3300 / 0xfff;
		xil_printf("Current Voltage is %d mv\n", temp);
		lastvolt = volt;
	}
	/** 清除SPI中断状态寄存器 */
	Xil_Out32(XPAR_AXI_QUAD_SPI_1_BASEADDR + XSP_IISR_OFFSET, Xil_In32(XPAR_AXI_QUAD_SPI_1_BASEADDR + XSP_IISR_OFFSET));
	/** 写INTC中断响应寄存器 */
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, Xil_In32(XPAR_AXI_INTC_0_BASEADDR + XIN_ISR_OFFSET));
	Xil_Out16(XPAR_AXI_QUAD_SPI_1_BASEADDR + XSP_DTR_OFFSET, 0);
}
