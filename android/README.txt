TPC:
plugin : https://github.com/Tlantic/cdv-socket-plugin
hacked to send raw binary frame:

SocketPlugin.java: 
* hexStringToByteArray added
* send function updated to replace socket.write(data); with 
	byte[] binaryFrame=hexStringToByteArray(data);
	socket.writeKernel(binaryFrame);
Connection.java:	
* writeKernel added


TODO:

- robustesse perte liaison TCP ( couper serveur en cours de traffic )
- icone fablab


use case:
1) table pref pour chargement params ip
2) table debug pour ouverture socket + démarrage traffic
3) table dashboard pour modif des trames par inclinaison smartphone


    <application android:hardwareAccelerated="true" android:icon="@drawable/icon" android:label="@string/app_name" android:screenOrientation="landscape">


- câbler arm/hand
- câbler slider


