package com.example.dvfs.cpu;

import java.util.Arrays;
import java.util.Collections;

import com.example.dvfs.IOStuff;

public class CPUOdroid extends CPU{
	
	private static final int NUM_CORES = 4;
	
	public static final String FILE_CPU_AVAILABLE_FREQS = "/sys/devices/system/cpu/cpufreq/iks-cpufreq/freq_table";
		
	private static final int LOWEST_FREQ_POSITION = 7; //0.6 GHz
	private static final int HIGHEST_FREQ_POSITION = 17; //Position 17 is actually 1.7 Ghz but we want 1.6 GHz and the array range copy is range exclusive on the upper value
	
	
	public CPUOdroid(IOStuff io){
		super(io);
	}
	

	@Override
	protected int getNumCores() {
		return NUM_CORES;
	}

	@Override
	protected String[] getCPUFreqStrings() {
		String[] givenFreqs = io.getAvailableOptionsFromFile(FILE_CPU_AVAILABLE_FREQS, false);
		Collections.reverse(Arrays.asList(givenFreqs));
		String[] selectedFreqs = Arrays.copyOfRange(givenFreqs, LOWEST_FREQ_POSITION, HIGHEST_FREQ_POSITION);
		return selectedFreqs;
	}


	@Override
	protected void afterSetGovernorToUserspace() {
		//Nothing
		
	}
	


	

	


	
	
}
