/*
 * Final_Task.c
 *
 *  Created on: 2022锟斤拷6锟斤拷16锟斤拷
 *      Author: TomSawyer2
 */

#include "stdio.h"
#include "xgpio.h"
#include "xgpio_l.h"
#include "xil_io.h"
#include "xintc_l.h"
#include "xtmrctr_l.h"
#define SET_VALUE 100000 - 2

void Seg_TimerCounterHandler();
void BtnHandler();
void swHandler();
void My_ISR() __attribute__((interrupt_handler));                                                                                  //锟斤拷锟叫断凤拷锟斤拷锟斤拷锟�
char seg[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};                                                                    //每锟斤拷锟斤拷锟斤拷锟斤拷锟绞臼裁�,seg[7]锟斤拷锟斤拷锟揭边碉拷锟斤拷锟斤拷锟�
char pos[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};                                                                    //位锟斤拷,GPIO1_2
short number_seg_code[16] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e};      // 16锟斤拷锟狡革拷锟斤拷锟脚的讹拷锟斤拷:0~F
short numwithdot_seg_code[16] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0e};  // 16锟斤拷锟狡革拷锟斤拷锟斤拷(锟斤拷锟斤拷)锟侥讹拷锟斤拷:0~F
short switchvalue;
int btnvalue;
int i = 0;
// unsigned short x = 0;
int startPos = 4;

int main() {
  Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI_OFFSET, 0xffff);
  Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_TRI2_OFFSET, 0x0);
  Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_TRI_OFFSET, 0x1f);
  Xil_Out8(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI_OFFSET, 0x0);
  Xil_Out8(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_TRI2_OFFSET, 0x0);
  Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_IER_OFFSET, XGPIO_IR_CH1_MASK);
  Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_GIE_OFFSET, XGPIO_GIE_GINTR_ENABLE_MASK);
  Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_IER_OFFSET, XGPIO_IR_CH1_MASK);
  Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_GIE_OFFSET, XGPIO_GIE_GINTR_ENABLE_MASK);
  Xil_Out32(XPAR_INTC_0_BASEADDR + XIN_IER_OFFSET, XPAR_AXI_GPIO_2_IP2INTC_IRPT_MASK);
  Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET,
            Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET) & ~XTC_CSR_ENABLE_TMR_MASK);
  Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TLR_OFFSET, SET_VALUE);
  Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET,
            Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET) | XTC_CSR_LOAD_MASK);
  Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET,
            (Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET) & ~XTC_CSR_LOAD_MASK) | XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_ENABLE_INT_MASK | XIN_INT_HARDWARE_ENABLE_MASK);

  Xil_Out32(XPAR_INTC_0_BASEADDR + XIN_IER_OFFSET, XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK | XPAR_AXI_GPIO_2_IP2INTC_IRPT_MASK | XPAR_AXI_TIMER_0_INTERRUPT_MASK);  //锟斤拷锟叫断匡拷锟斤拷锟斤拷锟斤拷锟斤拷锟叫讹拷源使锟斤拷
  Xil_Out32(XPAR_INTC_0_BASEADDR + XIN_MER_OFFSET, XIN_INT_MASTER_ENABLE_MASK | XIN_INT_HARDWARE_ENABLE_MASK);
  microblaze_enable_interrupts();
  Xil_Out16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA2_OFFSET, Xil_In16(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET));
  return 0;
}

void My_ISR() {
  int status;
  status = Xil_In32(XPAR_INTC_0_BASEADDR + XIN_ISR_OFFSET);
  if ((status & XPAR_AXI_TIMER_0_INTERRUPT_MASK) == XPAR_AXI_TIMER_0_INTERRUPT_MASK)
    Seg_TimerCounterHandler();
  if ((status & XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK) == XPAR_AXI_GPIO_0_IP2INTC_IRPT_MASK)
    swHandler();
  if ((status & XPAR_AXI_GPIO_2_IP2INTC_IRPT_MASK) == XPAR_AXI_GPIO_2_IP2INTC_IRPT_MASK)
    BtnHandler();
  Xil_Out32(XPAR_INTC_0_BASEADDR + XIN_IAR_OFFSET, status);
}

void Seg_TimerCounterHandler() {
  Xil_Out32(XPAR_AXI_GPIO_1_BASEADDR + XGPIO_DATA2_OFFSET, seg[i]);
  i = (i + 1) % 8;
  Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET));
}

void swHandler() {
  switchvalue = Xil_In32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_DATA_OFFSET) & 0xffff;
  Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET, Xil_In32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET));
}

void BtnHandler() {
  unsigned int btn_code = Xil_In8(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_DATA_OFFSET);
  switch (btn_code) {
    /** C */
    case 0x1:
      xil_printf("C");
      for (int j = 0; j < startPos; j++) {
        seg[j] = 0xff;
      }
      seg[startPos % 8] = number_seg_code[(switchvalue >> 12) & 0x000f];
      seg[(startPos + 1) % 8] = number_seg_code[(switchvalue >> 8) & 0x000f];
      seg[(startPos + 2) % 8] = number_seg_code[(switchvalue >> 4) & 0x000f];
      seg[(startPos + 3) % 8] = number_seg_code[(switchvalue >> 0) & 0x000f];
      Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET, Xil_In32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET));
      break;
    /** L */
    case 0x4:
      xil_printf("L");
      if (startPos > 0) {
        startPos--;
      } else {
        startPos = 7;
      }
      for (int j = 0; j < startPos; j++) {
        seg[j] = 0xff;
      }
      int startPos2 = startPos + 4;
      for (int j = startPos2; j < 8; j++) {
        seg[j] = 0xff;
      }
      seg[startPos % 8] = number_seg_code[(switchvalue >> 12) & 0x000f];
      seg[(startPos + 1) % 8] = number_seg_code[(switchvalue >> 8) & 0x000f];
      seg[(startPos + 2) % 8] = number_seg_code[(switchvalue >> 4) & 0x000f];
      seg[(startPos + 3) % 8] = number_seg_code[(switchvalue >> 0) & 0x000f];
      Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET, Xil_In32(XPAR_AXI_GPIO_0_BASEADDR + XGPIO_ISR_OFFSET));
      break;
    default:
      break;
  }
  /** 写GPIO_2锟斤拷ISR锟斤拷锟斤拷锟斤拷锟接ξ伙拷卸锟阶刺� */
  Xil_Out32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET, Xil_In32(XPAR_AXI_GPIO_2_BASEADDR + XGPIO_ISR_OFFSET));
}
