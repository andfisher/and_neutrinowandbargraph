
#include "Arduino.h"
#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library
#include "And_NeutrinoWandBarGraph.h"

/**
 * @TODO
 * Implement fallback for anyone not usin an SX1509
 *
And_NeutrinoWandBarGraph::And_NeutrinoWandBarGraph(int pins[], int pinCount) 
{
	_useSX1509 = false;
	_pins = pins;
	_setup();
}
*/
And_NeutrinoWandBarGraph::And_NeutrinoWandBarGraph() 
{
}

void And_NeutrinoWandBarGraph::init(SX1509 io, int pins[], int pinCount)
{
	_useSX1509 = true;
	_io = io;
	_pins = pins;
	_pinCount = pinCount;
	_setup();
}

void And_NeutrinoWandBarGraph::_setup()
{
	_begun = false;
	_active = false;
	_speed = SPEED_MINIMAL;
	_currentIndex = 0;

	for (int i = 0; i < _pinCount; i++) {
		_pinMode(_pins[i], OUTPUT);
		_digitalWrite(_pins[i], LOW);
	}
}

void And_NeutrinoWandBarGraph::begin(long startTime)
{
	if (! _begun) {
		_startTime = startTime;
		_begun = true;
	}
}

void And_NeutrinoWandBarGraph::_pinMode(int p, byte mode)
{
	if (_useSX1509) {
		_io.pinMode(p, mode);
	} else {
		pinMode(p, mode);
	}
}

int And_NeutrinoWandBarGraph::_digitalRead(int p)
{
	if (_useSX1509) {
		return _io.digitalRead(p);
	}
	
	return digitalRead(p);
}


void And_NeutrinoWandBarGraph::_digitalWrite(int p, int val)
{
	if (_useSX1509) {
		_io.digitalWrite(p, val);
	} else {
		digitalWrite(p, val);
	}
}

/**
 * Suggested speeds:
 * 10 - Extreme
 * 20 - Seminal
 * 50 - Nominal
 * 100 - Minimal
 */
void And_NeutrinoWandBarGraph::setSpeed(int s)
{
	_speed = s;
}

void And_NeutrinoWandBarGraph::setPowerLevel(int p)
{
	switch(p) {
		case 0:
		case 1:
		case 2:
		case 3:
		_power = p;
	}
}

void And_NeutrinoWandBarGraph::idle(long currentTime)
{
	if (_active) {
		_startTime = currentTime;
		_active = false;
		_currentIndex = 0;
	}
	
	// No power? No bargraph animation
	if (_power == 0) {
		return;
	}

	// _power = 0, no bars lit
	// _power = 1, bars 1-5 lit
	// _power = 2, bars 1-10 lit
	// _power = 3, bars 1-15 lit
	int maxLitBars = (_power * 5);

	// How many milliseconds have passed since the animation started?
	unsigned long diff = currentTime - _startTime;
	
	// Based on the speed, how many animation frames have passed?
	// (One "keyframe" equals one lit LED)
	int frames = diff / _speed;
	
	// A local Index
	int localIndex = frames % maxLitBars;
	
	int cycle = frames / maxLitBars;
	bool asc = (cycle % 2 == 0); // only ascend bars on EVEN cycles
	
	int inverseIndex = (maxLitBars - localIndex) -1;
	
	/*
	For debugging long type limits affecting the animation
	Serial.print(diff);
	Serial.print(" - ");
	Serial.println(frames);
	*/
	
	for (int i = 0; i < _pinCount; i++) {
	
		if (asc) {
	
			if (i < localIndex) {
				_digitalWrite(i, HIGH);
			} else {
				_digitalWrite(i, LOW);
			}
		
		} else {
		
			if (i <= inverseIndex) {
				_digitalWrite(i, HIGH);
			} else {
				_digitalWrite(i, LOW);
			}
		
		}
	}
}

void And_NeutrinoWandBarGraph::activate(long currentTime)
{
	int centerLED = (_pinCount - 1) / 2;
	int s = _speed / _pinCount;

	if (! _active) {
		_startTime = currentTime;
		_active = true;
	}

	unsigned long diff = (currentTime - _startTime);
	int frames = diff / s;
	int cycle = (frames / _pinCount) % (centerLED +1);
	int ascendingIndex = centerLED + cycle;
	int descendingIndex = centerLED - cycle;

	if (ascendingIndex >= _pinCount) {
		ascendingIndex = centerLED;
	}
	if (descendingIndex < 0) {
		descendingIndex = centerLED;
	}

	for (int i = 0; i < _pinCount; i++) {
		if (i >= ascendingIndex && i < ascendingIndex + _power) {
			_digitalWrite(i, HIGH);
		} else if (i <= descendingIndex && i > descendingIndex - _power) {
			_digitalWrite(i, HIGH);
		} else {
			_digitalWrite(i, LOW);
		}
	}
	
}