#ifndef _MODECONFIGDATA_
#define _MODECONFIGDATA_

void configServerApSta();
void loopServerConfig();
bool checkInfo();

extern void deserializeDataJson(String dados_recebidos);
extern void writeNVS();

extern const char* assid;
extern const char* asecret;

extern Data data_config;

extern WiFiServer server;
extern WebSocketServer webSocketServer;

void configServerApSta() {
  Serial.println("Iniciando modo de configuração!");
  WiFi.mode(WIFI_AP_STA);

  Serial.println("Criando ponto de acesso....");
  WiFi.softAP(assid,asecret,7,0,5);

  Serial.print("Endereço IP:\t");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

bool checkData(String data) {
  deserializeDataJson(data);

  bool status = checkInfo();

  Serial.println(status);
  if(status) {
    webSocketServer.sendData("ok");
  }
  else {
    webSocketServer.sendData("erro");
  }

  Serial.print("Dado recebido: ");
  Serial.println(data);

  return status;
}


void loopServerConfig() {
  WiFiClient client = server.available();
  bool configOk = false;
 
  if (client.connected() && webSocketServer.handshake(client)) {

    Serial.println("Cliente Conectado!");
    String data;
 
    while (client.connected()) {
 
      data = webSocketServer.getData();

      if(data.length() > 0)
        configOk = checkData(data);
    
      delay(10);
   }

   Serial.println("Cliente Desconectado");
   delay(100);
  }

  if(configOk) {
    Serial.println("Escrevendo na NVS...");
    WiFi.disconnect();
    writeNVS();
    ESP.restart();
  }
}

bool checkInfo() {

  Serial.print("connectando em... ");
  Serial.println(data_config.ssid);

  WiFi.begin(data_config.ssid.c_str(), data_config.password.c_str());

  int cont = 0;
  int MAX = 20;

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if(cont == MAX)
      return false;

    cont++;
  }

  Serial.println("\nÉ possível concectar............");

  return true;
}


#endif