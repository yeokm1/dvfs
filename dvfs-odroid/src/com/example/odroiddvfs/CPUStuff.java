package com.example.odroiddvfs;

import java.util.Arrays;
import java.util.Collections;

public class CPUStuff {
	
	private static final int NUM_CORES = 4;
	
	public static final String FILE_CPU_SCALING_FREQ = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed";
	public static final String FILE_CPU_SCALING_GOVERNER = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";
	public static final String FILE_CPU_AVAILABLE_FREQS = "/sys/devices/system/cpu/cpufreq/iks-cpufreq/freq_table";
	
	private static final String FILE_CPU_UTIL = "/proc/stat";
	
	private static final int LOWEST_FREQ_POSITION = 7; //0.6 GHz
	private static final int HIGHEST_FREQ_POSITION = 17; //1.6 GHz
	
	private long prevLoad = 0;
	private long prevTotal = 0;
	
	private double[] prevCoreLoad = new double[NUM_CORES];
	private double[] prevCoreTotal = new double[NUM_CORES];
	
	private long[] cpuFreqs;
	private String[] cpuFreqsString;
	private int cpuFreqPosition;
	
	private IOStuff io;
	
	public CPUStuff(IOStuff io){
		this.io = io;
		cpuFreqsString = getCPUFreqStrings();
		cpuFreqs = convertStringArrayToLong(cpuFreqsString);
		setGovernorToUserspace();
		setCPUFreq(0); //Assume it is the lowest at the beginning
	}
	
	public void setGovernorToUserspace(){
		io.setThisValueToThisFile("userspace", FILE_CPU_SCALING_GOVERNER);
	}
	
	public long getCurrentCPUFrequency(){
		return cpuFreqs[cpuFreqPosition];
	}
	
	public int getCpuFreqPosition(){
		return cpuFreqPosition;
	}
	
	public void setCPUFreq(int position){
		cpuFreqPosition = position;
		String newFrequency = cpuFreqsString[position];
		io.setThisValueToThisFile(newFrequency, FILE_CPU_SCALING_FREQ);
	}
	
	public long[] getCPUFreqs(){
		return cpuFreqs;
	}
	
	
	private String[] getCPUFreqStrings(){
		String[] givenFreqs = io.getAvailableOptionsFromFile(FILE_CPU_AVAILABLE_FREQS, false);
		Collections.reverse(Arrays.asList(givenFreqs));
		String[] selectedFreqs = Arrays.copyOfRange(givenFreqs, LOWEST_FREQ_POSITION, HIGHEST_FREQ_POSITION);
		return selectedFreqs;
	}
	
	public long[] convertStringArrayToLong(String[] array){
		long[] output = new long[array.length];
		for(int i = 0; i < array.length; i++){
			output[i] = Long.parseLong(array[i]);
		}
		
		return output;
	}
	

	
	public float getCPUUtilisation() {
	
	  String cpuOutput = IOStuff.getStringFromFile(FILE_CPU_UTIL);

	  String[] toks = cpuOutput.split(" ");
	  
	  // From here http://www.linuxhowtos.org/System/procstat.htm
	  long user = Long.parseLong(toks[2]);
	  long nice = Long.parseLong(toks[3]);
	  long system = Long.parseLong(toks[4]);
	  long currentIdle = Long.parseLong(toks[5]);
	  long iowait = Long.parseLong(toks[6]);
	  long irq = Long.parseLong(toks[7]);
	  long softirq = Long.parseLong(toks[8]);
	    
	   
	   
	   long currentLoad = user + nice + system + iowait + irq + softirq;
	   long currentTotal = currentLoad + currentIdle;
	   
	   float util = (float) ((((double) (currentLoad - prevLoad)) / (currentTotal - prevTotal)) * 100);
	   
	   prevLoad = currentLoad;
	   prevTotal = currentTotal;

	   return util;

	}
	
	public double getCoreWithHighestUtilisation(){
		double[] coreUtils = getCPUCoresUtilisation();
		
		double highestUtil = 0;
		for(double util : coreUtils){
			if(util > highestUtil){
				highestUtil = util;
			}
		}
		
		return highestUtil;
	}
	
	public double[] getCPUCoresUtilisation(){
		String[] toks = io.getAvailableOptionsFromFile(FILE_CPU_UTIL, false);

		double[] util = new double[NUM_CORES];
		int initialOffset = 11; //The initial offset is to skip the all cores fields
		int subsequentOffset = 10;
		
		for(int i = 0; i < NUM_CORES; i++){
			int start = initialOffset + (i * subsequentOffset);
			
			long user = Long.parseLong(toks[start + 1]);
			long nice = Long.parseLong(toks[start + 2]);
			long system = Long.parseLong(toks[start + 3]);
			long currentIdle = Long.parseLong(toks[start + 4]);
			long iowait = Long.parseLong(toks[start + 5]);
			long irq = Long.parseLong(toks[start + 6]);
			long softirq = Long.parseLong(toks[start + 7]);

			long currentLoad = user + nice + system + iowait + irq + softirq;
			long currentTotal = currentLoad + currentIdle;

			util[i] = ((((double) (currentLoad - prevCoreLoad[i])) / (currentTotal - prevCoreTotal[i]))) * 100;

			prevCoreLoad[i] = currentLoad;
			prevCoreTotal[i] = currentTotal;
		}
		
		return util;
		
		
	}

	
	
}
