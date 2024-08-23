#include "all_tasks.h"

extern volatile uint16_t adcValues[];
extern AnalogMonitor adcMonitor;
extern Scale<float, int16_t> scale;
extern MovAvg avg;
extern Ramp ramp;
extern PulseInterrapt pulse;
extern AnalogOut analogOut;
RFimpulse rise(FALL);
extern uint8_t step;
extern Sequence seq1;
extern SequenceDelayed seq2;
bool strt;
bool rest;
bool reslt;
bool seq_start;
bool seq_lock;
bool seq_finish;

void testTask(void *pvParameters){
	int16_t tmp;
	while(true){
		adcMonitor = adcValues[0];
		
		avg = adcMonitor.get();
		
		scale = avg.get();
		
		ramp = scale.get();
		
		analogOut = avg.get();
		
		pulse = strt;
		rise = rest;
		if(rise.get()){
			pulse.reset();
		}
		reslt = pulse.get();
		//threePosReg = avg.get();
		seq1.start(seq_start);
		seq1.lock(seq_lock);
		seq1.finish(seq_finish);
		seq2.start(seq_start);
		seq2.lock(seq_lock);
		if(seq2.finished()){
			int tmp = 0;
		}
		vTaskDelay(1);
	}
}