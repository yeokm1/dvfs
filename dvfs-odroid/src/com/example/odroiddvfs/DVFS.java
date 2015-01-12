package com.example.odroiddvfs;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import android.util.Log;

public class DVFS {

	public static final String TAG = "DVFS";

	private IOStuff io;
	private CPUStuff cpu;
	private GPUStuff gpu;

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
		cpu = new CPUStuff(io);
		gpu = new GPUStuff(io);
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
				
				int newValueFPS = shouldPursueFPSRecalculationToThisFPS(currentFPS);
				Log.i(TAG, "FPS: " + Integer.toString(currentFPS));
				
				if(newValueFPS != DO_NOT_PURSUE_FPS_VALUE){
					processInputs(currentFPS, newValueFPS);
				}





			}
		}, 0, 1000, TimeUnit.MILLISECONDS);
	}

	public void stop(){
		io.stopShell();


		if(scheduler != null){
			scheduler.shutdownNow();
			scheduler = null;

		}
	}
	
	private int shouldPursueFPSRecalculationToThisFPS(int fps){
		if(fps == GPUStuff.NO_FPS_CALCULATED){
			return DO_NOT_PURSUE_FPS_VALUE;
		}

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


	private void processInputs(int currentFPS, int newFPSValue){
		
		Log.i(TAG, "Current FPS: " + Integer.toString(currentFPS) + ", target FPS " + Integer.toString(newFPSValue));

		currentSlidingWindowPosition++;

		if(currentSlidingWindowPosition > slidingWindowLength){
			
			currentSlidingWindowPosition = 0;
			
			float gpuUtil = gpu.getGPUUtilisation();
			Log.i(TAG, "GPU Util: " + gpuUtil);
			
			makeGPUMeetThisFPS(newFPSValue, currentFPS, gpuUtil);
		}
		
		float cpuUtil = cpu.getCPUUtilisation();
		
		Log.i(TAG, "CPU Util: " + cpuUtil);
	

		makeCPUMeetThisFPS(newFPSValue, currentFPS, cpuUtil);

	}


	private void makeCPUMeetThisFPS(int Q_targetFPS, int Q_currentFPS, double UC_cpuUtil){
		Log.i(TAG, "CPU meet this FPS: " + Q_targetFPS);

		long c_currentCPUFreq = cpu.getCurrentCPUFrequency();	

		double PC_priceCPU = (UC_cpuUtil * c_currentCPUFreq) / Q_currentFPS;
		double OC_expectedCPUCost = PC_priceCPU * Q_targetFPS;


		int newCPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(OC_expectedCPUCost, cpu.getCPUFreqs(), UC_cpuUtil);
		int currentCPUFreqPosition = cpu.getCpuFreqPosition();		

		if(currentCPUFreqPosition != newCPUFreqPosition){
			cpu.setCPUFreq(newCPUFreqPosition);
		}	

		Log.i(TAG, "New CPU Freq: " + newCPUFreqPosition);
	}

	private void makeGPUMeetThisFPS(int Q_targetFPS, int Q_currentFPS, double UG_gpuUtil){
		long g_currentGPUFreq = gpu.getCurrentGPUFrequency();

		double PG_priceGPU = (UG_gpuUtil * g_currentGPUFreq) / Q_currentFPS;


		double OG_expectedGPUCost = PG_priceGPU * Q_targetFPS;

		int newGPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(OG_expectedGPUCost, gpu.getGPUFreqs(), UG_gpuUtil);


		int currentGPUFreqPosinios = gpu.getGpuFreqPosition();


		if(currentGPUFreqPosinios != newGPUFreqPosition){
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
