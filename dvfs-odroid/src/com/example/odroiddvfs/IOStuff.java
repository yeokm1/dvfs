package com.example.odroiddvfs;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.RandomAccessFile;

import android.util.Log;
import android.view.View;

public class IOStuff {
	public static final int BUFF_LEN = 1000;
	
	private static Process process;
	private static DataOutputStream stdin;
	private static InputStream stdout;
	

	public static String getStringFromFile (String filePath){
		try {
			RandomAccessFile reader = new RandomAccessFile(filePath, "r");
			String output = reader.readLine();
			reader.close();
			return output;
		} catch (IOException e) {
			return "";
		}

	}
	

	
	public static void startShell(){
		try {
			process = Runtime.getRuntime().exec("su");
			stdin = new DataOutputStream(process.getOutputStream());
			stdout = process.getInputStream();
		} catch (IOException e) {
		}
	}
	
	public static void stopShell(){
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
	}
	
	
	
	public static void getRoot(View view){
		runStaticCommandAsRoot("ls");
	}


	

	public static String getRunningCommandOutput(String command){
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
	
	


	public static String runStaticCommandAsRoot(String command){


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
