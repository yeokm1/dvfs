package com.example.dvfs;

import com.example.odroiddvfs.R;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class KeepAppAlive extends Service{


	
	private static final int ONGOING_NOTIFICATION = 123;


	public void onCreate() {
		super.onCreate();
		updateNotification();
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		return START_STICKY;
	}
	

	private Notification createNotification(){
		String title = "DVFS";
		String text = "DVFS is running";
			
		Notification noti = createNotification(title, text);
		return noti;
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}


	public void updateNotification(){
		startForeground(ONGOING_NOTIFICATION, createNotification());
	}
	
	public Notification createNotification(String title, String subtext){
		Intent intent = new Intent(this,MainActivity.class);
		PendingIntent pending=PendingIntent.getActivity(this, 0, intent, 0);
	

		Notification.Builder mBuilder =
				new Notification.Builder(this)
		.setSmallIcon(R.drawable.ic_launcher)
		.setContentTitle(title)
		.setContentText(subtext)
		.setContentIntent(pending);

		Notification noti = mBuilder.build();

		return noti;
	}


}
