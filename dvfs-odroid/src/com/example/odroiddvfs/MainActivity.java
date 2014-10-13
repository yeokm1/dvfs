package com.example.odroiddvfs;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;


public class MainActivity extends Activity {

	public static final String TAG = "MainActivity";
	private DVFS dvfs;
	
	public static final int FPS_LOW = 40;
	public static final int FPS_HIGH = 50;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		dvfs = new DVFS();
	}

	public void startButtonPress(View view){
		
		
		

		EditText fpsLowBoundView = (EditText) findViewById(R.id.fpsLowBound);
		EditText fpsHighBoundView = (EditText) findViewById(R.id.fpsHighBound);
		EditText slidingWindowView = (EditText) findViewById(R.id.slidingWindow);
		
		try{
			int fpsLowBound = Integer.parseInt(fpsLowBoundView.getText().toString());
			int fpsHighBound = Integer.parseInt(fpsHighBoundView.getText().toString());
			int slidingWindow = Integer.parseInt(slidingWindowView.getText().toString());
			
			if(fpsLowBound <= 0 || fpsHighBound > 60
					|| fpsHighBound < fpsLowBound
					|| slidingWindow <= 0){
				throw new NumberFormatException();
			}
			dvfs.start(fpsLowBound,fpsHighBound, slidingWindow);
			
			Toast.makeText(this, "Started, FPS Low: " + fpsLowBound + ", FPS High: " + fpsHighBound + ", Sliding Window: " + slidingWindow, Toast.LENGTH_SHORT).show();
		} catch (NumberFormatException e){
			Toast.makeText(this, "Invalid Input", Toast.LENGTH_SHORT).show();
		}
	


	}


	public void stopButtonPress(View view){
		Toast.makeText(this, "Stopped", Toast.LENGTH_SHORT).show();
		dvfs.stop();
	}


	



	

}
