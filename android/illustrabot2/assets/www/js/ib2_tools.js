


///////////////////////////////////////////////////
// Global Parameters
///////////////////////////////////////////////////	


	
	
	//TCP Server Copnfiguration
	//TODO: set it from a settings menu
	//var serverIP = "192.168.1.820";
	/*
	var serverIP = "192.168.128.46";
	var serverPort = "22300";
	*/
	var serverIP = "192.168.1.82";
	var serverPort = "9099";	
	
	var connectStatus = "false";
	var traffic= "false";

	
	//TODO : set period in a preference menu
	var sensorPeriod = "1000";	
	var start_time=0;
	var end_time=0;



///////////////////////////////////////////////////
// TCP Managment
///////////////////////////////////////////////////	


	//********************************************
	function sendTCPPattern(e){
	//For debug purpose only
	//********************************************
	TcpClient.write("0113030001020304f1f2f3f4e1e2e3e4d1d2d3d4");
	}

	//********************************************
	function sendTCPFrame(frametoSend){
	//********************************************

		console.log("sendTCPFrame:"+frametoSend);

		if (connectStatus=="false")
		{
		  console.log("Force TCP Connection...");
		  connect();
		
		}
		else
		{
		    //Connection established, can start send frames
			//TcpClient.write("0113030001020304f1f2f3f4e1e2e3e4d1d2d3d4");
			TcpClient.write(frametoSend);
			
		}
		
		

	}

	


	//********************************************
	function connect(e){
	//********************************************
		console.log("connect to TCP Server...");
		TcpClient.connect(serverIP,serverPort);
		//Toast.shortshow("Plugin called successfully :-)");
		connectStatus="true";
	}
	

///////////////////////////////////////////////////
// Sensor Managment
///////////////////////////////////////////////////	

	//********************************************	
	function startTraffic(e) {
	//********************************************
	traffic="true";
	}
	
	//********************************************	
	function stopTraffic(e) {
	//Disable sensor transmission
	//********************************************
	traffic="false";
	}	


	//********************************************	
	function pad(n, width, z) {
	//********************************************	
	  z = z || '0';
	  n = n + '';
	  return n.length >= width ? n : new Array(width - n.length + 1).join(z) + n;
	}
	
	//********************************************
	function decimalToHexString(number)
	//********************************************
	{
		//Truncate negative values
		if (number < 0)
		{
			number = 0;
		}
		hexavalue=pad(number.toString(16).toLowerCase(),8);
		//console.log(number+"<->0x"+hexavalue);
		return hexavalue;
		
		
	}


	
	//********************************************
	function updateRobotPosition(event){
	//********************************************

		

		end_time = new Date().getTime()	;
		
		var delta=end_time-start_time;
		if (delta>=sensorPeriod)
		{
			//Time to update sensors
			start_time=end_time;
			
			

			  
			  var gamma = Math.floor( (event.accelerationIncludingGravity.x+10)*100 ); //-10+10 => 0;2000
			  var beta = Math.floor( (event.accelerationIncludingGravity.y+10)*100 ); //-10+10 => 0;2000
			  //var alpha =  Math.floor( (event.accelerationIncludingGravity.z+10)*100 ); // not working?
			  var alpha = 0;
			  
			  
			  //Convert to hex format
			  hexaAlpha=decimalToHexString(alpha);
			  hexaBeta=decimalToHexString(beta);
			  hexaGamma=decimalToHexString(gamma);
			  
			  header="0113030001020304"
			  frametoSend=header+hexaAlpha+hexaBeta+hexaGamma;
			  
			  document.getElementById("log").innerHTML = "<ul><li>Alpha : " + alpha + "<->0x"+ hexaAlpha + "</li><li>Beta : " + 
			  beta + "<->0x"+ hexaBeta +  "</li><li>Gamma : " + gamma + "<->0x"+ hexaGamma + "</li><li>Frame : "+ frametoSend + "</li></ul>" ; 		

			  if (traffic== "true")
			  {
			    sendTCPFrame(frametoSend);
			  }
			  else
			  {
			    //console.log("Traffic Disabled");
			  
			  }
		}
		
		
	}	
	
	
