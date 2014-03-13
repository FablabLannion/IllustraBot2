package com.phonegap.plugins.tcpclient;

import android.os.Handler;
import android.util.Log;
import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

import org.apache.cordova.api.LOG;
 
public class TCPClient {
 
    private String serverMessage;

    
    public String SERVERIP = "192.168.1.82"; //your computer IP address
    public int SERVERPORT = 9099;
	
    final String TAG = "TcpClient";	
    
    
    private Handler handler = new Handler();
    
    private ServerSocket serverSocket;

    
	/*
    public static final String SERVERIP = "jjl.homelinux.net"; //your computer IP address
    public static final int SERVERPORT = 22560; 
	*/
    
    
    
    private OnMessageReceived mMessageListener = null;
    public boolean mRun = false;
    
    private boolean mConnectionStatus = false;
    
    
  
 
    PrintWriter out;
    BufferedReader in;


	OutputStream out2;
 
    /**
     *  Constructor of the class. OnMessagedReceived listens for the messages received from server
     */
    public TCPClient(OnMessageReceived listener) {
        mMessageListener = listener;
    }
 
    /**
     * Sends the message entered by client to the server
     * @param message text entered by client
     */
    public void sendMessage(String message){
        if (out != null && !out.checkError()) {
            out.println(message);
            out.flush();
            
        }
    }
	
    public void sendFrame(byte[] frame){
    	if (!mRun)
    	{
    		//TCP socket not openned, try to connect
    		run();
    	}
    	
    	else
    	{
	        if (out2 != null ) {
	            try {
					out2.write(frame);
					out2.flush();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					//e.printStackTrace();
					LOG.d(TAG, "!!!TCP Connection Lost!!!");
					stopClient();
					
				}
	            
	            
	        }
    	}
    }	
 
    public void stopClient(){    	
        mRun = false;
        LOG.d(TAG, "Close TCP Connection");
    }
 
    public void run_() {
 
        mRun = true;
 
        try {
            //here you must put your computer's IP address.
            InetAddress serverAddr = InetAddress.getByName(SERVERIP);
 

			LOG.d(TAG, "Connecting to"+SERVERIP+":"+SERVERPORT+"...");	
 
            //create a socket to make the connection with the server
            Socket socket = new Socket(serverAddr, SERVERPORT);
 
            try {
 
                //send the message to the server
                out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
                
                out2= socket.getOutputStream();


 
                LOG.d(TAG, "C: Sent.");
 
                LOG.d(TAG, "C: Done.");
 
                //receive the message which the server sends back
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
 
                //in this while the client listens for the messages sent by the server
                
                while (mRun) {
                    serverMessage = in.readLine();
 
                    if (serverMessage != null && mMessageListener != null) {
                        //call the method messageReceived from MyActivity class
                        mMessageListener.messageReceived(serverMessage);
                    }
                    serverMessage = null;
 
                }
                
 
                LOG.d(TAG, "S: Received Message: '" + serverMessage + "'");
                
 
            } catch (Exception e) {
 
                LOG.d(TAG, "S: Error", e);
 
            } finally {
                //the socket must be closed. It is not possible to reconnect to this socket
                // after it is closed, which means a new socket instance has to be created.
                socket.close();
            }
 
        } catch (Exception e) {
 
            LOG.d(TAG, "C: Error", e);
 
        }
 
    }
	
	
	
	public void run() {
        Thread fst = new Thread(new ClientThread());
        fst.start();	
	}
	
		
	 public class ClientThread implements Runnable {
	 
			public void run() {
		 
				mRun = true;
		 
				try {
					//here you must put your computer's IP address.
					InetAddress serverAddr = InetAddress.getByName(SERVERIP);
		 

					LOG.d(TAG, "Connecting to"+SERVERIP+":"+SERVERPORT+"...");	
		 
					//create a socket to make the connection with the server
					Socket socket = new Socket(serverAddr, SERVERPORT);
		 
					try {
		 
						//send the message to the server
						out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
						
						out2= socket.getOutputStream();


		 
						LOG.d(TAG, "C: Sent.");
		 
						LOG.d(TAG, "C: Done.");
		 
						//receive the message which the server sends back
						in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		 
						//in this while the client listens for the messages sent by the server
						
						while (mRun) {
							serverMessage = in.readLine();
		 
							if (serverMessage != null && mMessageListener != null) {
								//call the method messageReceived from MyActivity class
								mMessageListener.messageReceived(serverMessage);
							}
							serverMessage = null;
		 
						}
						
		 
						LOG.d(TAG, "S: Received Message: '" + serverMessage + "'");
						
		 
					} catch (Exception e) {
		 
						LOG.d(TAG, "S: Error", e);
		 
					} finally {
						//the socket must be closed. It is not possible to reconnect to this socket
						// after it is closed, which means a new socket instance has to be created.
						socket.close();
					}
		 
				} catch (Exception e) {
		 
					LOG.d(TAG, "C: Error", e);
		 
				}
		 
			}


		}	
		

		

		
		
	
 
    //Declare the interface. The method messageReceived(String message) will must be implemented in the MyActivity
    //class at on asynckTask doInBackground
    public interface OnMessageReceived {
        public void messageReceived(String message);
    }
}