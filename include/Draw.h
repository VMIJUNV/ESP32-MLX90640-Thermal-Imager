#ifndef _DRAW_H_
#define _DRAW_H_

#include "ST7789.h"

#define PIC_NUM 28800     //图片数据大小
#define pix 7
#define pix_x 8
#define pix_y 8


void TFT_Draw_Title();
void TFT_Draw_Map(uint16_t tempColor[],int max,int min);
void TFT_Draw_Num(float max_T,float min_T,float mid_T);
void Picture_Display(const unsigned char *ptr_pic);

#endif