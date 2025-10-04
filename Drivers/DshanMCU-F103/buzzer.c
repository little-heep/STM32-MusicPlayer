#include "buzzer.h"

uint8_t isrnext=0;
uint8_t speed=1;

void playTone(int frequency, int duration) {
  int period = 1000000 / frequency;
  long cycles = (long)duration * 1000 / period;
  
  for(long i = 0; i < cycles; i++) {
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
    udelay(period / 2);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
    udelay(period / 2);
  }
}


void playSong(int melody[], int rhythm[], int length, int baseDuration) 
{  
  for(int i = 0; i < length; i++) {
		if(isrnext)//按键中断切换下一曲
		{
			isrnext=0;
			return;
		}
    if(melody[i] == -1) {
      // 休止符
      int restDuration = rhythm[i] * baseDuration / 2 /speed;
      mdelay(restDuration);
			continue;
    } else {
      int frequency = notes[melody[i]];
      int duration = rhythm[i] * baseDuration/speed;
      playTone(frequency, duration);
    }
    mdelay(20); // 音符间间隔
  }
}

void MusicStart(int num) {
  playSong(melodys[num],rhythms[num],musicsize[num], durationtime[num]);
  mdelay(1500); // 等待3秒后重复
}
