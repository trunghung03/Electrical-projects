#pragma once

struct Led {
	char m_PIN;
	bool m_ledOn;
	int m_brightness;
	bool m_turnUp;
	KeyState m_lastState;
	bool m_isHold;

	char m_swPIN;
	int swFlag = LOW;

	Led() {}
	~Led() {}
	Led(char PIN, char swPIN) {
		m_PIN = PIN;
		m_ledOn = false;
		m_brightness = 0;
		m_turnUp = true;
		m_lastState = IDLE;
		m_isHold = false;

		m_swPIN = swPIN;
		swFlag = HIGH;
	}
};