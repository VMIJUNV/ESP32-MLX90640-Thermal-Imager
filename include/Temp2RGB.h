#ifndef _TEMP2RGB_H_
#define _TEMP2RGB_H_
#include "Arduino.h"


#define EMMISIVITY 0.95
#define TA_SHIFT 8 
#define RGB565(r, g, b)        (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} tRGBcolor;


void Temp_init();
void Analyze_Temp();
void readTempValues();
void Temp_Max_Min();
void Temp2RGB();
void getRainbowPalette();

#endif