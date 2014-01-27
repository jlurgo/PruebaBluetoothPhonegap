var ConectorArduinoBluetooth = function(opt){
    $.extend(true, this, opt);
    this.colaDeCaracteres = [];
    var _this = this;
    setTimeout(function(){
        _this.conectarPorBluetooth();
    }, 1000);   
};

ConectorArduinoBluetooth.prototype.recibirMensaje = function(mensaje){
    this.colaDeCaracteres = this.colaDeCaracteres.concat(JSON.stringify(mensaje).split(''));
    this.colaDeCaracteres.push('\n');
    this.enviarProximoCaracter();
};

ConectorArduinoBluetooth.prototype.enviarProximoCaracter = function(){
    var _this = this;
    if(this.colaDeCaracteres.length==0) return;
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
            _this.alConectar();
        }, function(){
            console.log('error al conectar a ' + _this.mac + " reintentando en 1 segundo...");
            _this.onErrorAlConectar();
            setTimeout(function(){
                _this.conectarPorBluetooth();
            }, 1000);            
    });
};

        