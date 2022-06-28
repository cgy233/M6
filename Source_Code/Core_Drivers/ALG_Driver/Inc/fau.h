/*
  ******************************************************************************
  * @file    FAU.h
  * @author  AisinoChip Firmware Team
  * @version V1.0.0
  * @date    2021
  * @brief   Header file of FAU  module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 AisinoChip.
  * All rights reserved.
  ******************************************************************************
*/
#ifndef __FAU_H_
#define __FAU_H_

#include "ACM32Fxx_HAL.h"

#define RANGE 2147483648U  //2^31
#define CORDIC_F_31  0xD2C90A46 // CORDIC gain F

/**************************************************************************
* Function Name  : cordic_sin_cos( precision 1 )
* Description    : calculate the sin & cos value of the input angle
* Input          : - angle_para   : input angle data in radians, divided by ¦Ð[range[-1,1],Q31 format];
* Output		     : - * cos_data   : the cos value of the input angle[range[-1,1],Q31 format]
                   - * sin_data   : the sin value of the input angle[range[-1,1],Q31 format]
* Return         : None
**************************************************************************/
void cordic_cos_sin_1(int angle_para, int* cos_data, int* sin_data);
/**************************************************************************
* Function Name  : cordic_sin_cos( precision 2 )
* Description    : calculate the sin & cos value of the input angle
* Input          : - angle_para   : input angle data in radians, divided by ¦Ð[range[-1,1],Q31 format];
* Output		     : - * cos_data   : the cos value of the input angle[range[-1,1],Q31 format]
                   - * sin_data   : the sin value of the input angle[range[-1,1],Q31 format]
* Return         : None
**************************************************************************/
void cordic_cos_sin_2(int angle_para, int* cos_data, int* sin_data);
/**************************************************************************
* Function Name  : cordic_sin_cos( precision 3 )
* Description    : calculate the sin & cos value of the input angle
* Input          : - angle_para   : input angle data in radians, divided by ¦Ð[range[-1,1],Q31 format];
* Output		     : - * cos_data   : the cos value of the input angle[range[-1,1],Q31 format]
                   - * sin_data   : the sin value of the input angle[range[-1,1],Q31 format]
* Return         : None
**************************************************************************/
void cordic_cos_sin_3(int angle_para, int* cos_data, int* sin_data);
/**************************************************************************
* Function Name  : cordic_sin_cos( precision 4 )
* Description    : calculate the sin & cos value of the input angle
* Input          : - angle_para   : input angle data in radians, divided by ¦Ð[range[-1,1],Q31 format];
* Output		     : - * cos_data   : the cos value of the input angle[range[-1,1],Q31 format]
                   - * sin_data   : the sin value of the input angle[range[-1,1],Q31 format]
* Return         : None
**************************************************************************/
void cordic_cos_sin_4(int angle_para, int* cos_data, int* sin_data);
/**************************************************************************
* Function Name  : cordic_sin_cos( precision 5 )
* Description    : calculate the sin & cos value of the input angle
* Input          : - angle_para   : input angle data in radians, divided by ¦Ð[range[-1,1],Q31 format];
* Output		     : - * cos_data   : the cos value of the input angle[range[-1,1],Q31 format]
                   - * sin_data   : the sin value of the input angle[range[-1,1],Q31 format]
* Return         : None
**************************************************************************/
void cordic_cos_sin_5(int angle_para, int* cos_data, int* sin_data);
/**************************************************************************
* Function Name  : cordic_sin_cos( precision 6 )
* Description    : calculate the sin & cos value of the input angle
* Input          : - angle_para   : input angle data in radians, divided by ¦Ð[range[-1,1],Q31 format];
* Output		     : - * cos_data   : the cos value of the input angle[range[-1,1],Q31 format]
                   - * sin_data   : the sin value of the input angle[range[-1,1],Q31 format]
* Return         : None
**************************************************************************/
void cordic_cos_sin_6(int angle_para, int* cos_data, int* sin_data);
/**************************************************************************
* Function Name  : cordic_sin_cos( precision 7 )
* Description    : calculate the sin & cos value of the input angle
* Input          : - angle_para   : input angle data in radians, divided by ¦Ð[range[-1,1],Q31 format];
* Output		     : - * cos_data   : the cos value of the input angle[range[-1,1],Q31 format]
                   - * sin_data   : the sin value of the input angle[range[-1,1],Q31 format]
* Return         : None
**************************************************************************/
void cordic_cos_sin_7(int angle_para, int* cos_data, int* sin_data);
/**************************************************************************
* Function Name  : cordic_sin_cos( precision 8 )
* Description    : calculate the sin & cos value of the input angle
* Input          : - angle_para   : input angle data in radians, divided by ¦Ð[range[-1,1],Q31 format];
* Output		     : - * cos_data   : the cos value of the input angle[range[-1,1],Q31 format]
                   - * sin_data   : the sin value of the input angle[range[-1,1],Q31 format]
* Return         : None
**************************************************************************/
void cordic_cos_sin_8(int angle_para, int* cos_data, int* sin_data);

/**************************************************************************
* Function Name  : cordic_atan_sqrt( precision 1 )
* Description    : calculate the atan & sqrt value of the input x,y
* Input          : - x             : input x data[range[-1,1],Q31 format];
*				         : - y             : input y data[range[-1,1],Q31 format];
                 : - precision     : the precison used in calculation
* Output		     : - * sqrt_data   : the sqrt value of the input x,y[Q31 format]
                   - * atan_data   : the atan value of the input x,y[Q31 format]
* Return         : None
**************************************************************************/
void cordic_atan_sqrt_1(int x, int y, int*sqrt_data, int* atan_data);
/**************************************************************************
* Function Name  : cordic_atan_sqrt( precision 2 )
* Description    : calculate the atan & sqrt value of the input x,y
* Input          : - x             : input x data[range[-1,1],Q31 format];
*				         : - y             : input y data[range[-1,1],Q31 format];
                 : - precision     : the precison used in calculation
* Output		     : - * sqrt_data   : the sqrt value of the input x,y[Q31 format]
                   - * atan_data   : the atan value of the input x,y[Q31 format]
* Return         : None
**************************************************************************/
void cordic_atan_sqrt_2(int x, int y, int*sqrt_data, int* atan_data);
/**************************************************************************
* Function Name  : cordic_atan_sqrt( precision 3 )
* Description    : calculate the atan & sqrt value of the input x,y
* Input          : - x             : input x data[range[-1,1],Q31 format];
*				         : - y             : input y data[range[-1,1],Q31 format];
                 : - precision     : the precison used in calculation
* Output		     : - * sqrt_data   : the sqrt value of the input x,y[Q31 format]
                   - * atan_data   : the atan value of the input x,y[Q31 format]
* Return         : None
**************************************************************************/
void cordic_atan_sqrt_3(int x, int y, int*sqrt_data, int* atan_data);
/**************************************************************************
* Function Name  : cordic_atan_sqrt( precision 4 )
* Description    : calculate the atan & sqrt value of the input x,y
* Input          : - x             : input x data[range[-1,1],Q31 format];
*				         : - y             : input y data[range[-1,1],Q31 format];
                 : - precision     : the precison used in calculation
* Output		     : - * sqrt_data   : the sqrt value of the input x,y[Q31 format]
                   - * atan_data   : the atan value of the input x,y[Q31 format]
* Return         : None
**************************************************************************/
void cordic_atan_sqrt_4(int x, int y, int*sqrt_data, int* atan_data);
/**************************************************************************
* Function Name  : cordic_atan_sqrt( precision 5 )
* Description    : calculate the atan & sqrt value of the input x,y
* Input          : - x             : input x data[range[-1,1],Q31 format];
*				         : - y             : input y data[range[-1,1],Q31 format];
                 : - precision     : the precison used in calculation
* Output		     : - * sqrt_data   : the sqrt value of the input x,y[Q31 format]
                   - * atan_data   : the atan value of the input x,y[Q31 format]
* Return         : None
**************************************************************************/
void cordic_atan_sqrt_5(int x, int y, int*sqrt_data, int* atan_data);
/**************************************************************************
* Function Name  : cordic_atan_sqrt( precision 6 )
* Description    : calculate the atan & sqrt value of the input x,y
* Input          : - x             : input x data[range[-1,1],Q31 format];
*				         : - y             : input y data[range[-1,1],Q31 format];
                 : - precision     : the precison used in calculation
* Output		     : - * sqrt_data   : the sqrt value of the input x,y[Q31 format]
                   - * atan_data   : the atan value of the input x,y[Q31 format]
* Return         : None
**************************************************************************/
void cordic_atan_sqrt_6(int x, int y, int*sqrt_data, int* atan_data);
/**************************************************************************
* Function Name  : cordic_atan_sqrt( precision 7 )
* Description    : calculate the atan & sqrt value of the input x,y
* Input          : - x             : input x data[range[-1,1],Q31 format];
*				         : - y             : input y data[range[-1,1],Q31 format];
                 : - precision     : the precison used in calculation
* Output		     : - * sqrt_data   : the sqrt value of the input x,y[Q31 format]
                   - * atan_data   : the atan value of the input x,y[Q31 format]
* Return         : None
**************************************************************************/
void cordic_atan_sqrt_7(int x, int y, int*sqrt_data, int* atan_data);
/**************************************************************************
* Function Name  : cordic_atan_sqrt( precision 8 )
* Description    : calculate the atan & sqrt value of the input x,y
* Input          : - x             : input x data[range[-1,1],Q31 format];
*				         : - y             : input y data[range[-1,1],Q31 format];
                 : - precision     : the precison used in calculation
* Output		     : - * sqrt_data   : the sqrt value of the input x,y[Q31 format]
                   - * atan_data   : the atan value of the input x,y[Q31 format]
* Return         : None
**************************************************************************/
void cordic_atan_sqrt_8(int x, int y, int*sqrt_data, int* atan_data);

#endif