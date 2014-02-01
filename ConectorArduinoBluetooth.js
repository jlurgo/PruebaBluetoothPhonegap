var ConectorArduinoBluetooth = function(opt){
    $.extend(true, this, opt);
    var _this = this;
    setTimeout(function(){
        _this.conectarPorBluetooth();
    }, 1000);   
};

ConectorArduinoBluetooth.prototype.recibirMensaje = function(mensaje){
    var caracteres = [];
    caracteres.concat(JSON.stringify(mensaje).split(''));
    caracteres.push('\n');
    bluetoothSerial.write(caracteres.join(""));
};

ConectorArduinoBluetooth.prototype.conectarCon = function(otro_nodo){
    this.nodoVecino = otro_nodo;
};

ConectorArduinoBluetooth.prototype.conectarPorBluetooth = function(){
    var _this = this;
    bluetoothSerial.connect(_this.mac, 
        function(){
            console.log('conectado a ' + _this.mac);
            bluetoothSerial.subscribe('\n', function (data) {
                _this.nodoVecino.recibirMensaje(JSON.stringify(data));
            }, function(){
            console.log('error al suscribirse');
            });
            _this.alConectar();
        }, function(){
            console.log('error al conectar a ' + _this.mac + " reintentando en 1 segundo...");
            _this.onErrorAlConectar();
            setTimeout(function(){
                _this.conectarPorBluetooth();
            }, 1000);            
    });
};

        