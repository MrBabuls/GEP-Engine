#pragma once
#include "SDL3/SDL.h"

class Time {
public: 
	static Time& Instance();

	Time(const Time&) = delete;
	Time& operator =(const Time&) = delete;

	void Tick();

	float GetDeltaTime() const { return m_deltaTime; }

private:
	Time() = default;
	~Time() = default;

	Uint64 m_lastCounter = 0;   // Last frame performance counter
	float m_deltaTime = 0.0f;	// Delta time in seconds
};