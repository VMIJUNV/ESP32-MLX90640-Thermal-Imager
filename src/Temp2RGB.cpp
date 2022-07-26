#include "Temp2RGB.h"
#include "Arduino.h"
#include <Wire.h>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
#include "ST7789.h"

int max_n=0;
int min_n=0;
float max_T;
float min_T;

paramsMLX90640 mlx90640;
const byte MLX90640_address = 0x33;
float tempValues[32 * 24];
uint16_t tempColor[32 * 24];

tRGBcolor Palette[4000];

void Temp_init()
{
  int i;
  uint16_t eeMLX90640[832];
  Wire.begin(33,32);
  Wire.setClock(400000); 
  Wire.beginTransmission((uint8_t)MLX90640_address);
  MLX90640_DumpEE(MLX90640_address, eeMLX90640);
  MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  MLX90640_SetRefreshRate(MLX90640_address, 0x04); 
  max_T=22.4;
  min_T=0;
  getRainbowPalette();
  for(i=0;i<224;i++)
  {
	TFT_Area(8+i,300,1,12, RGB565(Palette[i].r, Palette[i].g, Palette[i].b));
  }
  TFT_Area(8,270,224,2,WHITE);
}

void Analyze_Temp()
{
  readTempValues();
  Temp_Max_Min();
  getRainbowPalette();
  Temp2RGB();
}

void readTempValues() {
  uint16_t mlx90640Frame[834];
  for (byte x = 0 ; x < 2 ; x++) 
  {
    MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
    // float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);
    float tr = Ta - TA_SHIFT; 
    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, EMMISIVITY, tr, tempValues);
  }
}

void Temp_Max_Min()
{
  int i;
  max_T = tempValues[0];
  min_T = tempValues[0];
  for (i = 1; i < 24*32; i++)
  {
    if (tempValues[i] > max_T) {max_T = tempValues[i];max_n=i;}
    if (tempValues[i] < min_T) {min_T = tempValues[i];min_n=i;}
  }
}

void Temp2RGB()
{
	int PaletteSize= (uint16_t)((max_T-min_T)*10+1);
	int cnt = 0;
	for (int row = 0; row < 24; row++)
	{
		for (int col = 0; col < 32; col++, cnt++)
		{
			int16_t colorIdx = (tempValues[cnt] - min_T)*10;
			if (colorIdx < 0) colorIdx = 0;
			if (colorIdx >= PaletteSize) colorIdx = PaletteSize - 1;
	    	tempColor[cnt] = RGB565(Palette[colorIdx].r, Palette[colorIdx].g, Palette[colorIdx].b);
		}
	}
}

void getRainbowPalette()
{
	tRGBcolor *pBuff=Palette;
  	int steps= (uint16_t)((max_T-min_T)*10+1);
    if (steps % 5)
	{
    	steps = (steps / 5) * 5 + 5;
	}
	uint16_t partSize = steps / 5;
	tRGBcolor KeyColors[] =
	{
		{84, 0, 180},	// 紫罗兰
		{0, 97, 211},	// 蓝色
		{0, 145, 72},	// 绿色
		{207, 214, 0},	// 黄色
		{231, 108, 0},	// 橙色
		{193, 19, 33}	// 红色
	};
	for (uint8_t part = 0; part < 5; part++)
	{
	    for (uint16_t step = 0; step < partSize; step++)
		{
		    float n = (float)step / (float) (partSize - 1);
		    pBuff->r = ((float)KeyColors[part].r) * (1.0f - n) + ((float)KeyColors[part + 1].r) * n;
		    pBuff->g = ((float)KeyColors[part].g) * (1.0f - n) + ((float)KeyColors[part + 1].g) * n;
		    pBuff->b = ((float)KeyColors[part].b) * (1.0f - n) + ((float)KeyColors[part + 1].b) * n;
		    pBuff++;
		}
	}
}