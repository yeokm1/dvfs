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
	
	public DVFS(int fpsLowBound, int fpsHighBound){
		io = new IOStuff();
		gpu = new GPUStuff(io);
		cpu = new CPUStuff(io);
		this.fpsLowBound = fpsLowBound;
		this.fpsHighBound = fpsHighBound;
	}
	
	
	public void start(){
		io.startShell();


		scheduler = Executors.newSingleThreadScheduledExecutor();

		scheduler.scheduleAtFixedRate
		(new Runnable() {
			public void run() {
				float gpuUtil = gpu.getGPUUtilisation();
				
				float cpuUtil = cpu.getCPUUtilisation();

				int fps = gpu.getFPS(TIME_INTERVAL_NANO_SECONDS);
				
				
				Log.i(TAG, "FPS: " + Integer.toString(fps) + ", GPU: " + Float.toString(gpuUtil) + ", CPU: " + Float.toString(cpuUtil));

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
	
	
	
}
