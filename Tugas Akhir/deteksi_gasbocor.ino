//**********************************************************/
//       PRAKTIKUM IOT DETEKSI KEBOCORAN GAS DAN API       //
//              DENGAN PEMANFAATAN TELEGRAM                //
//    TEKNOLOGI INFORMASI - FAKULTAS SAINS DAN TEKNOLOGI   //
//                  UIN WALLISONGO SEMARANG                //
//**********************************************************/

/**************************************/
// KELOMPOK: 5                        //
// ANGGOTA:                           //
//         1. Adam Achsanul Munzali   //
//         2. M. Ilham Dwi P          //
//         3. M. Azhar Athaya         //
//**************************************/

#include <Wire.h>  
#include <LiquidCrystal_PCF8574.h>  
#include <CTBot.h>  
#include <ESP8266WiFi.h>  

CTBot myBot;  
LiquidCrystal_PCF8574 lcd(0x27);  

String ssid = "UIN-Walisongo";  
String pass = "";  
String token = "7384406602:AAFed-0M1TvVuDSJYE8MPKymmn7sQa2wDr0";  
const int chat_id = 1511936347;  

const int smokeA0 = A0;  
const int flamePin = D4;  
const int flameDetected = LOW;  
const int sensorThres = 500;  

const int ledPin = D0;  
const int buzzerPin = D3;  

bool alreadyAlertedGas = false;  
bool alreadyAlertedFire = false;  

void displayInfo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Deteksi");
  lcd.setCursor(0, 1);
  lcd.print("Gas dan Api");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UIN Walisongo");
  lcd.setCursor(0, 1);
  lcd.print("Semarang");
  delay(2000);

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
  Serial.begin(115200);
  delay(1000);

  lcd.begin(16, 2);
  lcd.setBacklight(255);

  // Tampilkan info kelompok
  displayInfo();

  lcd.setCursor(0, 0);
  lcd.print("Menghubungkan...");

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(buzzerPin, OUTPUT);
  noTone(buzzerPin);
  pinMode(flamePin, INPUT);

  WiFi.begin(ssid.c_str(), pass.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wi-Fi Terkoneksi!");
    lcd.setCursor(0, 0);
    lcd.print("Wi-Fi Terkoneksi");
  } else {
    Serial.println("Wi-Fi Gagal!");
    lcd.setCursor(0, 0);
    lcd.print("Wi-Fi Gagal");
    return;
  }

  myBot.setTelegramToken(token);
  if (myBot.testConnection()) {
    Serial.println("Bot Terkoneksi!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bot Aktif");
    myBot.sendMessage(chat_id, "🤖 Bot Deteksi Gas & Api Aktif!");
  } else {
    Serial.println("Koneksi Bot Gagal!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bot Gagal!");
    return;
  }

  delay(2000);
  lcd.clear();
}

void loop() {
  int gasValue = analogRead(smokeA0);
  int flameValue = digitalRead(flamePin);

  Serial.print("Nilai MQ-2: ");
  Serial.println(gasValue);
  Serial.print("Status Api: ");
  Serial.println(flameValue == flameDetected ? "Terdeteksi" : "Tidak");

  bool gasDetected = gasValue > sensorThres;
  bool fireDetected = (flameValue == flameDetected);

  String status = "AMAN";

  if (gasDetected && fireDetected) {
    status = "AWASS!!";
    if (!alreadyAlertedGas || !alreadyAlertedFire) {
      myBot.sendMessage(chat_id, "🚨🔥 Gas & Api terdeteksi bersamaan! SEGERA CEK LOKASI!");
      alreadyAlertedGas = true;
      alreadyAlertedFire = true;
    }
    tone(buzzerPin, 3000);
    digitalWrite(ledPin, HIGH);
  }
  else if (gasDetected) {
    status = "GAS BOCOR!";
    if (!alreadyAlertedGas) {
      myBot.sendMessage(chat_id, "🚨 Gas bocor terdeteksi! Segera cek lokasi!");
      alreadyAlertedGas = true;
    }
    alreadyAlertedFire = false;
    tone(buzzerPin, 2000);
    digitalWrite(ledPin, HIGH);
  }
  else if (fireDetected) {
    status = "API!!!";
    if (!alreadyAlertedFire) {
      myBot.sendMessage(chat_id, "🔥 Api terdeteksi! Segera cek lokasi!");
      alreadyAlertedFire = true;
    }
    alreadyAlertedGas = false;
    tone(buzzerPin, 4000);
    digitalWrite(ledPin, HIGH);
  }
  else {
    status = "AMAN";
    alreadyAlertedGas = false;
    alreadyAlertedFire = false;
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
  }

  lcd.setCursor(0, 0);
  lcd.print("MQ-2: ");
  lcd.print(gasValue);
  lcd.print("     ");

  lcd.setCursor(0, 1);
  lcd.print("Status:");
  lcd.print(status);
  lcd.print("     ");

  delay(1500);
}
