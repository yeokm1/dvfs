package com.example.odroiddvfs;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
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
		
		dvfs = new DVFS(FPS_LOW, FPS_HIGH);
	}

	public void startButtonPress(View view){
		Toast.makeText(this, "Started", Toast.LENGTH_SHORT).show();
		dvfs.start();

	}


	public void stopButtonPress(View view){
		Toast.makeText(this, "Stopped", Toast.LENGTH_SHORT).show();
		dvfs.stop();
	}


	



	

}
