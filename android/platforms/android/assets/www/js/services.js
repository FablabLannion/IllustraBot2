angular.module('starter.services', [])



.service('sharedIpParameters', function() {

  // Some fake testing data
  var port="1234";
  var address="192.168.1.2";
  var timer="1000";
  
  
  

  return {
            getAddress: function () {
                return address;
            },
            setAddress: function(value) {
                address = value;
            },
            getPort: function () {
                return port;
            },
            setPort: function(value) {
                port = value;
            },
            getTimer: function () {
                return timer
				;
            },
            setTimer: function(value) {
                timer = value;
            }				
  }
});