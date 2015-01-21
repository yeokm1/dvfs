package com.example.odroiddvfs;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import android.util.Log;

import com.example.odroiddvfs.gpu.GPU;

import cpu.CPU;

public class DVFS {

	private static final int DVFS_UPDATE_RATE = 1000;

	private static final float TARGET_CPU_UTILISATION = 80;

	public static final String TAG = "DVFS";

	private IOStuff io;
	private CPU cpu;
	private GPU gpu;

	private ScheduledExecutorService scheduler;
	public static final float TIME_INTERVAL_NANO_SECONDS = 1000000000;
	public static final int DO_NOT_PURSUE_FPS_VALUE = -1;

	private int fpsLowBound;
	private int fpsHighBound;
	private int slidingWindowLength;

	private int currentSlidingWindowPosition;


	public DVFS(){
		io = new IOStuff();
		io.startShell();
		cpu = Factory.getCPUObject(io);
		gpu = Factory.getGPUObject(io);
		io.stopShell();
	}


	public void start(int fpsLowbound, int fpsHighBound, int slidingWindowLength){
		this.fpsLowBound = fpsLowbound;
		this.fpsHighBound = fpsHighBound;
		this.slidingWindowLength = slidingWindowLength;
		currentSlidingWindowPosition = 0;

		io.startShell();


		scheduler = Executors.newSingleThreadScheduledExecutor();

		scheduler.scheduleAtFixedRate
		(new Runnable() {
			public void run() {
				int currentFPS = gpu.getFPS(TIME_INTERVAL_NANO_SECONDS);

				if(currentFPS != GPU.NO_FPS_CALCULATED){
					int newValueFPS = shouldPursueFPSRecalculationToThisFPS(currentFPS);
					Log.i(TAG, "FPS: " + Integer.toString(currentFPS));

					
					if(newValueFPS == DO_NOT_PURSUE_FPS_VALUE){
						processInputs(currentFPS, currentFPS, true);
					} else {
						processInputs(currentFPS, newValueFPS, false);
					}
				}






			}
		}, 0, DVFS_UPDATE_RATE, TimeUnit.MILLISECONDS);
	}

	public void stop(){
		io.stopShell();


		if(scheduler != null){
			scheduler.shutdownNow();
			scheduler = null;

		}
	}

	private int shouldPursueFPSRecalculationToThisFPS(int fps){
		if(fps > fpsHighBound){
			//We need to decrease FPS
			return fpsLowBound;
		} else if(fps < fpsLowBound){
			//We need to increase FPS
			return fpsHighBound;
		} else {
			currentSlidingWindowPosition = 0;
			return DO_NOT_PURSUE_FPS_VALUE;
		}

	}


	private void processInputs(int currentFPS, int newFPSValue, boolean fpsInRange){

		Log.i(TAG, "Current FPS: " + Integer.toString(currentFPS) + ", target FPS " + Integer.toString(newFPSValue));

		makeCPUMeetThisFPS(newFPSValue, currentFPS);

		if(fpsInRange){
			return;
		}

		Log.i(TAG, "Current FPS: " + Integer.toString(currentFPS) + ", target FPS " + Integer.toString(newFPSValue));

		currentSlidingWindowPosition++;

		if(currentSlidingWindowPosition > slidingWindowLength){

			currentSlidingWindowPosition = 0;

			makeGPUMeetThisFPS(newFPSValue, currentFPS);
		}


	}


	private void makeCPUMeetThisFPS(int Q_targetFPS, int Q_currentFPS){
		Log.i(TAG, "CPU meet this FPS: " + Q_targetFPS);

		int currentCPUFreqPosition = cpu.getCpuFreqPosition();

		long[] cpuFreqs = cpu.getCPUFreqs();

		double[] coreUtils = cpu.getCPUCoresUtilisation();
		Log.i(TAG, "CPU Util: " + coreUtils[0] + " " + coreUtils[1] + " " + coreUtils[2] + " " + coreUtils[3]);
	

		double UC_cpuUtil = cpu.getCoreWithHighestUtilisation(coreUtils);

		long c_currentCPUFreq = cpu.getCurrentCPUFrequency();	

		double PC_priceCPU = (UC_cpuUtil * c_currentCPUFreq) / Q_currentFPS;
		double OC_expectedCPUCost = PC_priceCPU * Q_targetFPS;

		double targetCPUUtil;

		if(UC_cpuUtil > TARGET_CPU_UTILISATION){
			targetCPUUtil = UC_cpuUtil;
		} else {
			targetCPUUtil = TARGET_CPU_UTILISATION;
		}


		int newCPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(OC_expectedCPUCost, cpuFreqs, targetCPUUtil);


		if(currentCPUFreqPosition != newCPUFreqPosition){
			cpu.setCPUFreq(newCPUFreqPosition);
		}	

		Log.i(TAG, "New CPU Freq: " + newCPUFreqPosition);
	}

	private void makeGPUMeetThisFPS(int Q_targetFPS, int Q_currentFPS){
		Log.i(TAG, "GPU meet this FPS: " + Q_targetFPS);

		int currentGPUFreqPosition = gpu.getGpuFreqPosition();

		long[] gpuFreqs = gpu.getGPUFreqs();

		long g_currentGPUFreq = gpu.getCurrentGPUFrequency();

		double PG_priceGPU = (g_currentGPUFreq) / (double) Q_currentFPS;


		double OG_expectedGPUCost = PG_priceGPU * Q_targetFPS;

		int newGPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(OG_expectedGPUCost, gpuFreqs, 1);





		if(currentGPUFreqPosition != newGPUFreqPosition){
			gpu.setGPUFreq(newGPUFreqPosition);
		}

		Log.i(TAG, "New GPU Freq: " + newGPUFreqPosition);
	}

	private int findLowestFreqPositionThatMeetsThisCost(double costToMeet, long[] availableFrequencies, double currentUtilisation){

		int position = 0;
		for(position = 0; position < availableFrequencies.length; position++){
			long chosenFreq = availableFrequencies[position];
			double calculatedCost = chosenFreq * currentUtilisation;
			if(calculatedCost >= costToMeet){
				return position;
			}
		}

		return availableFrequencies.length - 1;
	}






}
