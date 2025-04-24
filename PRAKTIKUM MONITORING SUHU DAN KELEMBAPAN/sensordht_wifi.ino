//**********************************************************/
//       PRAKTIKUM IOT MONITORING SUHU DAN KELEMBAPAN      //
//    TEKNOLOGI INFORMASI - FAKULTAS SAINS DAN TEKNOLOGI   //
//                  UIN WALLISONGO SEMARANG                //
//*********************************************************//

/**************************************/
// KELOMPOK: 5                        // Nama kelompok
// ANGGOTA:                           // Daftar anggota kelompok
//         1. Adam Achsanul Munzali   //
//         2. M. Ilham Dwi P          //
//         3. M. Azhar Athaya         //
//*************************************//

#include <DHT.h> // Library untuk sensor DHT11
#include <ESP8266WiFi.h> // Library untuk konektivitas WiFi
#include <Wire.h> // Library untuk komunikasi I2C
#include <LiquidCrystal_PCF8574.h> // Library untuk LCD dengan antarmuka I2C
#include <ThingSpeak.h> // Library untuk mengirim data ke ThingSpeak

// Konfigurasi Thingspeak
unsigned long channelID = 2920499; // ID channel ThingSpeak (diubah sesuai kebutuhan)
const char* apiKey = "88U9AS22EGE0D29D"; // API Key ThingSpeak (diubah sesuai kebutuhan)

// Konfigurasi WiFi
const char* ssid = "MA'HAD PUTRA WALISONGO"; // Nama SSID WiFi
const char* password = "22sampai26wisuda"; // Password WiFi

// Konfigurasi DHT
#define DHTPIN D4 // Pin GPIO NodeMCU tempat sensor DHT11 terhubung
#define DHTTYPE DHT11 // Jenis sensor DHT yang digunakan
DHT dht(DHTPIN, DHTTYPE); // Inisialisasi objek DHT

// Konfigurasi LCD I2C
LiquidCrystal_PCF8574 lcd(0x27); // Alamat I2C LCD (default 0x27)
WiFiClient client; // Inisialisasi klien WiFi

// Fungsi untuk menampilkan informasi kelompok pada LCD
void displayInfo() {
  lcd.clear(); // Membersihkan layar LCD
  lcd.setCursor(0, 0); // Menempatkan kursor di baris 1, kolom 1
  lcd.print("UIN Walisongo"); // Menampilkan teks
  lcd.setCursor(0, 1); // Menempatkan kursor di baris 2, kolom 1
  lcd.print("Semarang"); // Menampilkan teks
  delay(2000); // Menunda selama 2 detik

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PRODI TI.");
  lcd.setCursor(0, 1);
  lcd.print("FST");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IOT");
  lcd.setCursor(0, 1);
  lcd.print("SK");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kelompok: 5");
  lcd.setCursor(0, 1);
  lcd.print("Azhar,Adam,Ilham");
  delay(3000);
}

void setup() {
  Serial.begin(9600); // Memulai komunikasi serial dengan baud rate 9600
  delay(10); // Menunda selama 10 milidetik

  dht.begin(); // Inisialisasi sensor DHT

  lcd.begin(16, 2); // Inisialisasi LCD dengan ukuran 16x2
  lcd.setBacklight(255); // Menghidupkan backlight LCD
  lcd.clear(); // Membersihkan layar LCD

  displayInfo(); // Menampilkan informasi kelompok pada LCD

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi"); // Menampilkan status koneksi WiFi
  WiFi.begin(ssid, password); // Menghubungkan ke WiFi dengan SSID dan password

  while (WiFi.status() != WL_CONNECTED) { // Menunggu hingga WiFi terhubung
    delay(500); // Menunda 500 milidetik
    Serial.print("."); // Menampilkan tanda titik sebagai indikator proses
  }

  Serial.println();
  Serial.println("WiFi Connected!"); // Menampilkan status koneksi berhasil
  Serial.print("IP Address: "); // Menampilkan alamat IP perangkat
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected"); // Menampilkan pesan koneksi berhasil di LCD
  delay(2000);
  lcd.clear();

  ThingSpeak.begin(client); // Inisialisasi ThingSpeak
}

void loop() {
  float h = dht.readHumidity(); // Membaca kelembapan dari sensor DHT11
  float t = dht.readTemperature(); // Membaca suhu dari sensor DHT11

  if (isnan(h) || isnan(t)) { // Memeriksa jika pembacaan gagal
    Serial.println("Sensor DHT11 gagal membaca data!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!"); // Menampilkan pesan error di LCD
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C, Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C    ");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(h);
  lcd.print(" %    ");

  ThingSpeak.setField(1, t); // Mengirim suhu ke Field1 ThingSpeak
  ThingSpeak.setField(2, h); // Mengirim kelembapan ke Field2 ThingSpeak

  int response = ThingSpeak.writeFields(channelID, apiKey); // Mengirim data ke ThingSpeak
  if (response == 200) {
    Serial.println("Data berhasil dikirim ke Thingspeak!");
  } else {
    Serial.print("Error mengirim data ke Thingspeak: ");
    Serial.println(response);
  }

  delay(20000); // Menunda pengulangan loop selama 20 detik
}