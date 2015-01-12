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
			
			makeGPUMeetThisFPS(newFPSValue, currentFPS);
		}
		

	

		makeCPUMeetThisFPS(newFPSValue, currentFPS);

	}


	private void makeCPUMeetThisFPS(int Q_targetFPS, int Q_currentFPS){
		Log.i(TAG, "CPU meet this FPS: " + Q_targetFPS);
		
		int currentCPUFreqPosition = cpu.getCpuFreqPosition();
		
		long[] cpuFreqs = cpu.getCPUFreqs();
		
		if(currentCPUFreqPosition == 0 && Q_currentFPS >= Q_targetFPS //If we are at lowest GPU freq and still above target FPS, don't bother
				|| currentCPUFreqPosition == (cpuFreqs.length - 1) && Q_targetFPS >= Q_currentFPS){ //If we are at highest frequency and still below target FPS don't bother
			return;
			
		}
		
		float UC_cpuUtil = cpu.getCPUUtilisation();
		
		Log.i(TAG, "CPU Util: " + UC_cpuUtil);

		long c_currentCPUFreq = cpu.getCurrentCPUFrequency();	

		double PC_priceCPU = (UC_cpuUtil * c_currentCPUFreq) / Q_currentFPS;
		double OC_expectedCPUCost = PC_priceCPU * Q_targetFPS;


		int newCPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(OC_expectedCPUCost, cpuFreqs, UC_cpuUtil);


		if(currentCPUFreqPosition != newCPUFreqPosition){
			cpu.setCPUFreq(newCPUFreqPosition);
		}	

		Log.i(TAG, "New CPU Freq: " + newCPUFreqPosition);
	}

	private void makeGPUMeetThisFPS(int Q_targetFPS, int Q_currentFPS){
		Log.i(TAG, "GPU meet this FPS: " + Q_targetFPS);
		
		int currentGPUFreqPosition = gpu.getGpuFreqPosition();
		
		long[] gpuFreqs = gpu.getGPUFreqs();
		
		if(currentGPUFreqPosition == 0 && Q_currentFPS >= Q_targetFPS //If we are at lowest GPU freq and still above target FPS, don't bother
				|| currentGPUFreqPosition == (gpuFreqs.length - 1) && Q_targetFPS >= Q_currentFPS){ //If we are at highest frequency and still below target FPS don't bother
			return;
			
		}
		
		float UG_gpuUtil = gpu.getGPUUtilisation();
		Log.i(TAG, "GPU Util: " + UG_gpuUtil);
		
		long g_currentGPUFreq = gpu.getCurrentGPUFrequency();

		double PG_priceGPU = (UG_gpuUtil * g_currentGPUFreq) / Q_currentFPS;


		double OG_expectedGPUCost = PG_priceGPU * Q_targetFPS;

		int newGPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(OG_expectedGPUCost, gpuFreqs, UG_gpuUtil);





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
