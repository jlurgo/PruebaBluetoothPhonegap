#define MAX_PUERTOS 5
#define MAX_PEDIDOS 5

typedef struct
{
    aJsonObject *filtro;
	void (*callback)(aJsonObject*);
} Pedido; 
class Vortex {
	private:
	String caracteres_recibidos[MAX_PUERTOS];
        int cant_pedidos;
        Pedido pedidos[MAX_PEDIDOS];
        int cant_puertos;
        Stream* puertos[MAX_PUERTOS];
        
	public:	
       
	void Setup() {
          cant_pedidos=0;
          cant_puertos=0;
	}

        void PedirMensajes(aJsonObject *filtro, void (*callback)(aJsonObject*)){
          Pedido un_pedido = {filtro, callback};
          pedidos[cant_pedidos] = un_pedido;
          cant_pedidos++;
        }
        
        void AgregarPuerto(Stream* un_puerto){
          puertos[cant_puertos] = un_puerto;
          cant_puertos++;
        }
        
	void EnviarCharATodosLosPuertosMenosA(char caracter, int id_puerto){
		for(int i=0;i<cant_puertos;i++){
			if(i!=id_puerto)puertos[i]->write(caracter);
		} 
	}

	void EnviarMensajeATodosLosPuertosMenosA(aJsonObject* mensaje, int id_puerto_excluido){
		char* mensaje_str = aJson.print(mensaje);
		strcat(mensaje_str, "\n");
		for(int i=0;i<cant_puertos;i++){
			if(i!=id_puerto_excluido)puertos[i]->print(mensaje_str);
		} 
		free(mensaje_str);
	}

	void OnCaracterRecibido(char caracter, int id_puerto){  
		if (caracter != '\r' && caracter != '\n') {
		  caracteres_recibidos[id_puerto] += caracter;
		}
		else { //caracter == '\r' 
			aJsonObject* mensaje = aJson.parse(&caracteres_recibidos[id_puerto][0]);
			if (mensaje != NULL) {
				RecibirMensaje(mensaje, id_puerto); 
			} else{
				Serial.println(F("Error al parsear mensaje!")); 
			}		
			caracteres_recibidos[id_puerto] = "";  
			aJson.deleteItem(mensaje); 		
		}  
	}

	void RecibirMensaje(aJsonObject* mensaje, int id_puerto){   
		EnviarMensajeATodosLosPuertosMenosA(mensaje, id_puerto);

		for(int i=0; i<cant_pedidos; i++){
			if(ElMensajePasaElFiltro(mensaje, pedidos[i].filtro)){
				pedidos[i].callback(mensaje);
			}
		}
	}

	bool ElMensajePasaElFiltro(aJsonObject* mensaje, aJsonObject* filtro){
		if (filtro == NULL) {
			Serial.print(F("Error en el filtro")); 
			return false;
		}  
		aJsonObject* obj_tipo_filtro = aJson.getObjectItem(filtro , "tipo");
		String tipo_filtro = obj_tipo_filtro->valuestring;
		if(tipo_filtro=="EQ"){
			aJsonObject* obj_clave_filtro = aJson.getObjectItem(filtro , "clave");
			char* clave_filtro = obj_clave_filtro->valuestring;
			aJsonObject* obj_valor_filtro = aJson.getObjectItem(filtro , "valor");
		
			aJsonObject* obj_valor = aJson.getObjectItem(mensaje , clave_filtro);
			if(obj_valor->type==aJson_Int && obj_valor_filtro->type==aJson_Int){
				return(obj_valor->valueint == obj_valor_filtro->valueint);
			}	
		}
		return false;
	}

	void Tick(){
		for(int i=0;i<cant_puertos;i++){
			if(puertos[i]->available()){
				char caracter = puertos[i]->read();
				OnCaracterRecibido(caracter, i);
			}
		}  
	}
} vx;
