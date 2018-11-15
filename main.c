#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"

int main(void){
initADC();
DAC_initialize();
initEffects();
initLED();
}
