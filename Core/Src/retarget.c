/**
  ******************************************************************************
  * @file           : retarget.c
  * @brief          : ����ض���
  * @author         : yz.ouyang
  * @date           : 2021/10/26
  ******************************************************************************
  * @attention
  *
  * Copyright(C) ���ո�����Ϣ�������޹�˾ 2021-2031
  * All rights reserved.
  *
  ******************************************************************************
  */
#include "main.h"
#include <stdio.h>
#include "usart.h"

#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
  int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
  x = x;
}

void _ttywrch(int ch)
{
  ch = ch;
}

//�ض���fputc����
int fputc(int ch, FILE *f)
{

  while(( DEBUG_UART.Instance->SR & 0X40 ) == 0); /* �ȴ����ͽ��� */

  DEBUG_UART.Instance->DR = (uint8_t)ch;

  return ch;
}

/************************ (C) COPYRIGHT GauTure *****END OF FILE****/
