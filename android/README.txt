Android app for Illustrabot2 project, Fablab Kernel Lannion.
See http://fablab-lannion.org/wiki/index.php?title=IllustraBot2 for details on the project.

This app is built upon ionic framework.



use case:
1)  preference tab to load ip parameters
2)  debug tab pour open socket , send pattern or start  traffic
3)  dashboard tab to shake the arm :-) 


TODO:

- slider width to be improved
- TCP isconnected not working fine


    

--------------


plugin : https://github.com/Tlantic/cdv-socket-plugin hacked to send raw binary frame:

SocketPlugin.java: 
* hexStringToByteArray added
* send function updated to replace socket.write(data); with 
	byte[] binaryFrame=hexStringToByteArray(data);
	socket.writeKernel(binaryFrame);
Connection.java:	
* writeKernel added


AndroidManifest.xml updated to force landscape mode
<application android:hardwareAccelerated="true" android:icon="@drawable/icon" android:label="@string/app_name" android:screenOrientation="landscape">


ionic.css:
bar.balanced modified to use KerNel colors
.bar-balanced .buttons .customIconRed /Green added
