

////////////////////////////////////////////








	
	
	//TCP Server Copnfiguration
	//TODO: set it from a settings menu
	//var serverIP = "192.168.1.820";
	var serverIP = "192.168.1.100";
	var serverPort = "9099";

	



	
	function sendTCPFrame(e){

		console.log("sendTCPFrame...");

		
		TcpClient.write("0114030102030405f1f2f3f4e1e2e3e4d1d2d3d4");
		
		

	}

	function connect(e){
		console.log("connect to TCP Server...");
		TcpClient.connect(serverIP,serverPort);
		//Toast.shortshow("Plugin called successfully :-)");
	}
	
	
