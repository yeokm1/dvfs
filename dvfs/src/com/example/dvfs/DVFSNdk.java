package com.example.dvfs;

public class DVFSNdk implements DVFS {

	
	public DVFSNdk() {

	}
	
	@Override
	public void start(int fpsLowBound, int fpsHighBound, int slidingWindowLength) {
		startDVFS(fpsLowBound, fpsHighBound, slidingWindowLength);
	}

	@Override
	public void stop() {
		stopDVFS();
	}
	
	
    public native int startDVFS(int fpsLowBound, int fpsHighBound, int slidingWindowLength);
    
    public native int stopDVFS();
    
    static {
        System.loadLibrary("dvfs");
    }

}
