/*global cordova*/
cordova.define('cordova/plugin/tcpClient', function (require, exports, module) {

	var exec = require("cordova/exec");
	var TcpClient = function() {};
	
	TcpClient.prototype.connect = function(serverIP,serverPort,successCallback,failureCallback) {
	    exec(successCallback, failureCallback, 'TcpClientPlugin', 'connect', [serverIP,serverPort]);
	}
	
	// writes data to the bluetooth serial port - data must be a string
	TcpClient.prototype.write = function(data,successCallback,failureCallback) {
	    exec(successCallback, failureCallback, 'TcpClientPlugin', 'write', [data]);
	}
   
 	// Check wether TCP connection is alive or not
	TcpClient.prototype.isConnected = function(successCallback,failureCallback) {
	    exec(successCallback, failureCallback, 'TcpClientPlugin', 'isConnected', []);
	}  
   
   

	
    var tcpClient = new TcpClient();
    module.exports = tcpClient;	
});

if (!window.plugins) {
    window.plugins = {};
}

if (!window.plugins.TcpClient) {
    window.plugins.TcpClient = cordova.require("cordova/plugin/tcpClient");
    window.TcpClient = window.plugins.TcpClient;
}



