/*
 * lab3_2.c
 *
 *  Created on: 2022年5月20日
 *      Author: 62728
 */
//GPIO_2的CH1对应的button是中断源
#include "xil_io.h"
#include "stdio.h"
#include "xgpio_l.h"
#include "xintc_l.h"
#include "xtmrctr_l.h"
#define Sw_DATA 0x40000000 // Switch 数据寄存器地址
#define Sw_TRI 0x40000004 // Switch 控制寄存器地址
#define Sw_ISR 0x40000120 // Switch 中断状态寄存器地址
#define Led_DATA 0x40000008 // Leds 数据寄存器地址
#define Led_TRI 0x4000000C // Leds 控制寄存器地址
#define RESET_VALUE 100000

void BtnHandler();
//普通中断服务程序的子程序
void My_ISR() __attribute__ ((interrupt_handler));
//总的 普通中断服务程序

unsigned short csw1, csw2, r1, r2;

int main() {
    /*GPIO_0*/
    //GPIO:SW15~0  GPIO2:LED15~0
    Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET,0xffff);//设定16个开关为输入

    /*GPIO_2*/
    //GPIO:BTND,U,L,R,C  GPIO2:LD17(B\G\R),LD16(B\G\R)
    Xil_Out8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_TRI_OFFSET, 0x1f);//设定5个按键为输入
    /*此处的中断触发是按键，如果是sw之类的那就改为GPIO_0之类的中断输出*/
    Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_IER_OFFSET, XGPIO_IR_CH1_MASK);//CH1允许 中断，CH1对应的是按键，CH2是LD
    Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_GIE_OFFSET, XGPIO_GIE_GINTR_ENABLE_MASK);//允许 按键和LD所对应的GPIO IP核 GPIO_2 中断输出

    /*INTC*/
    //普通中断在中断服务程序中要写程序手动清IAR
    Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET,0x6);
    //初始化清除中断状态，写IAR寄存器
    //不需要写中断模式寄存器IMR
    Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IER_OFFSET,XPAR_AXI_GPIO_2_IP2INTC_IRPT_MASK | XPAR_AXI_TIMER_0_INTERRUPT_MASK);
    //写IER，开放中断控制器INTC对应的各个中断源输入引脚的中断，GPIO_2和TIMER_0是中断源，此处使能GPIO_2或TIMER_0所对应的输入引脚
    Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_MER_OFFSET,XIN_INT_MASTER_ENABLE_MASK | XIN_INT_HARDWARE_ENABLE_MASK);
    //写MER，开放中断控制器输出引脚的中断
    //无需填写中断控制器维护的中断向量表，跳转至中断向量表内的0x10处即可

    /*MicroBlaze*/
    microblaze_enable_interrupts();
    //microblaze中断开放
    return 0;
}

//在中断向量表内的0x10处的imm和brai分别记录了My_ISR()地址的高、低16位
void My_ISR() {
    int status;
    status=Xil_In32(XPAR_AXI_INTC_0_BASEADDR + XIN_ISR_OFFSET);//读取INTC的中断状态寄存器ISR
    if((status & XPAR_AXI_GPIO_2_IP2INTC_IRPT_MASK) == XPAR_AXI_GPIO_2_IP2INTC_IRPT_MASK) {//查询是否为GPIO_2产生的中断
    	BtnHandler();
    }
    Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + XIN_IAR_OFFSET, status);
    //普通中断，INTC需要将现在的中断状态ISR写到IAR寄存器中去，清除中断状态，而快速中断不用
}

//中断源为GPIO_2
void BtnHandler() {
    char button = Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET);
    short cur_sw = Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET);
    if (button == 0x10) { //BTND
    	r2 = csw1 * csw2;
    	Xil_Out16(Led_DATA, r2);
    } else if(button == 0x02) { //BTNU
    	r1 = csw1 + csw2;
    	Xil_Out16(Led_DATA, r1);
    } else if((button == 0x01)) { //BTNC
    	csw1 = cur_sw;
        Xil_Out16(Led_DATA, cur_sw);
    } else if((button == 0x08)) { //BTNR
    	csw2 = cur_sw;
        Xil_Out16(Led_DATA, cur_sw);
    }
    Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, Xil_In32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET));
    //无论是是快速中断还是普通中断，都需要清除GPIO_2的中断状态，写ISR寄存器存的的1(申请中断状态)到ISR寄存器中去，清除GPIO_2的申请中断状态
}
