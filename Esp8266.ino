#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial (D1, D2);
boolean startSystem = true;

//setup firebase
#define FIREBASE_HOST "https://realtime-sensor-ab125-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "JJUhqBWND5lEr1dmsPFXI3VAjQcRHAQ3XwzELAl8"

// mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;

unsigned long previousMillis = 0;
const long interval = 2000;

String arrData[3];
float suhu, ketinggianAir, tanah;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);

  // Koneksi ke Wifi
  //WiFiManager wifiManager;
  //wifiManager.autoConnect("SiHias-AP");
  //Serial.println("Connected......");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    //baca data yang diterima
    String data = "";
    while(mySerial.available()>0)
    {
      data += char(mySerial.read());
    }
    data.trim();

    //parsing data dari arduino
    if(data != "")
    {
      int index = 0;
      for(int i=0; i<= data.length(); i++)
      {
        char delimiter = '#';
        if(data[i] != delimiter)
          arrData[index] += data[i];
        else
          index++;  
      }

      //memeriksa data
      if(index == 2)
      {
        //tampilan serial monitor
        Serial.println("Suhu : " + arrData[0]); //tampilkan nilai suhu     
        Serial.println("Tinggi air : " + arrData[1]); //tampilkan nilai debit air
        Serial.println("Kelembaban Tanah : " + arrData[2]); //tampilkan nilai kelembaban tanah
        Serial.println();
      }
      //variable data
      suhu      = arrData[0].toFloat();
      ketinggianAir  = arrData[1].toFloat();
      tanah     = arrData[2].toFloat();
      
      arrData[0] = "";
      arrData[1] = "";
      arrData[2] = "";
    }

    //ambil data arduino
    mySerial.println("ya");

    if (startSystem) {
    WiFiManager wm;    

    wm.setTimeout(500);

    if (!wm.startConfigPortal("SiHias-AP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    startSystem = false;
  }
    if(startSystem == false){
      
    }
    
  }
   //kirim data ke firebase
    if (Firebase.setFloat(firebaseData, "/suhu", suhu)){
      Serial.println("Suhu terkirim");
    } else{
      Serial.println("Suhu tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    } 

   if (Firebase.setFloat(firebaseData, "/tinggiAir", ketinggianAir)){
      Serial.println("Tinggi air terkirim");
      Serial.println();
    } else{
      Serial.println("Tinggi Air tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }
    
   if (Firebase.setFloat(firebaseData, "/kelembabanTanah", tanah)){
      Serial.println("Kelembaban Tanah terkirim");
      Serial.println();
    } else{
      Serial.println("Kelembaban Tanah tidak terkirim");
      Serial.println("Karena: " + firebaseData.errorReason());
    }
}
