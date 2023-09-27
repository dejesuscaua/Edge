#include "Arduino.h"

#include "WiFi.h"

#include "HTTPClient.h"

 

 

#define ldr 34

#define analogIn 35

#define umid 32

 

int vldr = 0;

 

 

const char* ssid = "André Luis";

const char* password = "ainda2742";

 

 

char serverAddress[] = "https://api.tago.io/data";  // TagoIO address

char contentHeader[] = "application/json";

char tokenHeader[]   = "05450f65-93f6-4391-a9f7-39e189283366"; // TagoIO Token

 

 

void setup()

{

  pinMode(ldr, INPUT);

  pinMode(analogIn, INPUT);

  pinMode(umid, INPUT);

 

  Serial.begin(9600);

 

  // Conectar ao Wi-Fi

  WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {

    delay(1000);

    Serial.println("Conectando ao WiFi...");

  }

  Serial.println("Conectado ao Wi-Fi");

}


 

void loop()

{

  vldr = analogRead(ldr);

  Serial.print("LDR valor: ");

  Serial.println(vldr);

 

  //---------------- DHT11-------------//

  int RawValue = analogRead(analogIn);

  double tempC = (((RawValue / 1023.0) * 5000) - 500) * 0.1; // 500 is the offset

  Serial.print("Temperature in C = ");

  Serial.println(tempC, 1);

  Serial.println("");

  int humiditysensorOutput = analogRead(umid);

  int humidade = map(humiditysensorOutput, 0, 1023, 0, 100);

  Serial.print("Humidity: ");

  Serial.print(humidade);

  Serial.println("%");

  Serial.println("");

 

  // LDR

  if (vldr <= 103) // BAIXO LDR

  {

    Serial.println("Ambiente OK");

    Serial.println("");

    vldr = analogRead(ldr);


  }

 

  // MEIO TERMO

  if (vldr >= 104 && vldr <= 206)

  {

    Serial.println("Ambiente a meia luz");

    Serial.println("");

    vldr = analogRead(ldr);


  }

 

  // TEMP OK

  if (tempC >= 10 && tempC <= 15)

  {

    Serial.print("Temperature OK. Temp = ");

    Serial.println(tempC, 1);

    Serial.println("");

    RawValue = analogRead(analogIn);

    tempC = (((RawValue / 1023.0) * 5000) - 500) * 0.1; // 500 is the offset


  }

 

  // UMIDADE OK

  if (humidade >= 60 && humidade <= 80)

  {

    Serial.print("Humidity OK. Humidity: ");

    Serial.print(humidade);

    Serial.println("%");

    Serial.println("");

    humiditysensorOutput = analogRead(umid);

    humidade = map(humiditysensorOutput, 0, 1023, 0, 100);


  }

 

if(vldr >= 207)

  { // LDR ALTO

    Serial.println("Ambiente muito claro");

    Serial.println("");

    vldr = analogRead(ldr);

  }

 

  // TEMP BAIXA

if(tempC <= 9)

  {

    Serial.print("Temperature Baixa. Temp = ");

    Serial.println(tempC, 1);

    RawValue = analogRead(analogIn);

    tempC = (((RawValue / 1023.0) * 5000) - 500) * 0.1; // 500 is the offset


  }

 

  // TEMP ALTA

if(tempC >= 16)

  {

    Serial.print("Temperature Alta. Temp = ");

    Serial.println(tempC, 1);

    Serial.println("");

    RawValue = analogRead(analogIn);

    tempC = (((RawValue / 1023.0) * 5000) - 500) * 0.1; // 500 is the offset

  }

 

  // UMIDADE BAIXA

if(humidade <= 60)

  {

    Serial.print("Humidity Baixa. Humidity: ");

    Serial.print(humidade);

    Serial.println("%");

    Serial.println("");

    humiditysensorOutput = analogRead(umid);

    humidade = map(humiditysensorOutput, 0, 1023, 0, 100);


  }

 

  // UMIDADE ALTA

if(humidade >= 80)

  {

    Serial.print("Humidity Alta. Humidity: ");

    Serial.print(humidade);

    Serial.println("%");

    Serial.println("");

    humiditysensorOutput = analogRead(umid);

    humidade = map(humiditysensorOutput, 0, 1023, 0, 100);


  }
if (millis() % 5000 == 0)

  {

    sendData(tempC, humidade, vldr);

  }
 

}

void sendData(double tempC, int humidade, int vldr)

{

  if (WiFi.status() == WL_CONNECTED)

  {

    HTTPClient http;

    String postData = "{\n\t\"temperature\": " + String(tempC, 2) + ",\n\t\"humidity\": " + String(humidade) + ",\n\t\"ldrValue\": " + String(vldr) + "\n}";


    Serial.println("Enviando dados para o servidor...");

    Serial.println(postData);

 

    http.begin(serverAddress);

    http.addHeader("Content-Type", "application/json");

 

    int httpResponseCode = http.POST(postData);

 

    if (httpResponseCode > 0)

    {

      String response = http.getString();

      Serial.print("Resposta do servidor: ");

      Serial.println(response);

    }

    else

    {

      Serial.print("Erro na requisição HTTP: ");

      Serial.println(httpResponseCode);

    }

 

    http.end();

  }

  else

  {

    Serial.println("Não conectado ao Wi-Fi");

  }

  delay(5000); // Espera 5 segundos antes de enviar novamente

}
