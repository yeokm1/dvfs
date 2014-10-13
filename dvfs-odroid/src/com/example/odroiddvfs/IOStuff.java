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
	public static final String COMMAND_ECHO_FORMAT = "echo %1$s > %2$s";
	public static final String TAG = "IOStuff";
	public static final int BUFF_LEN = 1000;
	
	public static final String COMMAND_CAT_FORMAT = "cat %1$s";
	
	private Process process;
	private DataOutputStream stdin;
	private InputStream stdout;
	
	
	
	public IOStuff(){
		
	}

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
	
	public void setThisValueToThisFile(String value, String filePath){
		String command = String.format(COMMAND_ECHO_FORMAT, value, filePath);
		runStaticCommandAsRoot(command);
	}
	
	public String[] getAvailableOptionsFromFile(String filename, boolean needsRoot){
		String result = getCurrentField(filename, needsRoot);
		String[] splitted = result.trim().split(" ");
		return splitted;
	}
	
	public String getCurrentField(String filename, boolean needsRoot){
		if(needsRoot){
			return runStaticCommandAsRoot(String.format(COMMAND_CAT_FORMAT, filename));
		} else {
			return runAndGetStaticCommandOutput(String.format(COMMAND_CAT_FORMAT, filename));
		}

	}
	

	
	public void startShell(){
		try {
			process = Runtime.getRuntime().exec("su");
			stdin = new DataOutputStream(process.getOutputStream());
			stdout = process.getInputStream();
		} catch (IOException e) {
		}
	}
	
	public void stopShell(){
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
	
	
	
	public void getRoot(View view){
		runStaticCommandAsRoot("ls");
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
	
	private String runAndGetStaticCommandOutput(String command){
		Log.d("TAG", "Run Command: " + command);
		try {
			Process process = Runtime.getRuntime().exec(command);
			BufferedReader bufferedReader = new BufferedReader(
					new InputStreamReader(process.getInputStream()));

			StringBuilder log=new StringBuilder();
			String line = "";
			while ((line = bufferedReader.readLine()) != null) {
				log.append(line);
				log.append("\n");


			}


			String result = log.toString();
			Log.d(TAG, "Run Command output: " + result);
			return result;


		}catch (IOException e) {

		}

		return "";
	}
	
}
