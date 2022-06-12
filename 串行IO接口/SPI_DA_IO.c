/*
 * SPI_DA_IO.c
 *
 *  Created on: 2022年6月6日
 *      Author: 62728
 */
#include<xil_io.h>
#include<stdio.h>
#include<xgpio_l.h>
#include<xtmrctr_l.h>
#include<xintc_l.h>
#include<xspi_l.h>

void SwitchHandler();
void TimerHandler();
void My_ISR() __attribute__((interrupt_handler));
unsigned int RESET_VALUE;
u16 volt;

int main() {
	RESET_VALUE = 1;
	//写TCSR,停止计数器
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET) & ~XTC_CSR_ENABLE_TMR_MASK);
	//写TLR,预置计数初值
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TLR_OFFSET, RESET_VALUE);
	//装载计数初值
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET) | XTC_CSR_LOAD_MASK);
	//开始计时运行,自动装载,允许中断,减计数
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, (Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET) & ~XTC_CSR_LOAD_MASK)
	 | XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_ENABLE_INT_MASK | XTC_CSR_DOWN_COUNT_MASK);

	//设定SPI为主设备,CPOL=1,CPHA=0,自动方式,高位优先传送
	Xil_Out32(XPAR_AXI_QUAD_SPI_0_BASEADDR + XSP_CR_OFFSET, XSP_CR_ENABLE_MASK | XSP_CR_MASTER_MODE_MASK | XSP_CR_CLK_POLARITY_MASK);
	//设定SSR寄存器
	Xil_Out32(XPAR_AXI_QUAD_SPI_0_BASEADDR + XSP_SSR_OFFSET, 0xfffffffe);
	//开放SPI发送寄存器空中断
	Xil_Out32(XPAR_AXI_QUAD_SPI_0_BASEADDR + XSP_IIER_OFFSET, XSP_INTR_TX_EMPTY_MASK);
	Xil_Out32(XPAR_AXI_QUAD_SPI_0_BASEADDR + XSP_DGIER_OFFSET, XSP_GINTR_ENABLE_MASK);

	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0Xffff); //开关读入中断
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_IER_OFFSET, XGPIO_IR_CH1_MASK); //GPIO_0通道一允许中断
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_GIE_OFFSET, XGPIO_GIE_GINTR_ENABLE_MASK); //允许GPIO_0中断输出
	//设置中断使能，需要开关和时钟两个
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET, XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK | XPAR_AXI_TIMER_0_INTERRUPT_MASK);
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET, XIN_INT_MASTER_ENABLE_MASK | XIN_INT_HARDWARE_ENABLE_MASK);

	microblaze_enable_interrupts();
	return 0;
}

void My_ISR() {
	int status;
	status = Xil_In32(XPAR_AXI_INTC_0_BASEADDR + XIN_ISR_OFFSET); //读ISR
	if((status & XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK) == XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK) {
		/** 开关中断 */
		SwitchHandler();
	} else if((status & XPAR_AXI_TIMER_0_INTERRUPT_MASK) == XPAR_AXI_TIMER_0_INTERRUPT_MASK) {
		/** 时钟中断 */
		TimerHandler();
	}
	Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, status);	//写IAR
}

void TimerHandler() {
	Xil_Out16(XPAR_AXI_QUAD_SPI_0_BASEADDR + XSP_DTR_OFFSET, volt & 0xfff);
	volt += 150;
	if(volt == 0xfff) {
		/** 相当于学习通里面volt&0xfff，到指定大小就置零 */
		volt = 0;
	}
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET));//清除中断
}

void SwitchHandler() {
	RESET_VALUE = (Xil_In32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff); //读取开关状态
	xil_printf("%d\n", RESET_VALUE);
	Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TLR_OFFSET, RESET_VALUE);
	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET, Xil_In32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET));//清除中断
}
