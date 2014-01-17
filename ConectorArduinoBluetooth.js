var ConectorArduinoBluetooth = function(opt){
    $.extend(true, this, opt);
    this.colaDeCaracteres = "";
    this.conectarPorBluetooth();
};

ConectorArduinoBluetooth.prototype.recibirMensaje(mensaje){
    this.colaDeCaracteres += mensaje + '\n';
    this.enviarProximoCaracter();
};

ConectorArduinoBluetooth.prototype.enviarProximoCaracter(){
    var _this = this;
    if(this.colaDeCaracteres == "") return;
    var caracter_a_enviar = this.colaDeCaracteres.shift();
    bluetoothSerial.write(caracter_a_enviar);
    setTimeout(function(){
        _this.enviarProximoCaracter();
    }, 10); 
};
    
    
ConectorArduinoBluetooth.prototype.conectarPorBluetooth = function(){
    var _this = this;
    bluetoothSerial.connect(_this.mac, 
        function(){
            console.log('conectado a ' + _this.mac);
        }, function(){
            console.log('error al conectar a ' + _this.mac + " reintentando en 1 segundo...");
            setTimeout(function(){
                _this.conectarPorBluetooth();
            }, 1000);            
    });
};

        