package com.example.odroiddvfs;

import java.util.Arrays;
import java.util.Collections;

import android.util.Log;

public class GPUStuff {

	
	public static final int NO_FPS_CALCULATED = -1;
	public static final String FPS_COMMAND = "dumpsys SurfaceFlinger --latency SurfaceView\n";
	
	public static final String TAG = "GPU Stuff";
	
	public static final String FILE_GPU_UTIL = "/sys/module/pvrsrvkm/parameters/sgx_gpu_utilization";
	
	public static final String FILE_GPU_CURRENT_FREQ = "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_cur_clk";

	public static final String FILE_GPU_MIN_FREQ = "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_min_lock";
	public static final String FILE_GPU_MAX_FREQ = "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_max_lock";
	public static final String FILE_GPU_AVAILABLE_FREQS = "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_table";
	
	private long[] gpuFreqs;
	private String[] gpuFreqsString;
	private int gpuFreqPosition;;
	
	
	private IOStuff io;
	
	public GPUStuff(IOStuff io){
		this.io = io;
		gpuFreqsString = getGPUFreqStrings();
		gpuFreqs = convertStringArrayToLong(gpuFreqsString);
		setGPUFreq(0);
	}
	
	public long getCurrentGPUFrequency(){
		return gpuFreqs[gpuFreqPosition];
	}
	
	public int getGpuFreqPosition(){
		return gpuFreqPosition;
	}
	
	public long[] convertStringArrayToLong(String[] array){
		long[] output = new long[array.length];
		for(int i = 0; i < array.length; i++){
			String element = array[i];
			output[i] =  Long.parseLong(element);
		}
		
		return output;
	}
	
	public void setGPUFreq(int position){
		gpuFreqPosition = position;
		String newFrequency = gpuFreqsString[position];
		io.setThisValueToThisFile(newFrequency, FILE_GPU_MIN_FREQ);
		io.setThisValueToThisFile(newFrequency, FILE_GPU_MAX_FREQ);
	}
	
	public long[] getGPUFreqs(){
		return gpuFreqs;
	}
	
	public String[] getGPUFreqStrings(){
		String[] freqs = io.getAvailableOptionsFromFile(FILE_GPU_AVAILABLE_FREQS, false);
		Collections.reverse(Arrays.asList(freqs));
		return freqs;
	}
	
	public float getGPUUtilisation(){
		String output = IOStuff.getStringFromFile(FILE_GPU_UTIL);
		float numOutput = 0;
		
		try{
			numOutput = Integer.parseInt(output);
			numOutput /= 2.56;
		} catch (NumberFormatException e){
			Log.e(TAG + " GPU Util", "Cannot get GPU Util value");
		}
		return numOutput;
	}
	
	
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

		for(int i = 1; i <= 128 ; i++){
			String[] splitted = splitLine(output[i]);
			String thisFrameFinishTimeStr = splitted[2];
			double thisFrameFirstTime = Double.parseDouble(thisFrameFinishTimeStr);
			if((lastFrameFinishTime - thisFrameFirstTime) <= timeIntervalNanoSeconds){
				frameCount++;
			}

		}
		return frameCount;
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
	
	
}
