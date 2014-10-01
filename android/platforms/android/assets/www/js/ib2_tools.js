


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
	
	//var serverIP = "192.168.1.82";
	//var serverIP = "192.168.1.224";
	var serverIP = "192.168.1.79";
	var serverPort = "48569";
	var serverID="";
    var data = "0113030001020304f1f2f3f4e1e2e3e4d1d2d3d4";
    var  key = "";	
		
	
	var connectStatus = "false";
	var traffic= "false";


	
    //default periodi value to poll sensors
	var sensorPeriod = "1000";	
	var start_time=0;
	var end_time=0;
	var delta=0;
	var checkConnectionIntervalId="";
	
	//select which motor will be controlled
	var motorHeader="00000000"
	var armselected="00"
	var motorFooter="0000"
	var mecaLength="00";
	var frametoSend="";
	
	

	
	
///////////////////////////////////////////////////
// Preferences Managment
///////////////////////////////////////////////////		
	function ok (value) {}
	function fail (error) {}
	//********************************************
	function loadPreferences(){
	//********************************************
		prefs.fetch (ok, fail, 'key');
	}
	
	
	//********************************************
	function savePreferences(){
	//********************************************
		prefs.store (ok, fail, 'key', 'value');
	}	

///////////////////////////////////////////////////
// TCP Managment
///////////////////////////////////////////////////	




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
			 window.tlantic.plugins.socket.send(stub, stub, key, frametoSend);
			
		}
		
		

	}

	


	//********************************************
	function connect(serverIP, serverPort){
	//********************************************
		
		//Close previous connections if any
		window.tlantic.plugins.socket.disconnectAll(
		  function () {
			console.log('worked!');  
		  },

		  function () {
			console.log('failed!');
		  }
		);
		
		//Store ServerID for further check
		serverID=serverIP+":"+serverPort;
		console.log("connect to TCP Server "+serverID);
		
		
		document.getElementById("connectionStatus").className = "button button-icon icon ion-radio-waves";
		//TcpClient.connect(serverIP,serverPort);
		//Toast.shortshow("Plugin called successfully :-)");
		document.getElementById("log0").innerHTML = "<ul><li>Openning TCP connection ...</li></ul>" ; 	
		window.tlantic.plugins.socket.connect(onConnect, errorStub, serverIP, serverPort);
		
			
	
		


		
	}	
	
	//********************************************	
	function checkConnection() {
	//Check if  TCP connection is still alive
	//********************************************	
	
			console.log("check TPC connection is alive...");
			//window.tlantic.plugins.socket.send(stub, errorStub, key, "ALIVE?");
			window.tlantic.plugins.socket.isConnected(
			  serverID,
			  function (e) {
				if (JSON.stringify(e)=="0")
				{
					connectStatus="false";	
					document.getElementById("connectionStatus").className = "button button-icon icon ion-radio-waves customIconRed";
					document.getElementById("log0").innerHTML = "<ul><li>TCP connection closed!</li></ul>" ;
					/* later */
					clearInterval(checkConnectionIntervalId);
									
				}
				else
				{ 
					connectStatus="true";	
					document.getElementById("connectionStatus").className = "button button-icon icon ion-radio-waves customIconGreen";
					document.getElementById("log0").innerHTML = "<ul><li>TCP socket alive</li></ul>" ; 	
															
				}					
				
			  },
			  function (e) {
				console.log('error:'+JSON.stringify(e));		
			  }
			);


	}
	

	

		
	//********************************************
	 function stub(d) {
	//********************************************
            	console.log(d);
    }
	
	
	//********************************************
	 function errorStub(d) {
	//********************************************
					console.log('error with TCP connection ');
					connectStatus="false";	
					document.getElementById("connectionStatus").className = "button button-icon icon ion-radio-waves customIconRed";
					document.getElementById("log0").innerHTML = "<ul><li>TCP connection closed!</li></ul>" ;
					/* later */
					clearInterval(checkConnectionIntervalId);				
				
    }	
	
			
			
    //********************************************
        function onConnect(k) {
	//********************************************
            console.log('Established connection with ', k);
            key = k;
			connectStatus="true";
			//Call Connection Status check every 5s
			checkConnectionIntervalId = setInterval(checkConnection, 5000);			
    }
	

	
	//********************************************
        function sendTCPPattern() {
	//********************************************
            window.tlantic.plugins.socket.send(stub, stub, key, data);
        }

		//********************************************
        function isConnected() {
		//********************************************
            window.tlantic.plugins.socket.isConnected(key, stub, stub);
        }
		
		
		
        //Callback for tcp reception
        document.addEventListener('SOCKET_RECEIVE_DATA_HOOK', function (ev) {
                                  console.log('Data has been received: ', JSON.stringify(ev.metadata));
                                  
                                  alert(ev.metadata.data);
                                  
                                  var p = JSON.parse(ev.metadata.data);
                                  console.log(p);
                                  
                                  });		

	
	
	
	
	
	
	


	
	
	

///////////////////////////////////////////////////
// Sensor Managment
///////////////////////////////////////////////////	

	     //Callback for device orientation
		if(window.DeviceOrientationEvent) {
		  window.addEventListener("deviceorientation", updateRobotPosition, false);
		} 
		


	//********************************************
	function setHand(val){
	//********************************************
	console.log("Hand selected");
	document.getElementById("handButton").className = "button button-icon icon ion-android-hand customIconGreen";
	document.getElementById("armButton").className = "button button-icon icon ion-steam customIconGrey";	
	armselected="00";
	motorHeader=mecaLength+armselected+motorFooter;	
	}
	
	
	//********************************************
	function setArm(val){
	//********************************************
	console.log("Arm selected");
	document.getElementById("handButton").className = "button button-icon icon ion-android-hand customIconGrey";
	document.getElementById("armButton").className = "button button-icon icon ion-steam customIconGreen";
	armselected="01";
	motorHeader=mecaLength+armselected+motorFooter;
	}
		
	
	//********************************************
	function setMotorHeader(val){
	//********************************************
			switch(val)
			{
			case 0:
			  motorHeader="00000000"
			  break;			
			case 1:
			  motorHeader="01000000"	  
			  break;
			case 2:
			  motorHeader="00010000"
			  break;
			case 3:
			  motorHeader="00000100"
			  break;			  
			default:
			   //motor 4 selected by default
			   motorHeader="00000001"
			   
			}	
	

		console.log("New motor selected:"+motorHeader);

	}	

	

	//********************************************
	function updateMotorHeader(value){
	//********************************************

		intValue= parseInt(value);
		hexavalue=intValue.toString(16);
		hexavalue=hexavalue.toLowerCase();
		console.log(intValue+"<->0x"+hexavalue);
		mecaLength=hexavalue;
		motorHeader=mecaLength+armselected+motorFooter;


	}	

	
	//********************************************
	function updatePeriod(e){
	//********************************************

		//Refresh sample periodi used for sensor polling		
		sensorPeriod=document.getElementById("SamplePeriod").value;
		console.log("New period:"+sensorPeriod);

	}	

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
	//Use Device orientation: check http://code.tutsplus.com/tutorials/an-introduction-to-the-device-orientation-api--cms-21067 for further details
	//********************************************

		

			end_time = new Date().getTime()	;
			
			delta=end_time-start_time;
			if (delta>=sensorPeriod)
			{
				//Time to update sensors
				start_time=end_time;
				
				

				  
					var alpha = Math.floor( (event.alpha+0)*100 ); //0+360 => 0;36000
					var beta = Math.floor( (event.beta+90)*100 ); //-90+90 => 0;18000
					var gamma = Math.floor( (event.gamma+90)*100 ); //-90+270 => 0;36000
				  
				  
				  //Convert to hex format
				  hexaAlpha=decimalToHexString(alpha);
				  hexaBeta=decimalToHexString(beta);
				  hexaGamma=decimalToHexString(gamma);
				  
				  header="01170300"			  
				  footer="00000000"
				  frametoSend=header+motorHeader+hexaAlpha+hexaBeta+hexaGamma+footer;
				  
				  if (traffic== "true")
				  {
					sendTCPFrame(frametoSend);
				  }
				  else
				  {
					//console.log("Traffic Disabled");
				  
				  }			  
				  

				var element = document.getElementById("log");
				if (element != null ) {
					element.innerHTML = "<ul><li>Alpha : " + alpha + "<->0x"+ hexaAlpha + "</li><li>Beta : " + 
				  beta + "<->0x"+ hexaBeta +  "</li><li>Gamma : " + gamma + "<->0x"+ hexaGamma + "</li><li>Frame : "+ frametoSend + "</li></ul>" ;
				}


			}
	

		
		
	}	
	
	
