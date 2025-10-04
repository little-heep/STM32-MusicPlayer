#ifndef __BUZZER_H__
#define __BUZZER_H__
 
 #include <stdint.h>
 #include "driver_systick.h"
 #include "gpio.h"
 
 extern int notes[];
 extern int* melodys[];
 extern int* rhythms[];
 extern int musicsize[3];
 extern int durationtime[3];
 
 extern uint8_t isrnext;//是否切换下一曲的中断
 extern uint8_t speed;//播放倍速
 
 void MusicStart(int num);

#endif
