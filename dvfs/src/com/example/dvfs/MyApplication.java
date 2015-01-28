package com.example.dvfs;

import android.app.Application;
import android.content.Intent;

public class MyApplication extends Application {
	 public void onCreate(){
		Intent keepAppIntent= new Intent(this, KeepAppAlive.class);
		startService(keepAppIntent);
	 }
	 

}
