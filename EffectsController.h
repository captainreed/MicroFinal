#ifndef EFFECTSCONTROLLER_H
#define EFFECTSCONTROLLER_H
#include <stdbool.h>

void initEffects();
void handleEffects();
void configureInterrupts();
	
extern bool playBackEnabled;
#endif