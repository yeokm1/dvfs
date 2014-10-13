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
	
	private int fpsLowBound;
	private int fpsHighBound;
	private int slidingWindowLength;
	
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
		
		io.startShell();


		scheduler = Executors.newSingleThreadScheduledExecutor();

		scheduler.scheduleAtFixedRate
		(new Runnable() {
			public void run() {
				float gpuUtil = gpu.getGPUUtilisation();
				
				float cpuUtil = cpu.getCPUUtilisation();

				int fps = gpu.getFPS(TIME_INTERVAL_NANO_SECONDS);
				
				
				Log.i(TAG, "FPS: " + Integer.toString(fps) + ", GPU: " + Float.toString(gpuUtil) + ", CPU: " + Float.toString(cpuUtil));
				
				processInputs(fps, gpuUtil, cpuUtil);

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
	
	
	private void processInputs(int fps, float gpuUtil, float cpuUtil){
		
		if(fps == GPUStuff.NO_FPS_CALCULATED){
			return;
		}
		
		if(fps > fpsHighBound){
			//We need to decrease FPS
			
			tryToMeetThisFPS(fpsLowBound, fps, gpuUtil, cpuUtil);
		} else if(fps < fpsLowBound){
			//We need to increase FPS
			
			tryToMeetThisFPS(fpsHighBound, fps, gpuUtil, cpuUtil);
		}
		
	}
	
	private void tryToMeetThisFPS(int Q_targetFPS, int Q_currentFPS, double UG_gpuUtil, double UC_cpuUtil){
		
		Log.i(TAG, "Meet this FPS: " + Q_targetFPS);
		
		long c_currentCPUFreq = cpu.getCurrentCPUFrequency();
		long g_currentGPUFreq = gpu.getCurrentGPUFrequency();
		
		double PC_priceCPU = (UC_cpuUtil * c_currentCPUFreq) / Q_currentFPS;
		double PG_priceGPU = (UG_gpuUtil * g_currentGPUFreq) / Q_currentFPS;
		
		double OC_expectedCPUCost = PC_priceCPU * Q_targetFPS;
		double OG_expectedGPUCost = PG_priceGPU * Q_targetFPS;
		
		int newCPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(OC_expectedCPUCost, cpu.getCPUFreqs(), UC_cpuUtil);
		int newGPUFreqPosition = findLowestFreqPositionThatMeetsThisCost(OG_expectedGPUCost, gpu.getGPUFreqs(), UG_gpuUtil);
		
		int currentCPUFreqPosition = cpu.getCpuFreqPosition();
		int currentGPUFreqPosinios = gpu.getGpuFreqPosition();
		
		if(currentCPUFreqPosition != newCPUFreqPosition){
			cpu.setCPUFreq(newCPUFreqPosition);
		}
		if(currentGPUFreqPosinios != newGPUFreqPosition){
			gpu.setGPUFreq(newGPUFreqPosition);
		}
		
		Log.i(TAG, "New GPU Freq: " + newGPUFreqPosition + ", new CPU Freq: " + newCPUFreqPosition);
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
