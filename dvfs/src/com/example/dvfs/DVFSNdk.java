package com.example.dvfs;

public class DVFSNdk implements DVFS {

	
	public DVFSNdk() {

	}
	
	@Override
	public void start(int fpsLowBound, int fpsHighBound, int slidingWindowLength) {
		
		boolean isPhoneNexus5 = Factory.isPhoneNexus5();
		
		startDVFS(fpsLowBound, fpsHighBound, slidingWindowLength, isPhoneNexus5);
	}

	@Override
	public void stop() {
		stopDVFS();
	}
	
	
    public native int startDVFS(int fpsLowBound, int fpsHighBound, int slidingWindowLength, boolean isPhoneNexus5);
    
    public native int stopDVFS();
    
    static {
        System.loadLibrary("dvfs");
    }

}
