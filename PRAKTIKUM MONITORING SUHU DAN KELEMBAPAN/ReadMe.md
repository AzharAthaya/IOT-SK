IOT-Monitor-Suhu-dan-Kelembapan
Hardware :
1. Esp8266
2. LCD 16x2 l2C
3. Sensor DHT11

Software dan Tools :
1. Arduino IDE
2. Library:
- ESP8266WiFi.h
- DHT.h
- ThingSpeak.h
3. ThingSpeak (akun dan channel)

Circuit:
![image](https://github.com/user-attachments/assets/7f796fdb-c4e7-4862-afce-2963a54cb482)

Tabel Diagram
<img width="422" alt="image" src="https://github.com/user-attachments/assets/acd86fb5-cf4d-4d9b-af1f-a6c78d1d3e5c" />

1. Buat Channel di ThingSpeak https://thingspeak.mathworks.com/
	- Daftar/login ke ThingSpeak
	- Buat channel baru
	- Aktifkan field 1 dan field 2 untuk suhu dan kelembapan
	- Salin Write API Key
	- Instal Library di Arduino IDE
2. Melalui Library Manager:
	- DHT sensor library by Adafruit
	- ThingSpeak library
	- ESP8266 board melalui Board Manager
3. Upload Kode ke ESP8266

HASIL:
![image](https://github.com/user-attachments/assets/ead57c71-3ca8-45ea-b749-1d8544d0082c)

Grafik di ThinkSpeak
![image](https://github.com/user-attachments/assets/647cdb75-c12c-44c6-a379-efd3ae407051)




