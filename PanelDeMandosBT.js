var PanelDeMandosBT = function(opt){
    $.extend(true, this, opt);
    this.start();
};

PanelDeMandosBT.prototype.start = function(){
    var _this = this;
    this.txt_status = $("#status"); 
    this.mac = '20:13:06:14:05:97';
    this.conector = new ConectorArduinoBluetooth({  mac: _this.mac,
                                                    alConectar:function(){
                                                        _this.txt_status.text('conectado a ' + _this.mac);
                                                    },
                                                    onErrorAlConectar:function(){
                                                        _this.txt_status.text('error al conectar a' + _this.mac);
                                                    }
                                                });
    
    this.conector.conectarCon({
        recibirMensaje:function(mensaje){
            if(mensaje.tipoDeMensaje=='control_servo'){     
                _this.envioHabilitado = false;
                _this.knobs[mensaje.id_servo].val(mensaje.angulo).trigger('change');
                _this.envioDesHabilitado = true;
            }            
        }
    });
    
    this.knobs = [];
    this.knobs.push($("#knob1"));
    this.knobs.push($("#knob2"));
    this.knobs[0].knob({max:175, 
        angleArc:180, 
        angleOffset:90, 
        skin:"tron",
        linecap:"round",
        change:function(valor){  
            if(_this.envioDesHabilitado) return;
            _this.conector.recibirMensaje({
                tipoDeMensaje:'control_servo',
                id_servo:0,
                angulo:valor
            });
        }
    });   
    this.knobs[1].knob({max:175, 
        angleArc:180, 
        angleOffset:90, 
        skin:"tron",
        linecap:"round",
        change:function(valor){  
            if(_this.envioDesHabilitado) return;
            _this.conector.recibirMensaje({
                tipoDeMensaje:'control_servo',
                id_servo:1,
                angulo:valor
            });
        }
    });   
    
};