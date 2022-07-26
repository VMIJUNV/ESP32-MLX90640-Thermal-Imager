#include "ST7789.h"
#include "Arduino.h"
#include <SPI.h>

void  IO_init(void )
{
  pinMode(SPI_RST,OUTPUT);//设置数字脚为输出
  pinMode(SPI_DC,OUTPUT);//设置数字脚为输出
  pinMode(SPI_BLK,OUTPUT);//设置数字脚为输出
  pinMode(SPI_CS,OUTPUT);//设置数字脚为输出
  SPI.begin(SPI_SCK,-1,SPI_SDA,SPI_CS);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setFrequency(SPI_FREQUENCY);
}

void TFT_SEND_CMD(unsigned char o_command)
{
  SPI_DC_0;
  SPI_CS_0;
  SPI.write(o_command);
  SPI_CS_1;
  SPI_DC_1;
}

void TFT_SEND_DATA(unsigned  char o_data)
{ 
  SPI_CS_0;
  SPI.write(o_data);
  SPI_CS_1;
}

void TFT_SEND_DATA_16(uint16_t o_data)
{ 
  SPI_CS_0;
  SPI.write16(o_data);
  SPI_CS_1;
}  

void TFT_Area(int x,int y,int size_x,int size_y,unsigned int color)
{
  unsigned int ROW,column;
  TFT_SEND_CMD(0x2a);     //Column address set
  TFT_SEND_DATA_16(x);    //start column
  TFT_SEND_DATA_16(x+size_x-1);   //end column

  TFT_SEND_CMD(0x2b);     //Row address set
  TFT_SEND_DATA_16(y);    //start row
  TFT_SEND_DATA_16(y+size_y);    //end row
  TFT_SEND_CMD(0x2C);     //Memory write
  for(ROW=0;ROW<size_y;ROW++)             //ROW loop
  { 
    for(column=0;column<size_x ;column++) //column loop
    {
      TFT_SEND_DATA_16(color);
    }
  }
}

void TFT_full(unsigned int color)
{
  unsigned int ROW,column;
  TFT_SEND_CMD(0x2a);     //Column address set
  TFT_SEND_DATA_16(0);    //start column
  TFT_SEND_DATA_16(240);   //end column

  TFT_SEND_CMD(0x2b);     //Row address set
  TFT_SEND_DATA_16(0);    //start row
  TFT_SEND_DATA_16(320);    //end row
  TFT_SEND_CMD(0x2C);     //Memory write
  for(ROW=0;ROW<TFT_LINE_NUMBER;ROW++)             //ROW loop
  { 
    for(column=0;column<TFT_COLUMN_NUMBER ;column++) //column loop
    {
      TFT_SEND_DATA_16(color);
    }
  }
}

void display_char_24X32(unsigned int x,unsigned int y,unsigned int color,unsigned char word)
{
  unsigned int column,row;
  TFT_SEND_CMD(0x2a);     //Column address set
  TFT_SEND_DATA_16(x);    //start column
  TFT_SEND_DATA_16(x+23);    //end column

  TFT_SEND_CMD(0x2b);       //Row address set
  TFT_SEND_DATA_16(y);    //start row
  TFT_SEND_DATA_16(y+32);      //end row
  TFT_SEND_CMD(0x2C);     //Memory write
  for(row=0;row<32*3;row++)  //column loop
  { 
    for(column=0;column<8;column++)
    {
      if( ASCII24x32_Table[32*3*(word-32)+row] & (0x80>>column)) TFT_SEND_DATA_16(color);
      else TFT_SEND_DATA_16(0);
    }
  }
}

void display_char_16X24(unsigned int x,unsigned int y,unsigned int color,unsigned char word)
{
  unsigned int column,row;
  TFT_SEND_CMD(0x2a);     //Column address set
  TFT_SEND_DATA_16(x);    //start column
  TFT_SEND_DATA_16(x+15);    //end column

  TFT_SEND_CMD(0x2b);       //Row address set
  TFT_SEND_DATA_16(y);    //start row
  TFT_SEND_DATA_16(y+24);      //end row
  TFT_SEND_CMD(0x2C);     //Memory write
  for(row=0;row<24*2;row++)  //column loop
  { 
    for(column=0;column<8;column++)
    {
      if( ASCII16x24_Table[24*2*(word-32)+row] & (0x80>>column)) TFT_SEND_DATA_16(color);
      else TFT_SEND_DATA_16(0);
    }
  }
}

void display_char_8X16(unsigned int x,unsigned int y,unsigned int color,unsigned char word)
{
  unsigned int column,row;
  TFT_SEND_CMD(0x2a);     //Column address set
  TFT_SEND_DATA_16(x);    //start column
  TFT_SEND_DATA_16(x+7);    //end column

  TFT_SEND_CMD(0x2b);       //Row address set
  TFT_SEND_DATA_16(y);    //start row
  TFT_SEND_DATA_16(y+16);      //end row
  TFT_SEND_CMD(0x2C);     //Memory write
  for(row=0;row<16;row++)  //column loop
  { 
    for(column=0;column<8;column++)
    {
      if( ASCII8x16_Table[16*(word-32)+row] & (0x80>>column)) TFT_SEND_DATA_16(color);
      else TFT_SEND_DATA_16(0);
    }
  }
}

void display_string(unsigned int x,unsigned int y,unsigned int color,int size,char words[])
{
  int i=0;
  if(size == 8)
  {
    while(words[i]!='\0')
    {
      display_char_8X16(x+8*i,y,color,words[i]);
      i++;
    }
  }
  else if(size == 16)
  {
    while(words[i]!='\0')
    {
      display_char_16X24(x+16*i,y,color,words[i]);
      i++;
    }
  }
  else if(size == 24)
  {
    while(words[i]!='\0')
    {
      display_char_24X32(x+24*i,y,color,words[i]);
      i++;
    }
  }
}

void TFT_init(void)        
  {
  SPI_RST_0;
  delay(10);
  SPI_RST_1;
  delay    (120);
//-----------------------ST7789V Frame rate setting-----------------//
//************************************************
                TFT_SEND_CMD(0x3A);        //65k mode
                TFT_SEND_DATA(0x05);
                TFT_SEND_CMD(0xC5);     //VCOM
                TFT_SEND_DATA(0x1A);
                TFT_SEND_CMD(0x36);                 // 屏幕显示方向设置
                  TFT_SEND_DATA(0x00);
                //-------------ST7789V Frame rate setting-----------//
                TFT_SEND_CMD(0xb2);   //Porch Setting
                TFT_SEND_DATA(0x05);
                TFT_SEND_DATA(0x05);
                TFT_SEND_DATA(0x00);
                TFT_SEND_DATA(0x33);
                TFT_SEND_DATA(0x33);

                TFT_SEND_CMD(0xb7);     //Gate Control
                TFT_SEND_DATA(0x05);      //12.2v   -10.43v
                //--------------ST7789V Power setting---------------//
                TFT_SEND_CMD(0xBB);//VCOM
                TFT_SEND_DATA(0x3F);

                TFT_SEND_CMD(0xC0); //Power control
                TFT_SEND_DATA(0x2c);

                TFT_SEND_CMD(0xC2);   //VDV and VRH Command Enable
                TFT_SEND_DATA(0x01);

                TFT_SEND_CMD(0xC3);     //VRH Set
                TFT_SEND_DATA(0x0F);    //4.3+( vcom+vcom offset+vdv)

                TFT_SEND_CMD(0xC4);     //VDV Set
                TFT_SEND_DATA(0x20);        //0v

                TFT_SEND_CMD(0xC6);       //Frame Rate Control in Normal Mode
                TFT_SEND_DATA(0X01);      //111Hz

                TFT_SEND_CMD(0xd0);       //Power Control 1
                TFT_SEND_DATA(0xa4);
                TFT_SEND_DATA(0xa1);

                TFT_SEND_CMD(0xE8);       //Power Control 1
                TFT_SEND_DATA(0x03);

                TFT_SEND_CMD(0xE9);       //Equalize time control
                TFT_SEND_DATA(0x09);
                TFT_SEND_DATA(0x09);
                TFT_SEND_DATA(0x08);
                //---------------ST7789V gamma setting-------------//
                TFT_SEND_CMD(0xE0); //Set Gamma
                TFT_SEND_DATA(0xD0);
                TFT_SEND_DATA(0x05);
                TFT_SEND_DATA(0x09);
                TFT_SEND_DATA(0x09);
                TFT_SEND_DATA(0x08);
                TFT_SEND_DATA(0x14);
                TFT_SEND_DATA(0x28);
                TFT_SEND_DATA(0x33);
                TFT_SEND_DATA(0x3F);
                TFT_SEND_DATA(0x07);
                TFT_SEND_DATA(0x13);
                TFT_SEND_DATA(0x14);
                TFT_SEND_DATA(0x28);
                TFT_SEND_DATA(0x30);
                 
                TFT_SEND_CMD(0XE1); //Set Gamma
                TFT_SEND_DATA(0xD0);
                TFT_SEND_DATA(0x05);
                TFT_SEND_DATA(0x09);
                TFT_SEND_DATA(0x09);
                TFT_SEND_DATA(0x08);
                TFT_SEND_DATA(0x03);
                TFT_SEND_DATA(0x24);
                TFT_SEND_DATA(0x32);
                TFT_SEND_DATA(0x32);
                TFT_SEND_DATA(0x3B);
                TFT_SEND_DATA(0x14);
                TFT_SEND_DATA(0x13);
                TFT_SEND_DATA(0x28);
                TFT_SEND_DATA(0x2F);

                TFT_SEND_CMD(0x20);     //反显
TFT_SEND_CMD(0x11); //Exit Sleep // 退出睡眠模式
delay    (120); 
TFT_SEND_CMD(0x29); //Display on // 开显示
}



