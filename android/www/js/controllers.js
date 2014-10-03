angular.module('starter.controllers', [])

.controller('DashCtrl', function($scope, $log) {

	$scope.setHand = function () {
		$log.log('setHand from AngularJS...');
		setHand();
	}	
	
	$scope.setArm = function () {
		$log.log('setArm from AngularJS...');
		setArm();
	}		
})

.controller('DebugCtrl', function($scope, $log,sharedIpParameters) {

    $scope.IpParameters = function() {
        return sharedIpParameters.getAddress() + ":" + sharedIpParameters.getPort();
    };
	
    $scope.Timer_Values = function() {
        return "Timer:"+sharedIpParameters.getTimer() + "ms" ;
    };	
	
	
	
	$scope.Timer=sharedIpParameters.getTimer();
	$scope.frametoSend=frametoSend;
	
	$scope.connect = function () {
		$log.log('connectin from AngularJS...');
		connect(sharedIpParameters.getAddress(),sharedIpParameters.getPort());
	}

	$scope.startTraffic = function () {
		$log.log('connecting from AngularJS...');
		updatePeriod($scope.Timer);
		startTraffic();
	}
	
})



.controller('AccountCtrl', function($scope,sharedIpParameters) {
	
    $scope.IpParameters = function() {
        return sharedIpParameters.getAddress() + ":" + sharedIpParameters.getPort();
    };	
});



