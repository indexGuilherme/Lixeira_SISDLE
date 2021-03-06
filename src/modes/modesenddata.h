#ifndef _MODESENDDATA_
#define _MODESENDDATA_

extern void readNVS();
extern void checkButton();
extern void startSensor();
extern int readSensor();
extern float convertMMinPorcent(int distance);
extern void startTimer();
extern void getTimer();

extern Data data_config;

void configStation();
void reconnectWiFi();
void funcLoopEnviarInfo();

void configStation() {
  readNVS();
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect (true);

  WiFi.begin(data_config.ssid.c_str(), data_config.password.c_str());

  startSensor();
  startTimer();
  reconnectWiFi();
}

void reconnectWiFi() {
  if(WiFi.status() == WL_CONNECTED) 
    return;

  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    checkButton();
    Serial.println("Connectando a Rede WiFi..");
    WiFi.reconnect();
  }

  Serial.println("Conectado!");
  Serial.println(WiFi.localIP());
}

void loopSendInfo() {
  reconnectWiFi();

  int distance = readSensor();
  float porcent =  convertMMinPorcent(distance);

  Serial.printf("Distance: %d mm\n", distance);
  Serial.printf("Porcent: %f %%\n", porcent);

  getTimer();
  
  delay(1000);
}

#endif