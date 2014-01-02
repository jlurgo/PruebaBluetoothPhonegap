var onDeviceReady = function() {     
    var vista_devices = $("#devices"); 
    var btn_buscar = $("#btnBuscar");
    btn_buscar.click(function(){
        vista_devices.text("buscando dispositivos...");
        bluetoothSerial.list(function(devices){
            vista_devices.text(JSON.stringify(devices));
        }, function(){
            vista_devices.text('error al obtener lista de dispositivos');
        });
    });
    vista_devices.text("presione el boton buscar");
};

$(document).ready(function() {  
    console.log("comenzando aplicacion de prueba bluetooth");
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

