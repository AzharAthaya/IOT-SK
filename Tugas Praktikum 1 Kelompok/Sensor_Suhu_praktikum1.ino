/*/////////////////////////////////////////////////////
\\\      PROGRAM STUDI TEKNOLOGI INFORMASI         ///
 \\\         FAKULTAS SAINS DAN TEKNOLOGI         ///
  \\\    UNIVERSITAS ISLAM NEGERI WALISONGO      ///
   \\\///////////////////////////////////////////*/
//       MONITORING SUHU DAN KELEMBABAN         //
//////////////////////////////////////////////////
//              Anggota Kelompok:               //
//   1. Adam Achsanul Munzali 2208096055        //
//   2. Muhammad Ilham D. P 2208096065          //
//   3. Muhammad Azhar A. 2208096076            //
//////////////////////////////////////////////////

// Mengimpor library DHT untuk membaca sensor DHT11
#include "DHT.h"

// Mendefinisikan pin data sensor DHT11 terhubung ke pin GPIO 21
#define DHTPIN 21

// Menentukan tipe sensor yang digunakan (DHT11)
#define DHTTYPE DHT11

// Membuat objek dht dari class DHT dengan pin dan tipe sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Memulai komunikasi serial dengan kecepatan 115200 bps
  Serial.begin(115200);

  // Menampilkan pesan awal di Serial Monitor
  Serial.println("DHT11 Sensor Monitoring");

  // Menginisialisasi sensor DHT11
  dht.begin();
}

void loop() {
  // Membaca kelembapan dari sensor
  float h = dht.readHumidity();

  // Membaca suhu dalam derajat Celsius dari sensor
  float t = dht.readTemperature();

  // Mengecek apakah pembacaan gagal (hasilnya NaN / Not a Number)
  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca dari sensor DHT11!");
    return; // Keluar dari loop saat ini dan ulangi lagi nanti
  }

  // Menampilkan kelembapan dan suhu ke Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C");

  // Menunggu 2 detik sebelum melakukan pembacaan ulang
  delay(2000);
}
