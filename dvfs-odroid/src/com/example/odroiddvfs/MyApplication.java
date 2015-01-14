package com.example.odroiddvfs;

import android.app.Application;
import android.content.Intent;

public class MyApplication extends Application {
	 public void onCreate(){
		Intent keepAppIntent= new Intent(getApplicationContext(), KeepAppAlive.class);
		startService(keepAppIntent);
	 }
	 

}
