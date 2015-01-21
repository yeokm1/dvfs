package cpu;

import com.example.odroiddvfs.IOStuff;

public class CPUNexus5 extends CPU{
	
	public static final String FILE_CPU_AVAILABLE_FREQS = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies";
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

}
