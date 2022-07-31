#define trigPin 8
#define echoPin 7
#include "DHT.h" 
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int relay = 2;
int ledGreen = 6; 
int ledRed = 3;

int output_value = 0;
int in_percent = 0;
int sensorTanah = A0;

float t;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Sistem Monitoring Tanaman Hias");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  dht.begin();
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  //menghitung tinggi air
  long duration, distance, ketinggianAir;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 28.5;
  ketinggianAir = 18.5-distance;
  Serial.println("Tinggi Air Tangki = ");
  Serial.print(ketinggianAir);             //menampilkan nilai tinggi air di serial monitor
  Serial.println(" cm");
  

  //menghitung kelembaban tanah
  output_value = analogRead(sensorTanah); //pembacaan data analog pada sensor
  in_percent = map(output_value, 1023, 0, 0, 100);  //inisiasi data analog menjadi persen
  if (in_percent <= 60){               //pengkondisian pada relay
    digitalWrite(ledGreen, LOW);
    digitalWrite(relay, HIGH);
  } if (in_percent > 60){               
    digitalWrite(ledGreen, HIGH);
    digitalWrite(relay, LOW);
  }
  printOnSerial();          //menampilkan kelembaban tanah di serial monitor
  
  //menghitung suhu
  t = dht.readTemperature();   //inisiasi nilai suhu
  Serial.println("Suhu = ");
  Serial.print(t);             //menampilkan nilai suhu di serial monitor
  Serial.println(" *C");
  
  //Pengkondisian suhu
  if (t <= 21){      
    digitalWrite(ledRed, LOW);
    Serial.println("Suhu Terlalu Dingin");
  } else if (t <= 29) {
    digitalWrite(ledRed, HIGH);
    Serial.println("Suhu Stabil");
  } else if (t > 29) {
    digitalWrite(ledRed, LOW);
    Serial.println("Suhu Terlalu Panas");
  }
  Serial.println();
  delay(2000);

  // Menampilkan suhu dan kelembaban pada serial monitor
  String data = String(t) + "#" + String (ketinggianAir)+ "#" + String(in_percent);
  Serial.println(data);
  Serial.println();
}

void printOnSerial(){
  Serial.println("Kelembaban Tanah = ");
  Serial.print(in_percent);
  Serial.println(" %");
}
