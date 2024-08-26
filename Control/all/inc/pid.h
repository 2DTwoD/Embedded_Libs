#ifndef PID_H
#define PID_H

#include <cstdint>

#include "interfaces.h"
#include "math_fun.h"

class PIDreg: public IUpdatedSomewhere{
	private:
		float pv{0.0f};
		uint16_t t{20};
	
		float sp{0.0f};
		float kp{1.0f};
		float ti{10.0f};
		float td{0.0f};
		float db{0.0f};
		float upOutLim{100.0f};
		float downOutLim{0.0f};
		bool AUTO{false};
		bool inverse{false};
	
		float out{0.0f};
		float e0{0.0f};
		float e1{0.0f};
		float e2{0.0f};
		float g0{0.0f};
		float g1{0.0f};
		float g2{0.0f};
	public:

    PIDreg(uint16_t t, float sp, float kp = 1.0f,
               float ti = 10.0f, float td = 0.0f, float db = 0.0f,
               float upOutLim = 100.0f, float downOutLim = 0.0f, bool inverse = false);
		uint16_t getT();
		void setPv(float);
		float getPv();
		void setSp(float);
		float getSp();
		void setKp(float);
		float getKp();
		void setTi(float);
		float getTi();
		void setTd(float);
		float getTd();
		void setDb(float);
		float getDb();
		void setUpOutLim(float);
		float getUpOutLim();
		void setDownOutLim(float);
		float getDownOutLim();
		void setOut(float);
		float getOut();
		void setAuto(bool);
		bool isAUTO();
		void setInverse(bool);
		bool isInverse();
		void updateSomewhere() override;
		void reset();
		void updateKoef();
		PIDreg& operator=(float value);
		float *const getPvRef();
		float *const getOutRef();
};

#endif //PID_H
