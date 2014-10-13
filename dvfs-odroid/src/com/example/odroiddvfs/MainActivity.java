package com.example.odroiddvfs;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;


public class MainActivity extends Activity {

	public static final String TAG = "MainActivity";

	public static final float TIME_INTERVAL_NANO_SECONDS = 1000000000;

	private ScheduledExecutorService scheduler;
	
	private IOStuff io;
	private GPUStuff gpu;
	private CPUStuff cpu;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		io = new IOStuff();
		gpu = new GPUStuff(io);
		cpu = new CPUStuff();

	}

	public void startButtonPress(View view){
		Toast.makeText(this, "Started", Toast.LENGTH_SHORT).show();

		io.startShell();


		scheduler = Executors.newSingleThreadScheduledExecutor();

		scheduler.scheduleAtFixedRate
		(new Runnable() {
			public void run() {
				float gpuUtil = gpu.getGPUUtilisation();
				Log.i(TAG + " GPU", Float.toString(gpuUtil));
				
				double cpuUtil = cpu.getCPUUtilisation();
				Log.i(TAG + " CPU", Double.toString(cpuUtil));

				int fps = gpu.getFPS(TIME_INTERVAL_NANO_SECONDS);
				Log.i(TAG + " FPS", Integer.toString(fps));

			}
		}, 0, 1000, TimeUnit.MILLISECONDS);


	}


	public void stopButtonPress(View view){
		Toast.makeText(this, "Stopped", Toast.LENGTH_SHORT).show();
		io.stopShell();


		if(scheduler != null){
			scheduler.shutdownNow();
			scheduler = null;

		}
	}


	



	

}
