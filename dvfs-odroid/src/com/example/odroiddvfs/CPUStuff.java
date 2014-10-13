package com.example.odroiddvfs;

public class CPUStuff {
	
	private static final String FILE_CPU_UTIL = "/proc/stat";
	
	private long prevLoad = 0;
	private long prevTotal = 0;
	
	public CPUStuff(){
		
	}
	
	public double getCPUUtilisation() {
	
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
	   
	   double util = ((double) (currentLoad - prevLoad)) / (currentTotal - prevTotal) * 100;
	   
	   prevLoad = currentLoad;
	   prevTotal = currentTotal;

	   return util;

	} 
	
	
}
