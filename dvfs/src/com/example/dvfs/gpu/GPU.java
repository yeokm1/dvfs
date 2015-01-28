package com.example.dvfs.gpu;

import com.example.dvfs.IOStuff;

public abstract class GPU {
	
	public static final String TAG = "GPU";
	
	public static final int NO_FPS_CALCULATED = -1;
	protected static final String FPS_COMMAND = "dumpsys SurfaceFlinger --latency SurfaceView\n";
	
	protected int FPS_LINES;
	
	
	protected long[] gpuFreqs;
	protected String[] gpuFreqsString;
	protected int gpuFreqPosition;;
	
	
	protected IOStuff io;
	
	
	public GPU(IOStuff io){
		this.io = io;
		gpuFreqsString = getGPUFreqStrings();
		gpuFreqs = convertStringArrayToLong(gpuFreqsString);
		FPS_LINES = getFPSLines();
		setGPUFreq(0);
	}
		
	public long[] getGPUFreqs(){
		return gpuFreqs;
	}
	
	public long getCurrentGPUFrequency(){
		return gpuFreqs[gpuFreqPosition];
	}
	
	public int getGpuFreqPosition(){
		return gpuFreqPosition;
	}
	
	public abstract String[] getGPUFreqStrings();
	
	public abstract void setGPUFreq(int position);
	
	public abstract float getGPUUtilisation();
	
	protected abstract int getFPSLines();
	
	
	/* 
    From: http://src.chromium.org/svn/branches/1312/src/build/android/pylib/surface_stats_collector.py 
    adb shell dumpsys SurfaceFlinger --latency <window name>
    prints some information about the last 128 frames displayed in
    that window.
    The data returned looks like this:
    16954612
    7657467895508   7657482691352   7657493499756
    7657484466553   7657499645964   7657511077881
    7657500793457   7657516600576   7657527404785
    (...)

    The first line is the refresh period (here 16.95 ms), it is followed
    by 128 lines w/ 3 timestamps in nanosecond each:
    A) when the app started to draw
    B) the vsync immediately preceding SF submitting the frame to the h/w
    C) timestamp immediately after SF submitted that frame to the h/w

	 */
	public int getFPS(double timeIntervalNanoSeconds){
		String[] output = getFPSCommandOutput();

		if(output.length == 0 || output.length == 1){
			return NO_FPS_CALCULATED;
		}

		//First line is not used
		

		String lastLine = output[output.length - 1];
		String[] split = splitLine(lastLine);
		String lastFrameFinishTimeStr = split[2];

		double lastFrameFinishTime = Double.parseDouble(lastFrameFinishTimeStr);
		int frameCount = 0;

		for(int i = 1; i <= FPS_LINES ; i++){
			String[] splitted = splitLine(output[i]);
			String thisFrameFinishTimeStr = splitted[2];
			double thisFrameFirstTime = Double.parseDouble(thisFrameFinishTimeStr);
			if((lastFrameFinishTime - thisFrameFirstTime) <= timeIntervalNanoSeconds){
				frameCount++;
			}

		}
		
		if(frameCount > 100 || frameCount <= 3){
			return NO_FPS_CALCULATED;
		} else {
			return frameCount;
		}

	}

	private String[] splitLine(String input){
		String[] result = input.split("\t");
		return result;
	}


	private String[] getFPSCommandOutput(){
		String out = io.getRunningCommandOutput(FPS_COMMAND);
		String[] lines = out.split("\n");
	//	Log.i(TAG, "Num Lines " + lines.length);

		return lines;
	}
	
	public long[] convertStringArrayToLong(String[] array){
		long[] output = new long[array.length];
		for(int i = 0; i < array.length; i++){
			String element = array[i];
			output[i] =  Long.parseLong(element);
		}
		
		return output;
	}
}
