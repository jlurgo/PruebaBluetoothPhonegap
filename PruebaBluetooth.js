var onDeviceReady = function() {     
    var txt_status = $("#status"); 
    var mac = '20:13:06:14:05:97';
    var conector = new ConectorArduinoBluetooth({mac:mac,
                                                alConectar:function(){
                                                    txt_status.text('conectado a ' + mac);
                                                },
                                                onErrorAlConectar:function(){
                                                    txt_status.text('error al conectar a' + mac);
                                                }});
    
//    btnSuscribirse.click(function(){
//        // the success callback is called whenever data is received
//        bluetoothSerial.subscribe('\n', function (data) {
//            vista_mensajes.text(vista_mensajes.text() + JSON.stringify(data));
//        }, function(){
//            vista_devices.text('error al suscribirse');
//        });
//    });
    
    $("#knob").knob({max:175, 
                     'data-linecap':'round',
                    change:function(valor){                        
                        conector.recibirMensaje({
                            tipoDeMensaje:'control_servo',
                            id_servo:1,
                            angulo:valor.toString()
                        });
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

