/*
 * lab_3.c
 *
 *  Created on: 2022年5月19日
 *      Author: 62728
 */
#include "stdio.h"
#include "xil_io.h"
#include "xgpio.h"
#define Sw_DATA 0x40000000 // Switch 数据寄存器地址
#define Sw_TRI 0x40000004 // Switch 控制寄存器地址
#define Sw_ISR 0x40000120 // Switch 中断状态寄存器地址
#define Led_DATA 0x40000008 // Leds 数据寄存器地址
#define Led_TRI 0x4000000C // Leds 控制寄存器地址
int main() {
  short button;
  unsigned short csw1, csw2, r1, r2;
  short int sw, swFlag;
  Xil_Out16(Sw_TRI,0xffff); // Sw is used as input;
  Xil_Out16(Led_TRI,0x0000); // Led is used as output;

  while(1) {
	  swFlag = Xil_In16(Sw_ISR);
	  if (swFlag & 0x0001) {
		  button = Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET) & 0x1f;
	      if (button == 0x1) {
	        csw1 = Xil_In16(Sw_DATA);
	        Xil_Out16(Led_DATA, csw1);
	      }

	      if(button == 0x8) {
	    	  csw2 = Xil_In16(Sw_DATA);
	    	  Xil_Out16(Led_DATA, csw2);
	      }

	      if(button == 0x2) {
			  r1 = csw1 + csw2;
			  Xil_Out16(Led_DATA, r1);
	      }

	      if(button == 16) {
	        r2 = csw1 * csw2;
	        Xil_Out16(Led_DATA, r2);
	      }
	  }
  }
}
