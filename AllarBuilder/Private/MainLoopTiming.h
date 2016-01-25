#pragma once

class FMainLoopTiming
{
public:
	/**
	* Constructor: set up initial timing values
	*/
	explicit FMainLoopTiming(float IdealTickRate);

	/**
	* Tick core application objects and throttle rate as requested in constructor
	*/
	void Tick();

private:
	/** Interval between each Tick that we're aiming for */
	float IdealFrameTime;
};