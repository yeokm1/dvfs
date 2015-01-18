package com.example.odroiddvfs;

import java.util.Arrays;
import java.util.Collections;

public class CPUStuff {
	
	private static final int NUM_CORES = 4;
	
	public static final String FILE_CPU_SCALING_FREQ = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed";
	public static final String FILE_CPU_SCALING_GOVERNER = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";
	public static final String FILE_CPU_AVAILABLE_FREQS = "/sys/devices/system/cpu/cpufreq/iks-cpufreq/freq_table";
	
	public static final String[] FILE_CPU_CORE_ONLINE = {
		"/sys/devices/system/cpu/cpu0/online",
		"/sys/devices/system/cpu/cpu1/online",
		"/sys/devices/system/cpu/cpu2/online", 
		"/sys/devices/system/cpu/cpu3/online"
	};
	
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
	
	
	private boolean[] coreOnlineStatus = new boolean[NUM_CORES];
	private int coresActive = 0;
	
	private IOStuff io;
	
	public CPUStuff(IOStuff io){
		this.io = io;
		cpuFreqsString = getCPUFreqStrings();
		cpuFreqs = convertStringArrayToLong(cpuFreqsString);
		initValues();
		setGovernorToUserspace();
		setCPUFreq(0); //Assume it is the lowest at the beginning
//		setCoreOnlineStatus(1);
		
	}
	
	private void initValues(){
		for(int i = 0; i < NUM_CORES; i++){
			prevCoreLoad[i] = 0;
			prevCoreTotal[i] = 0;
			coreOnlineStatus[i] = true;
			coresActive++;
		}
	}
	
	public void setGovernorToUserspace(){
		io.setThisValueToThisFile("userspace", FILE_CPU_SCALING_GOVERNER);
	}
	
	public int getNumCoresActive(){
		return coresActive;
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
	
	
//	public void setCoreOnlineStatus(int newActiveCores){
//		
//		
//		for(int coreNumber = 0; coreNumber < NUM_CORES; coreNumber++){
//			boolean oldStatus = coreOnlineStatus[coreNumber];
//			boolean newStatus = coreNumber < newActiveCores ? true : false; 
//			
//			if(oldStatus != newStatus){
//				if(newStatus){
//					io.setThisValueToThisFile("1", FILE_CPU_CORE_ONLINE[coreNumber]);
//				} else {
//					io.setThisValueToThisFile("0", FILE_CPU_CORE_ONLINE[coreNumber]);
//				}
//
//			}
//			
//			coreOnlineStatus[coreNumber] = newStatus;
//			
//		}
//		
//		coresActive = newActiveCores;
//				
//	}
	
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
		
	public double getCoreWithHighestUtilisation(double[] coreUtils){		
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
		
		int coreTokStartMultiplier = -1;
		for(int coreNumber = 0; coreNumber < NUM_CORES; coreNumber++){
			
			boolean coreOnline = coreOnlineStatus[coreNumber];
			
			if(coreOnline){
				coreTokStartMultiplier++;
			} else {
				util[coreNumber] = 0; 
				continue;
			}
			
			int start = initialOffset + (coreTokStartMultiplier * subsequentOffset);
			
			long user = Long.parseLong(toks[start + 1]);
			long nice = Long.parseLong(toks[start + 2]);
			long system = Long.parseLong(toks[start + 3]);
			long currentIdle = Long.parseLong(toks[start + 4]);
			long iowait = Long.parseLong(toks[start + 5]);
			long irq = Long.parseLong(toks[start + 6]);
			long softirq = Long.parseLong(toks[start + 7]);

			long currentLoad = user + nice + system + iowait + irq + softirq;
			long currentTotal = currentLoad + currentIdle;

			util[coreNumber] = ((((double) (currentLoad - prevCoreLoad[coreNumber])) / (currentTotal - prevCoreTotal[coreNumber]))) * 100;

			prevCoreLoad[coreNumber] = currentLoad;
			prevCoreTotal[coreNumber] = currentTotal;
		}
		
		return util;
		
		
	}

	
	
}
