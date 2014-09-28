cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "file": "plugins/com.ionic.keyboard/www/keyboard.js",
        "id": "com.ionic.keyboard.keyboard",
        "clobbers": [
            "cordova.plugins.Keyboard"
        ]
    },
    {
        "file": "plugins/com.simonmacdonald.prefs/src/android/applicationPreferences.js",
        "id": "com.simonmacdonald.prefs.applicationPreferences",
        "clobbers": [
            "applicationPreferences"
        ]
    },
    {
        "file": "plugins/com.tlantic.plugins.socket/www/socket.js",
        "id": "com.tlantic.plugins.socket.Socket",
        "clobbers": [
            "window.tlantic.plugins.socket"
        ]
    },
    {
        "file": "plugins/org.apache.cordova.device/www/device.js",
        "id": "org.apache.cordova.device.device",
        "clobbers": [
            "device"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "com.ionic.keyboard": "1.0.3",
    "com.simonmacdonald.prefs": "1.0.0",
    "com.tlantic.plugins.socket": "0.3.1",
    "org.apache.cordova.console": "0.2.10",
    "org.apache.cordova.device": "0.2.11"
}
// BOTTOM OF METADATA
});