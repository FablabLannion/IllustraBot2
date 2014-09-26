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
	$scope.frametoSend=frametoSend;
	
	$scope.connect = function () {
		$log.log('connectin from AngularJS...');
		connect(sharedIpParameters.getAddress(),sharedIpParameters.getPort());
	}	
})



.controller('AccountCtrl', function($scope,sharedIpParameters) {
	
    $scope.IpParameters = function() {
        return sharedIpParameters.getAddress() + ":" + sharedIpParameters.getPort();
    };	
});



