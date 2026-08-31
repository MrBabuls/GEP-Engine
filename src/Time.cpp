#include "Time.h"

Time& Time::Instance()
{
	static Time instance;
	return instance;
}

void Time::Tick()
{
	Uint64 currentCounter = SDL_GetPerformanceCounter();

	if (m_lastCounter > 0)
	{
		Uint64 frequency = SDL_GetPerformanceFrequency();
		float delta = static_cast<float>(currentCounter - m_lastCounter) / static_cast<float>(frequency);

		if (delta > 0.016f)
		{
			delta = 0.016f;
		}

		m_deltaTime = delta;
	}

	m_lastCounter = currentCounter;
}