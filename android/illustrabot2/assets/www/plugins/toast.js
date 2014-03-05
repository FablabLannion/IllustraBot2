/**
 * @author Tanin Srivaraphong
 *
 * Usage:
 *
 * Toast.longshow("Long Toast);
 * Toast.shortshow("Short Toast");
 */

cordova.define("cordova/plugin/toast", function(require, exports, module) {

    var exec = require("cordova/exec");

    var Toast = function () {};

    Toast.prototype.LENGTH_SHORT = 0;
    Toast.prototype.LENGTH_LONG = 1;

    Toast.prototype._show = function (msg, duration, successCallback, failureCallback) {

        if (duration.toString().toLowerCase() === 'short') {
            duration = this.LENGTH_SHORT;
        } else if (duration.toString().toLowerCase() === 'long') {
            duration = this.LENGTH_LONG;
        }

        exec(successCallback, failureCallback, 'ToastPlugin', null, [msg, duration]);
    };

    //Short Toast
    Toast.prototype.shortshow = function (msg, duration, successCallback, failureCallback) {
        this._show(msg, this.LENGTH_SHORT, successCallback, failureCallback);
    };

    //Long Toast
    Toast.prototype.longshow = function (msg, duration, successCallback, failureCallback) {
        this._show(msg, this.LENGTH_LONG, successCallback, failureCallback);
    };

    var toast = new Toast();
    module.exports = toast;
});

if (!window.plugins) {
    window.plugins = {};
}

if (!window.plugins.Toast) {
    window.plugins.Toast = cordova.require("cordova/plugin/toast");
    window.Toast = window.plugins.Toast;
}
