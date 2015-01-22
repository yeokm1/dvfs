package com.example.odroiddvfs;

import com.example.odroiddvfs.cpu.CPU;
import com.example.odroiddvfs.cpu.CPUNexus5;
import com.example.odroiddvfs.cpu.CPUOdroid;
import com.example.odroiddvfs.gpu.GPU;
import com.example.odroiddvfs.gpu.GPUNexus5;
import com.example.odroiddvfs.gpu.GPUOdroid;

import android.os.Build;

public class Factory {


	public static CPU getCPUObject(IOStuff io){
		if(isPhoneNexus5()){
			return new CPUNexus5(io);
		} else {
			return new CPUOdroid(io);
		}


	}

	public static GPU getGPUObject(IOStuff io){
		if(isPhoneNexus5()){
			return new GPUNexus5(io);
		} else {
			return new GPUOdroid(io);
		}

	}

	public static boolean isPhoneNexus5(){
		String deviceName = getDeviceName();
		if(deviceName.equals("LGE Nexus 5")){
			return true;
		} else {
			return false;
		}
	}

	public static String getDeviceName() {
		String manufacturer = Build.MANUFACTURER;
		String model = Build.MODEL;
		if (model.startsWith(manufacturer)) {
			return capitalize(model);
		} else {
			return capitalize(manufacturer) + " " + model;
		}
	}


	private static String capitalize(String s) {
		if (s == null || s.length() == 0) {
			return "";
		}
		char first = s.charAt(0);
		if (Character.isUpperCase(first)) {
			return s;
		} else {
			return Character.toUpperCase(first) + s.substring(1);
		}
	} 

}
