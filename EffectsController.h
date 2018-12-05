#ifndef EFFECTSCONTROLLER_H
#define EFFECTSCONTROLLER_H
#include <stdbool.h>

void initEffects();
void handleEffects(uint16_t *data_array, bool overdrive_status, bool delay_status, int rec_index, int play_index);
	
#endif