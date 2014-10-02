angular.module('starter.services', [])



.service('sharedIpParameters', function() {

  // Some fake testing data
  var port="8089";
  var address="91.121.84.129";
  var timer="500";
  
  
  

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