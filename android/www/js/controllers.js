angular.module('starter.controllers', [])

.controller('DashCtrl', function($scope) {
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



