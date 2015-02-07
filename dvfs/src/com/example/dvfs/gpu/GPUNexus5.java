package com.example.dvfs.gpu;

import java.util.Arrays;
import java.util.Collections;

import com.example.dvfs.IOStuff;

public class GPUNexus5 extends GPU{

	public static final String POWER_SCALE_POLICY = "/sys/class/kgsl/kgsl-3d0/pwrscale/policy";
	public static final String FILE_GPU_MAX_FREQ = "/sys/class/kgsl/kgsl-3d0/max_gpuclk";
	public static final String FILE_GPU_AVAILABLE_FREQS = "/sys/class/kgsl/kgsl-3d0/gpu_available_frequencies";
	public static final String FILE_GPU_UTIL = "/sys/class/kgsl/kgsl-3d0/gpubusy";

	public GPUNexus5(IOStuff io) {
		super(io);
		setToNoGPUPolicy();
	}

	public void setToNoGPUPolicy(){
		io.setThisValueToThisFile("none", POWER_SCALE_POLICY);
	}

	@Override
	public String[] getGPUFreqStrings() {
		String[] freqs = io.getAvailableOptionsFromFile(FILE_GPU_AVAILABLE_FREQS, false);
		Collections.reverse(Arrays.asList(freqs));
		return freqs;
	}

	@Override
	public void setGPUFreq(int position) {
		gpuFreqPosition = position;
		String newFrequency = gpuFreqsString[position];
		io.setThisValueToThisFile(newFrequency, FILE_GPU_MAX_FREQ);	
	}

	@Override
	public float getGPUUtilisation() {
		try{
			String output = IOStuff.getStringFromFile(FILE_GPU_UTIL).trim();
			String[] splitted = output.split(" ");
			int busyCycles = Integer.parseInt(splitted[0].trim());
			int totalCycles = Integer.parseInt(splitted[1].trim());

			if(totalCycles == 0){
				return 0;
			} else {
				float util = (((float) busyCycles) / totalCycles) * 100;		
				return util;
			}
		}catch (NumberFormatException e){
			return 0;
		}

	}

	@Override
	protected int getFPSLines() {
		return 127;
	}

}
