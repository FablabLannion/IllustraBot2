//adapted from https://github.com/giver/cordova-android-toast-plugin

package com.phonegap.plugins.tcpclient;


import android.util.Log;

import org.apache.cordova.api.*;
import android.widget.Toast;
import org.json.JSONArray;
import org.json.JSONException;

public class TcpClientPlugin extends CordovaPlugin {


    // actions
    private static final String CONNECT = "connect";
    private static final String IS_CONNECTED  = "isConnected";
	private static final String WRITE = "write";
	
    // callbacks
    private CallbackContext connectCallback;

	


    final String TAG = "TcpClientPlugin";
	
	private TCPClient mTcpClient;
	

	public static byte[] hexStringToByteArray(String s) {
	    int len = s.length();
	    byte[] data = new byte[len / 2];
	    for (int i = 0; i < len; i += 2) {
	        data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
	                             + Character.digit(s.charAt(i+1), 16));
	    }
	    return data;
	}	

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {


		
		LOG.d(TAG, "action = " + action);	


        boolean validAction = true;
        
        if (action.equals(CONNECT)) {
		    connect(args, callbackContext);

		
        }  else if (action.equals(IS_CONNECTED)) {

            boolean connected = false;
			LOG.d(TAG, "connection status: TBC");
            PluginResult result = new PluginResult(PluginResult.Status.OK, connected);
            callbackContext.sendPluginResult(result);
		}  else if (action.equals(WRITE)) {

            String data = args.getString(0);
            byte[] binaryFrame=hexStringToByteArray(data);
			mTcpClient.sendFrame(binaryFrame);
            callbackContext.success();

        } else {
            validAction = false;		
		}		


        return validAction;
    }
	
	
    private void connect(JSONArray args, CallbackContext callbackContext) throws JSONException {
		String tcpIP = args.getString(0);
		String tcpPort = args.getString(1);
		LOG.d(TAG, "connect to TPC Server : TBC= "+tcpIP+":"+tcpPort);
		connectCallback = callbackContext;
		mTcpClient = new TCPClient(new TCPClient.OnMessageReceived() {
			
			//here the messageReceived method is implemented
			public void messageReceived(String message) {
				Log.d(TAG, "Message receveived from TCP Mode:"+message);
				
			}
		});
		mTcpClient.SERVERIP=tcpIP;
		mTcpClient.SERVERPORT= Integer.parseInt(tcpPort);					


		PluginResult result = new PluginResult(PluginResult.Status.NO_RESULT);
		result.setKeepCallback(true);
		callbackContext.sendPluginResult(result);	
		
		
		mTcpClient.run();



    }	


}
