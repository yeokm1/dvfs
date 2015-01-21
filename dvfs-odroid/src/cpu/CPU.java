package cpu;

import com.example.odroiddvfs.IOStuff;

public abstract class CPU {
	private static final String FILE_CPU_UTIL = "/proc/stat";


	public static final String FILE_CPU_SCALING_FREQ = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed";
	public static final String FILE_CPU_SCALING_GOVERNER = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";
	
	public static final String[] FILE_CPU_CORE_ONLINE = {
		"/sys/devices/system/cpu/cpu0/online",
		"/sys/devices/system/cpu/cpu1/online",
		"/sys/devices/system/cpu/cpu2/online", 
		"/sys/devices/system/cpu/cpu3/online"
	};

	private static int NUM_CORES;
	protected IOStuff io;


	protected long prevLoad = 0;
	protected long prevTotal = 0;

	protected double[] prevCoreLoad;
	protected double[] prevCoreTotal;

	protected long[] cpuFreqs;
	protected String[] cpuFreqsString;
	protected int cpuFreqPosition;

	
	protected abstract String[] getCPUFreqStrings();
	
	protected abstract int getNumCores();

	public CPU(IOStuff io){
		this.io = io;
		NUM_CORES = getNumCores();
		prevCoreLoad = new double[NUM_CORES];
		prevCoreTotal = new double[NUM_CORES];
		initValues();
		
		cpuFreqsString = getCPUFreqStrings();
		cpuFreqs = convertStringArrayToLong(cpuFreqsString);
		
		setGovernorToUserspace();
		setCPUFreq(0); //Assume it is the lowest at the beginning
		

	}
	


	protected void initValues(){
		for(int i = 0; i < NUM_CORES; i++){
			prevCoreLoad[i] = 0;
			prevCoreTotal[i] = 0;
		}
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

		for(int coreNumber = 0; coreNumber < NUM_CORES; coreNumber++){

			int start = initialOffset + (coreNumber * subsequentOffset);

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


	protected long[] convertStringArrayToLong(String[] array){
		long[] output = new long[array.length];
		for(int i = 0; i < array.length; i++){
			output[i] = Long.parseLong(array[i]);
		}

		return output;
	}

}
