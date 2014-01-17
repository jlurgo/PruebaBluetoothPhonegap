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
    
    if (window.DeviceOrientationEvent) {
      window.addEventListener('deviceorientation', function(eventData) {
//        // gamma is the left-to-right tilt in degrees, where right is positive
//        var tiltLR = eventData.gamma;
//    
//        // beta is the front-to-back tilt in degrees, where front is positive
//        var tiltFB = eventData.beta;
//    
        // alpha is the compass direction the device is facing in degrees
        var dir = eventData.alpha.toString();
        
        bluetoothSerial.write(dir + '\r', 
            function(){
                
            }, function(){
                vista_devices.text('error al enviar el angulo');
        });        
      }, false);
    } else {
      vista_devices.text("Device Orientation Not supported.");
    }
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

