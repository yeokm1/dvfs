package com.example.odroiddvfs;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends Activity {

//	public static final String COMMAND_CAT_FORMAT = "cat %1$s";
	private static final int NO_FPS_CALCULATED = -1;
	public static final String TAG = "MainActivity";
	public static final String FPS_COMMAND = "dumpsys SurfaceFlinger --latency SurfaceView\n";


	public static final String FILE_GPU_UTIL = "/sys/module/pvrsrvkm/parameters/sgx_gpu_utilization";
//	public static final String GPU_UTIL_COMMAND = String.format(COMMAND_CAT_FORMAT, FILE_GPU_UTIL);

	public TextView outputView;

	public static final float TIME_INTERVAL_NANO_SECONDS = 1000000000;
	public static final int BUFF_LEN = 1000;





	private long totalFramesPerSeconds = 0;
	private int totalFrameCaptures = 0;


	Process process;
	DataOutputStream stdin;
	InputStream stdout;

	private ScheduledExecutorService scheduler;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		outputView = (TextView) findViewById(R.id.outputView);
	}

	public void startButtonPress(View view){
		Toast.makeText(this, "Started", Toast.LENGTH_SHORT).show();

		try {
			process = Runtime.getRuntime().exec("su");
			stdin = new DataOutputStream(process.getOutputStream());
			stdout = process.getInputStream();
		} catch (IOException e) {
		}
		totalFramesPerSeconds = 0;
		totalFrameCaptures = 0;

		scheduler = Executors.newSingleThreadScheduledExecutor();

		scheduler.scheduleAtFixedRate
		(new Runnable() {
			public void run() {
				float gpuUtil = getGPUUtilisation();
				Log.i(TAG + " GPU", Float.toString(gpuUtil));

				int fps = getFPS(TIME_INTERVAL_NANO_SECONDS);
				//outputView.setText(Integer.toString(fps));
				Log.i(TAG + " FPS", Integer.toString(fps));

				if(fps > 0){
					totalFrameCaptures++;
					totalFramesPerSeconds += fps;
				}
			}
		}, 0, 1000, TimeUnit.MILLISECONDS);


	}


	public void stopButtonPress(View view){
		Toast.makeText(this, "Stopped", Toast.LENGTH_SHORT).show();
		try {
			if(process != null){
				stdin.write("exit\n".getBytes());
				stdin.flush();
				stdin.close();
				process.waitFor();
				process.destroy();
			}
		} catch (InterruptedException e) {
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}


		if(scheduler != null){
			scheduler.shutdownNow();
			scheduler = null;

		}
		if(totalFrameCaptures != 0){
			int average = (int) (totalFramesPerSeconds / totalFrameCaptures);
			outputView.setText(Integer.toString(average));
		}

	}

	public float getGPUUtilisation(){
		String output = getStringFromFile(FILE_GPU_UTIL);
		float numOutput = 0;
		
		try{
			numOutput = Integer.parseInt(output);
			numOutput /= 2.56;
		} catch (NumberFormatException e){
			Log.e(TAG + " GPU Util", "Cannot get GPU Util value");
		}
		return numOutput;
	}
	
	public String convertStreamToString(InputStream is) throws Exception {
	    BufferedReader reader = new BufferedReader(new InputStreamReader(is));
	    StringBuilder sb = new StringBuilder();
	    String line = null;
	    while ((line = reader.readLine()) != null) {
	      sb.append(line);
	    }
	    reader.close();
	    return sb.toString();
	}

	public String getStringFromFile (String filePath){
	    File fl = new File(filePath);
	    FileInputStream fin;
		try {
			fin = new FileInputStream(fl);
		    String ret = convertStreamToString(fin);
		    //Make sure you close all streams.
		    fin.close();        
		    return ret;
		} catch (Exception e) {
			return "";
		}

	}


	public void getRoot(View view){
		runStaticCommandAsRoot("ls");
	}


	/* 
     From: http://src.chromium.org/svn/branches/1312/src/build/android/pylib/surface_stats_collector.py 
     adb shell dumpsys SurfaceFlinger --latency <window name>
     prints some information about the last 128 frames displayed in
     that window.
     The data returned looks like this:
     16954612
     7657467895508   7657482691352   7657493499756
     7657484466553   7657499645964   7657511077881
     7657500793457   7657516600576   7657527404785
     (...)

     The first line is the refresh period (here 16.95 ms), it is followed
     by 128 lines w/ 3 timestamps in nanosecond each:
     A) when the app started to draw
     B) the vsync immediately preceding SF submitting the frame to the h/w
     C) timestamp immediately after SF submitted that frame to the h/w

	 */
	public int getFPS(double timeIntervalNanoSeconds){
		String[] output = getFPSCommandOutput();

		if(output.length == 0 || output.length == 1){
			return NO_FPS_CALCULATED;
		}

		//First line is not used

		String lastLine = output[output.length - 1];
		String[] split = splitLine(lastLine);
		String lastFrameFinishTimeStr = split[2];

		double lastFrameFinishTime = Double.parseDouble(lastFrameFinishTimeStr);
		int frameCount = 0;

		for(int i = 1; i <= 128 ; i++){
			String[] splitted = splitLine(output[i]);
			String thisFrameFinishTimeStr = splitted[2];
			double thisFrameFirstTime = Double.parseDouble(thisFrameFinishTimeStr);
			if((lastFrameFinishTime - thisFrameFirstTime) <= timeIntervalNanoSeconds){
				frameCount++;
			}

		}
		return frameCount;
	}

	public String[] splitLine(String input){
		String[] result = input.split("\t");
		return result;
	}


	public String[] getFPSCommandOutput(){
		String out = getRunningCommandOutput(FPS_COMMAND);
		String[] lines = out.split("\n");
	//	Log.i(TAG, "Num Lines " + lines.length);

		return lines;
	}

	public String getRunningCommandOutput(String command){
		try {
			stdin.write((command).getBytes());


			stdin.flush();

			byte[] buffer = new byte[BUFF_LEN];
			int read;
			String out = new String();
			//read method will wait forever if there is nothing in the stream
			//so we need to read it in another way than while((read=stdout.read(buffer))>0)
			while(true){
				int bytesAvailable = stdout.available();
				if(bytesAvailable == 0){
					throw new Exception();
				}
				read = stdout.read(buffer);
				out += new String(buffer, 0, read);
				if(read<BUFF_LEN){
					break;
				}
			}

			return out;
		} catch (Exception ex) {
			return new String();
		}

	}
	
	


	public String runStaticCommandAsRoot(String command){


		try {
			String line;
			StringBuilder log=new StringBuilder();
			Process process = Runtime.getRuntime().exec("su");
			OutputStream stdin = process.getOutputStream();
			InputStream stdout = process.getInputStream();

			stdin.write((command + "\n").getBytes());
			stdin.write("exit\n".getBytes());
			stdin.flush();

			stdin.close();
			BufferedReader br =
					new BufferedReader(new InputStreamReader(stdout));
			while ((line = br.readLine()) != null) {
				Log.d("[Output]", line);
				log.append(line);
				log.append("\n");
			}
			br.close();
			process.waitFor();
			process.destroy();


			return log.toString();
		} catch (Exception ex) {
			return "";
		}

	}

}
