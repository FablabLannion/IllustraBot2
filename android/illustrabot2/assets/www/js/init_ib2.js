/*
* Entry point for Illustrabot2. Startappli will be called once device is ready
*/


//***************************************
// GLOBAL DATAS
//***************************************

//Used for intro
var introtitreFxSettings = {
	animation : 9,
	color : "#3F4C6B",
	animationType : "in",
	speed : 1500
};


//-------------------------------------------
function start_appli() {
// Entry point, called once device is ready
//-------------------------------------------
	$.mobile.allowCrossDomainPages = true; 
	$.support.cors = true; 
	$.mobile.phonegapNavigationEnabled = true;
	$('#intro').hide();
	$('#container').hide();

	$('#mainib').show();
	



}

















