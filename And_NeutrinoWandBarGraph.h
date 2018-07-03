
#ifndef And_NeutrinoWandBarGraph_h
#define And_NeutrinoWandBarGraph_h

#include "Arduino.h"
#include "SparkFunSX1509.h"

class And_NeutrinoWandBarGraph {
	public:
		//And_NeutrinoWandBarGraph(int pins[], int pinCount);
		And_NeutrinoWandBarGraph();
		void init(SX1509 io, int pins[], int pinCount);
		void setSpeed(int s);
		void setPowerLevel(int p);
		void begin(long startTime);
		void idle(long currentTime);
		void activate(long currentTime);
		
		const static int SPEED_MINIMAL = 70;
		const static int SPEED_NOMINAL = 50;
		const static int SPEED_SEMIMAL = 20;
		const static int SPEED_EXTREME = 10;
	private:
		SX1509 _io;
		bool _useSX1509;
		void _setup();
		void _pinMode(int p, byte mode);
		int _digitalRead(int p);
		void _digitalWrite(int p, int val);
		int* _pins;
		int _power;
		int _speed;
		bool _begun;
		bool _active;
		unsigned long _startTime;
		int _pinCount;
		int _currentIndex;
};

#endif