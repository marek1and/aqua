#ifndef TIMER_H_
#define TIMER_H_

void (*timerTask)(void);

void cancelTimerTask();
void scheduleTimerTask(int seconds, void (*task)(void));


#endif /* TIMER_H_ */
