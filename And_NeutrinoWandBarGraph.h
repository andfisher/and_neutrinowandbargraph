
#ifndef And_NeutrinoWandBarGraph_h
#define And_NeutrinoWandBarGraph_h

#include "Arduino.h"

class And_NeutrinoWandBarGraph {
	public:
		And_NeutrinoWandBarGraph();
		void setSpeed(int s);
		void setPowerLevel(int p);
	private:
		int _power;
		int _speed;
};

#endif