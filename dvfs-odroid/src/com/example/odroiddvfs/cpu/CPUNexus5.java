package com.example.odroiddvfs.cpu;

import com.example.odroiddvfs.IOStuff;

public class CPUNexus5 extends CPU{
	
	//Assume mpdecision is not running
	public static final String FILE_CPU_AVAILABLE_FREQS = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies";
	public static final String FILE_CPU_SET_MIN_FREQ = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq";
	public static final String FILE_CPU_SET_MAX_FREQ = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq";
	
	private static final int NUM_CORES = 4;

	public CPUNexus5(IOStuff io) {
		super(io);
	}

	@Override
	protected String[] getCPUFreqStrings() {
		String[] givenFreqs = io.getAvailableOptionsFromFile(FILE_CPU_AVAILABLE_FREQS, false);
		return givenFreqs;
	}

	@Override
	protected int getNumCores() {
		return NUM_CORES;
	}

	@Override
	protected void priorToSetGovernorToUserspace() {
		io.setThisValueToThisFile(cpuFreqsString[0], FILE_CPU_SET_MIN_FREQ);
		io.setThisValueToThisFile(cpuFreqsString[cpuFreqsString.length - 1], FILE_CPU_SET_MAX_FREQ);		
	}

}
