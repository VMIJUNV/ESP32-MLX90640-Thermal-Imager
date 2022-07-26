#include "Arduino.h"
#include "Draw.h"
#include "Temp2RGB.h"


extern int max_n;
extern int min_n;
extern float max_T;
extern float min_T;
extern float tempValues[];
extern uint16_t tempColor[];

void setup() {
  Serial.begin(115200);
  IO_init();
  SPI_BLK_1;
  SPI_CS_1;
  TFT_init();
  TFT_full(0);
  TFT_Draw_Title();
  delay(1000);
  TFT_full(0);
  Temp_init();
}

void loop(void) {
  Analyze_Temp();
  TFT_Draw_Map(tempColor,max_n,min_n);
  TFT_Draw_Num(max_T,min_T,(tempValues[367]+tempValues[368]+tempValues[399]+tempValues[400])/4);
}
