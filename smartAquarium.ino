#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Servo.h>

// Pin definisi
#define DHTPIN 2         // Pin data DHT22
#define DHTTYPE DHT22    // Tipe DHT22
#define PH_PIN A0        // Pin analog untuk pH sensor
#define FEEDER_PIN D4    // Pin untuk motor servo

// Inisialisasi sensor dan servo
DHT dht(DHTPIN, DHTTYPE);
Servo feederServo;

// Variabel waktu
unsigned long lastFeedTime = 0;
const unsigned long feedInterval = 2 * 60 * 60 * 1000; // 2 jam dalam milidetik

void setup() {
  Serial.begin(115200);
  dht.begin();
  feederServo.attach(FEEDER_PIN);

  // Posisi awal servo (tertutup)
  feederServo.write(0);
}

void loop() {
  // Membaca data dari sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int phValue = analogRead(PH_PIN);

  // Konversi nilai pH analog ke pH aktual (asumsi nilai ADC 0-1023 ke pH 0-14)
  float voltage = phValue * (3.3 / 1023.0);
  float ph = 7 + ((2.5 - voltage) / 0.18);

  // Tampilkan hasil pembacaan sensor di Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C\t");
  Serial.print("pH: ");
  Serial.println(ph);

  // Pemberian makan otomatis setiap 2 jam
  if (millis() - lastFeedTime > feedInterval) {
    feedFish();
    lastFeedTime = millis();
  }

  delay(2000); // Delay 2 detik sebelum pembacaan berikutnya
}

void feedFish() {
  Serial.println("Feeding fish...");
  feederServo.write(90);  // Buka mekanisme pemberian makan
  delay(2000);            // Tunggu 2 detik
  feederServo.write(0);   // Tutup mekanisme pemberian makan
}