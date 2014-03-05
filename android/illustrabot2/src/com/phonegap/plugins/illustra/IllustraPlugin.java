//adapted from https://github.com/giver/cordova-android-toast-plugin
//websocket service : see http://autobahn.ws/android/getstarted/ for details


package com.phonegap.plugins.illustra;


import android.util.Log;
import org.apache.cordova.api.*;
import android.widget.Toast;
import org.json.JSONArray;
import org.json.JSONException;
import de.tavendo.autobahn.WebSocketConnection;
import de.tavendo.autobahn.WebSocketException;
import de.tavendo.autobahn.WebSocketHandler;


public class IllustraPlugin extends CordovaPlugin {

    final String LOG_TAG = "IllustraLog";
	
	private final WebSocketConnection mConnection = new WebSocketConnection();
	
	
    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {

        Log.d(LOG_TAG, "Start Illustra");


		
		if (action.equals("connect")) {
			final String server_ip = args.getString(0);
			final String server_port = args.getString(1);
			final int toastDuration = 1000;
			final CallbackContext ctx = callbackContext;
			//final String wsuri = "ws://192.168.1.132:9000";
			final String wsuri =  "ws://echo.websocket.org" + "";		
			Runnable runnable = new Runnable() {
				public void run() {
				
				
					
					try {
					  mConnection.connect(wsuri, new WebSocketHandler() {
			  
						 @Override
						 public void onOpen() {
							Log.d(LOG_TAG, "Status: Connected to " + wsuri);
						   // mConnection.sendTextMessage("Starting illustrabot");
						   Toast.makeText(cordova.getActivity().getApplicationContext(), "Status: Connected to "+server_ip+":"+server_port, toastDuration).show();
							//send_commands();
							ctx.success();
						 }
			  
						 
						 
						 @Override
						 public void onTextMessage(String payload) {
							Log.d(LOG_TAG, "Got echo: " + payload);
							
						 }
			  
						 
						 
						 @Override
						 public void onClose(int code, String reason) {
							//MenuItem menuConnect = menu.findItem(R.id.connect);	  
							//menuConnect.setTitle(R.string.disconnect);            	 
							Log.d(LOG_TAG, "Connection lost.");
							// Let everyone know we are disconnected
							/*
							Bundle b = new Bundle();
							b.putBoolean("connection", false);
							Intent i = new Intent(CONNECTION_STATUS);
							i.putExtras(b);
							sendBroadcast(i, null);
							*/
						 }
						 
						 
					  });
					} catch (WebSocketException e) {
	  
						Log.d(LOG_TAG, e.toString());
						
					}
				}
			};

			cordova.getActivity().runOnUiThread(runnable);
			return true;
		}
		
		
		if (action.equals("getConnectionStatus")) {
			final int toastDuration = 1000;
			final CallbackContext ctx = callbackContext;
			Runnable runnable = new Runnable() {

				public void run() {
					Toast.makeText(cordova.getActivity().getApplicationContext(), "getConnectionStatus TBC", toastDuration).show();
					ctx.success();
				}
			};

			cordova.getActivity().runOnUiThread(runnable);
			return true;
		}


		if (action.equals("selectMotor")) {
			final String id = args.getString(0);	
			final int toastDuration = 1000;
			final CallbackContext ctx = callbackContext;
			Runnable runnable = new Runnable() {
				public void run() {
					Toast.makeText(cordova.getActivity().getApplicationContext(), "ID:"+id, toastDuration).show();
					ctx.success();
				}
			};

			cordova.getActivity().runOnUiThread(runnable);
			return true;
		}		
		
		
		return false;
        
    }


	
}
