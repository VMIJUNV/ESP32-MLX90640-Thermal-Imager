#include "DRAW.h"
#include <SPI.h>
#include <string.h>

void TFT_Draw_Title()
{
  char title1[]="Designed By";
  char title2[]="VMIJUNV";
  display_string(120-strlen(title1)*4,96,0xFFFF,8,title1);
  display_string(120-strlen(title2)*12,128,0xFFFF,24,title2);
}

void TFT_Draw_Map(uint16_t tempColor[],int max,int min)
{
  unsigned int ROW,COLUMN;
  unsigned int NUM;
  for(ROW=0;ROW<24;ROW++)             //ROW loop
  { 
    for(COLUMN=0;COLUMN<32 ;COLUMN++) //column loop
    {
      TFT_SEND_CMD(0x2a);     //Column address set
      TFT_SEND_DATA_16(pix*COLUMN+pix_y);    //start column
      TFT_SEND_DATA_16(pix*COLUMN+pix+pix_y-1);   //end column

      TFT_SEND_CMD(0x2b);     //Row address set
      TFT_SEND_DATA_16(pix*ROW+pix_x);    //start row
      TFT_SEND_DATA_16(10*ROW+pix+pix_x);    //end row
      TFT_SEND_CMD(0x2C);     //Memory write
      if ((ROW*32+(31-COLUMN)) == max) {for(NUM=0;NUM<pix*pix;NUM++) TFT_SEND_DATA_16(WHITE);}
      else if ((ROW*32+(31-COLUMN)) == min) {for(NUM=0;NUM<pix*pix;NUM++) TFT_SEND_DATA_16(0);}
      else {for(NUM=0;NUM<pix*pix;NUM++) TFT_SEND_DATA_16(tempColor[ROW*32+(31-COLUMN)]);}
    }
  }
}

void TFT_Draw_Num(float max_T,float min_T,float mid_T)
{
  char mid_S[10];
  char max_S[10];
  char min_S[10];
  char gap_S[10];
  sprintf(mid_S,"%.1f",mid_T);
  sprintf(max_S,"%.1f",max_T);
  sprintf(min_S,"%.1f",min_T);
  sprintf(gap_S,"%.1f",max_T-min_T);

  TFT_Area(119,84,2,16,0xFFFF);
  TFT_Area(112,91,16,2,0xFFFF);

  TFT_Area(60,210,120,32,0);
  TFT_Area(10,280,220,16,0);

  display_string(120-strlen(mid_S)*12,210,WHITE,24, mid_S);
  display_string(10,280,BLUE,8, min_S);
  display_string(120-strlen(gap_S)*4,280,GREEN,8,gap_S);
  display_string(230-strlen(max_S)*8,280,RED,8,max_S);
}
