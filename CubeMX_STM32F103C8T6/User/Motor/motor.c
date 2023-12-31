/*
 * motor.c
 *
 *  Created on: Dec 30, 2023
 *      Author: YellowWenJie
 */

#include "motor.h"
#include "tim.h"
#include <stdio.h>

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwmb(int motor_right) // 赋值给PWM寄存器
{
  int pwmb_abs;
  if (motor_right > 0)
    Motor_Right(1);
  else
    Motor_Right(0);
  pwmb_abs = myabs(motor_right);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwmb_abs);
}
void Set_Pwma(int motor_left) // 赋值给PWM寄存器
{
  int pwma_abs;
  if (motor_left > 0)
    Motor_Left(1);
  else
    Motor_Left(0);
  pwma_abs = myabs(motor_left);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwma_abs);
}

void Motor_Right()
{
  HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET); // 即01，正转
}
void Motor_Left()
{
  HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET); // 即01，反转
}

/*
 *	函数功能：取绝对值
 *	入口参数：int
 *	返回值：无 unsingned int
 */
int myabs(int num)
{
  int temp;
  if (num < 0)
    temp = -num;
  else
    temp = num;
  return temp;
}

//! ********************************************************************************* //
//* 停止

typedef enum
{
  Low = 2800,
  Medium = 5600,
  High = 8400,
  Stop = 0
} Speed;
//* 速度
Speed Motor_Speed(Speed speed)
{
  if (speed == Low)
  {
    Set_Pwmb(Low);
    Set_Pwma(Low);
  }
  else if (speed == Medium)
  {
    Set_Pwmb(Medium);
    Set_Pwma(Medium);
  }
  else if (speed == High)
  {
    Set_Pwmb(High);
    Set_Pwma(High);
  }
  else if (speed == Stop)
  {
    Set_Pwmb(Stop);
    Set_Pwma(Stop);
  }
  return speed;
}

typedef enum
{
  Left,
  Right,
} Orientations;
//* 转向
void Motor_Redirect(Orientations orientations)
{
}

//* 控制指令
typedef enum
{
  Containment_Speed = '1',
  Containment_Speed_Buf = 0,

  Speed_Buf = 1,
  Speed_Stop = '0',
  Speed_Low = '1',
  Speed_Medium = '2',
  Speed_High = '3',

} Directives;
//* bluetooth 控制（USART2）
void Bluetooth_Containment()
{

  //* 速度控制
  // 1
  if (USART2_Rx_Buf[Containment_Speed_Buf] == Containment_Speed)
  {
    // 1, 0
    if (USART2_Rx_Buf[Speed_Buf] == Speed_Stop)
    {
      Motor_Speed(Stop);
    }
    // 1, 1
    if (USART2_Rx_Buf[Speed_Buf] == Speed_Low)
    {
      Motor_Speed(Low);
    }
    // 1, 2

    if (USART2_Rx_Buf[Speed_Buf] == Speed_Medium)
    {
      Motor_Speed(Medium);
    }
    // 1, 3

    if (USART2_Rx_Buf[Speed_Buf] == Speed_High)
    {
      Motor_Speed(High);
    }

    USART2_Rx_STA = 0;
  }
}

// 控制模式
typedef enum
{
  Bluetooth,
} Containment_Model;
void Containment(Containment_Model model)
{
  if (model == Bluetooth)
  {
    /* code */
  }
}