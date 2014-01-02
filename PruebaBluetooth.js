var onDeviceReady = function() {     
    var vista_devices = $("#devices"); 
    var vista_mensajes = $("#mensajes"); 
    var btnBuscar = $("#btnBuscar");
    var btnConectar = $("#btnConectar");
    var btnPrenderLed = $("#btnPrenderLed");
    var btnSuscribirse = $("#btnSuscribirse");
    
    btnBuscar.click(function(){
        vista_devices.text("buscando dispositivos...");
        bluetoothSerial.list(function(devices){
            vista_devices.text(devices.lenght);
        }, function(){
            vista_devices.text('error al obtener lista de dispositivos');
        });
    });
    
    btnConectar.click(function(){
        vista_devices.text("conectando...");
        bluetoothSerial.connect('00:11:06:14:02:42', 
            function(){
                vista_devices.text('conectado a linvor');
            }, function(){
                vista_devices.text('error al conectar');
        });
    });
    
    btnPrenderLed.click(function(){
        bluetoothSerial.write('L', 
            function(){
                
            }, function(){
                vista_devices.text('error al enviar L');
        });
    });
    
    btnSuscribirse.click(function(){
        // the success callback is called whenever data is received
        bluetoothSerial.subscribe('\n', function (data) {
            vista_mensajes.text(vista_mensajes.text() + JSON.stringify(data));
        }, function(){
            vista_devices.text('error al suscribirse');
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

