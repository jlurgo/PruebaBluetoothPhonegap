var onDeviceReady = function() {     
    var vista_devices = $("#devices"); 
    var vista_mensajes = $("#mensajes"); 
    var btnBuscar = $("#btnBuscar");
    var btnConectar = $("#btnConectar");
    var btnPrenderLed = $("#btnPrenderLed");
    var btnSuscribirse = $("#btnSuscribirse");
    
    var conector = new ConectorArduinoBluetooth();
    
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
        conector.recibirMensaje('La puta madre');
    });
    
    btnSuscribirse.click(function(){
        // the success callback is called whenever data is received
        bluetoothSerial.subscribe('\n', function (data) {
            vista_mensajes.text(vista_mensajes.text() + JSON.stringify(data));
        }, function(){
            vista_devices.text('error al suscribirse');
        });
    });
    
//    window.compass.watchHeading(function(heading){
//        bluetoothSerial.write(heading.magneticHeading.toString() + '\r',
//                              function(){}, 
//                              function(){
//                                vista_devices.text('error al enviar el heading');
//                                });
//    },
//    function(){
//        vista_devices.text('error al obtener el heading');
//    }, 
//    {frequency: 200});
    
    $("#knob").knob({max:175, 
                     'data-linecap':'round',
                    change:function(valor){
                        conector.recibirMensaje(valor.toString());
                    }});    
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

