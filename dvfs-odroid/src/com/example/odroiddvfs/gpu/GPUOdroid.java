package com.example.odroiddvfs.gpu;

import java.util.Arrays;
import java.util.Collections;

import com.example.odroiddvfs.IOStuff;

import android.util.Log;

public class GPUOdroid extends GPU{

	
	
	public static final String FILE_GPU_UTIL = "/sys/module/pvrsrvkm/parameters/sgx_gpu_utilization";
	
	public static final String FILE_GPU_CURRENT_FREQ = "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_cur_clk";

	public static final String FILE_GPU_MIN_FREQ = "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_min_lock";
	public static final String FILE_GPU_MAX_FREQ = "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_max_lock";
	public static final String FILE_GPU_AVAILABLE_FREQS = "/sys/devices/platform/pvrsrvkm.0/sgx_dvfs_table";
	
	
	public GPUOdroid(IOStuff io){
		super(io);
	}
	
	public void setGPUFreq(int position){
		gpuFreqPosition = position;
		String newFrequency = gpuFreqsString[position];
		io.setThisValueToThisFile(newFrequency, FILE_GPU_MIN_FREQ);
		io.setThisValueToThisFile(newFrequency, FILE_GPU_MAX_FREQ);
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

	@Override
	protected int getFPSLines() {
		return 128;
	}
	
	

	
	
}
