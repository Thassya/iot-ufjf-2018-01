// bibliotecas do WiFi e servidor web
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
// credenciais da rede sem fio
//const char* ssid = "iot";
//const char* password = "netdascoisas#";
const char* ssid = "Suecia806";
const char* password = "Qazplm27";
bool ledOn = false;

// porta 80 para HTTP
ESP8266WebServer server(80);

// conteudo da pagina web
String page = "";

// pino de saída onde p LED esta ligado
int LEDPin = 13;

void setup(void){
  // conteudo da pagina web HTTP
  page = "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
          <h1>NodeMCU Web Server </h1> \
          <h3>DCC091 - IoT - 2018.1 - Grupo 6</h3> \
          <p>Status: <b id=\"status\">Loading...</b></p> \
          <p><button onclick=\"lightOn()\">ON</button>&nbsp;<button onclick=\"lightOff()\">OFF</button></p>\
          <script> \
                const statusEl = document.getElementById('status'); \
                const lightOn = () => fetch('/on'); \
                const lightOff = () => fetch('/off'); \
                const updateStatus = () => fetch('/status').then(data => data.json()).then(status => {statusEl.innerHTML = status ? 'On' : 'Off'; setTimeout(updateStatus, 500); }); \
                updateStatus(); \
          </script>";
  //informando o pino de saida e deixando-o desligado
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); // conecta ao wifi
  Serial.println("");
 
  // esperando pela conexao
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado na rede -> ");
  Serial.println(ssid);
  Serial.print("O IP do servidor web HTTP NodeMCU -> ");
  Serial.println(WiFi.localIP());
  
  server.on("/", [](){
    server.send(200, "text/html", page);
  });
  server.on("/LEDOn", [](){
    server.send(200, "text/html", page);
    digitalWrite(LEDPin, HIGH);
    ledOn = true;
    //delay(100);
  });
  server.on("/LEDOff", [](){
    server.send(200, "text/html", page);
    digitalWrite(LEDPin, LOW);
    ledOn = false;
    //delay(100);
  });
  server.on("/status", [](){
    server.send(200, "text/json", ledOn ? "true" : "false");
  });
  server.on("/on", [](){
    digitalWrite(LEDPin, HIGH);
    ledOn = true;
    server.send(200, "text/json", ledOn ? "true" : "false");
  });
  server.on("/off", [](){
    digitalWrite(LEDPin, LOW);
    ledOn = false;
    server.send(200, "text/json", ledOn ? "true" : "false");
  });
  server.begin();
  Serial.println("Servidor Web Iniciado!");
  Serial.println("Abra seu navegador e digite o endereco IP:");
}
 
void loop(void){
  // atende ao cliente http
  server.handleClient();
}
