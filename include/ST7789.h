#ifndef _ST7789_H_
#define _ST7789_H_

#include "Arduino.h"

#define SPI_SCK  18            
#define SPI_SDA  23               
#define SPI_RST  17              
#define SPI_DC   16              
#define SPI_CS   5               
#define SPI_BLK  22

#define SPI_FREQUENCY           40 * 1000000

#define SPI_RST_0  digitalWrite(SPI_RST,LOW)               
#define SPI_RST_1  digitalWrite(SPI_RST,HIGH)
#define SPI_DC_0  digitalWrite(SPI_DC,LOW)               
#define SPI_DC_1  digitalWrite(SPI_DC,HIGH)
#define SPI_CS_0  digitalWrite(SPI_CS,LOW)               
#define SPI_CS_1 digitalWrite(SPI_CS,HIGH)
#define SPI_BLK_0 digitalWrite(SPI_BLK,LOW)
#define SPI_BLK_1 digitalWrite(SPI_BLK,HIGH)

#define     RED          0XF800    //红色
#define     GREEN        0X07E0   //绿色
#define     BLUE         0X001F   //蓝色
#define     WHITE        0XFFFF   //白色

#define TFT_COLUMN_NUMBER 240
#define TFT_LINE_NUMBER 320
#define TFT_COLUMN_OFFSET 0


extern const uint8_t ASCII8x16_Table[];
extern const uint8_t ASCII16x24_Table[];
extern const uint8_t ASCII24x32_Table[];


void TFT_SEND_DATA(unsigned  char o_data);
void TFT_SEND_DATA_16(uint16_t o_data);
void TFT_SEND_CMD(unsigned char o_command);
void IO_init(void );
void TFT_init(void);
void TFT_Area(int x,int y,int size_x,int size_y,unsigned int color);
void TFT_full(unsigned int color);
void display_char_8X16(unsigned int x,unsigned int y,unsigned int color,unsigned char word);
void display_char_24X32(unsigned int x,unsigned int y,unsigned int color,unsigned char word);
void display_char_16X24(unsigned int x,unsigned int y,unsigned int color,unsigned char word);
void display_string(unsigned int x,unsigned int y,unsigned int color,int size,char words[]);

#endif