var PanelDeMandosBT = function(opt){
    $.extend(true, this, opt);
    this.start();
};

PanelDeMandosBT.prototype.start = function(){
    var _this = this;
    this.ui = $("#panel_control_bt");
    this.txt_status = this.ui.find("#status"); 
    
    var clienteHTTP = new NodoClienteHTTP('http://router-vortex.herokuapp.com', 1000);             
    NodoRouter.instancia.conectarBidireccionalmenteCon(clienteHTTP);
    
    //var socket = io.connect('https://router-vortex.herokuapp.com');
    //var adaptador = new NodoConectorSocket(socket);    
    //NodoRouter.instancia.conectarBidireccionalmenteCon(adaptador);
    
    this.mac = '20:13:06:14:05:97';
    var adaptadorArduino = new NodoAdaptadorBluetoothArduino({  mac: _this.mac,
                                                    alConectar:function(){
                                                        _this.txt_status.text('conectado a ' + _this.mac);
                                                    },
                                                    onErrorAlConectar:function(){
                                                        _this.txt_status.text('error al conectar a' + _this.mac);
                                                    }
                                                });
    NodoRouter.instancia.conectarBidireccionalmenteCon(adaptadorArduino);
    
    this.panelKnobs = this.ui.find("#knobs");
    var knob0 = new VortexKnob({id:0});
    knob0.dibujarEn(this.panelKnobs);
    var knob1 = new VortexKnob({id:1});
    knob1.dibujarEn(this.panelKnobs);
    
};