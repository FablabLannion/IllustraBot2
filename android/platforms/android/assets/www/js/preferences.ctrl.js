angular.module('starter.preferences.ctrl', [])

  .controller('PreferencesCtrl', function ($scope, $log, $cordovaPreferences,sharedIpParameters) {

    var key = 'exampleKey';
    $scope.data = {};
    $scope.data.showMore = true;
    $scope.data.key = key;
	

	$scope.savePreferences = function () {
		$log.log('port:' +$scope.data.port);		
		$log.log('address:' +$scope.data.address);	
		$log.log('timer:' +$scope.data.timer);	
		

		
		//Update shared parameters to forward the values to other controllers
		sharedIpParameters.setAddress($scope.data.address);
		sharedIpParameters.setPort($scope.data.port);
		sharedIpParameters.setTimer($scope.data.timer);
		
		
		//Save data into internal memory	
		  $cordovaPreferences.set('address', $scope.data.address)
			.then(function (result) {
			  if (result) {
				$log.log('address' + ' was succesfully set to:', $scope.data.address);
				$scope.data.showMore = true;
			  } else {
				$log.log('address' + ' was not set to: ' + $scope.data.address + ' we got ', result);
			  }
			}, function (err) {
			  $log.log('address' + ' was not set to: ' + $scope.data.address + ' due to', err);
			});
			
			
		  $cordovaPreferences.set('timer', $scope.data.timer)
			.then(function (result) {
			  if (result) {
				$log.log('timer' + ' was succesfully set to:', $scope.data.timer);
				$scope.data.showMore = true;
			  } else {
				$log.log('timer' + ' was not set to: ' + $scope.data.timer + ' we got ', result);
			  }
			}, function (err) {
			  $log.log('timer' + ' was not set to: ' + $scope.data.timer + ' due to', err);
			});
			


		  $cordovaPreferences.set('port', $scope.data.port)
			.then(function (result) {
			  if (result) {
				$log.log('port' + ' was succesfully set to:', $scope.data.port);
				$scope.data.showMore = true;
			  } else {
				$log.log('port' + ' was not set to: ' + $scope.data.port + ' we got ', result);
			  }
			}, function (err) {
			  $log.log('port' + ' was not set to: ' + $scope.data.port + ' due to', err);
			});		
	
	};
	
	
	var loadDefaultValues = function () {

	  key='port';
      $cordovaPreferences.get(key)
        .then(function (value) {
          $log.log(key + ' get was succesfully:', value);
          $scope.data.port = value;
			//Update shared parameters to forward the values to other controllers
			sharedIpParameters.setPort($scope.data.port);		  
        }, function (err) {
          $log.log(key + ' get was not succesfully: ' + $scope.data.port + ' due to', err);
        });
		
		
	  key='address';
      $cordovaPreferences.get(key)
        .then(function (value) {
          $log.log(key + ' get was succesfully:', value);
          $scope.data.address = value;
		//Update shared parameters to forward the values to other controllers
		sharedIpParameters.setAddress($scope.data.address);
  
        }, function (err) {
          $log.log(key + ' get was not succesfully: ' + $scope.data.address + ' due to', err);
        });	
		
		
	  key='timer';
      $cordovaPreferences.get(key)
        .then(function (value) {
          $log.log(key + ' get was succesfully:', value);
          $scope.data.timer = value;
		  //Update shared parameters to forward the values to other controllers
		  sharedIpParameters.setTimer($scope.data.timer);
        }, function (err) {
          $log.log(key + ' get was not succesfully: ' + $scope.data.timer + ' due to', err);
        });		


	};	
	
	
	$scope.loadPreferences = function () {
		loadDefaultValues();
	}
	
	
	//Load default values at page load for IP,port, and so on
	//loadDefaultValues();
	
	
	

  });
