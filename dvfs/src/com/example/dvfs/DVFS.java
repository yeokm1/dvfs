package com.example.dvfs;

public interface DVFS {
	
	public void start(int fpsLowbBound, int fpsHighBound, int slidingWindowLength);
	
	public void stop();

}
