var onDeviceReady = function() {     
    var vista_devices = $("#devices");    
    bluetoothSerial.list(function(devices){
        vista_devices.text(JSON.stringify(devices));
    }, function(){
        vista_devices.text('error al obtener lista de dispositivos');
    });
};

$(document).ready(function() {  
    // are we running in native app or in browser?
    window.isphone = false;
    if(document.URL.indexOf("file://") == -1) {
        window.isphone = true;
    }

    if(window.isphone) {
        document.addEventListener("deviceready", onDeviceReady, false);
    } else {
        onDeviceReady();
    }
});

