#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <SoftwareSerial.h>  
#include <DFPlayer_Mini_Mp3.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD dengan alamat I2C 0x27, lebar 16 karakter, 2 baris
SoftwareSerial mySerial(2, 3);
RTC_DS1307 rtc;

char namaHari[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
char teksBerjalan[] = "POLTEKKES";

void setup()
{
  Serial.begin(9600);
  
  // Inisialisasi LCD
  lcd.init();        // Menggunakan lcd.init() sebagai pengganti lcd.begin()
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Inisialisasi...");
  
  if (!rtc.begin()) {
    Serial.println("RTC TIDAK TERBACA");
    lcd.setCursor(0, 1);
    lcd.print("RTC TIDAK TERBACA");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    lcd.setCursor(0, 1);
    lcd.print("RTC NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));    
  }

  mySerial.begin(9600);
  mp3_set_serial(mySerial);
  delay(10);
  mp3_set_volume(50);
  delay(10);
 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistem Siap");
  delay(2000); // Memberikan waktu untuk membaca pesan di LCD
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();

  // Display date and time
  lcd.setCursor(0, 0);
  lcd.print(String(namaHari[now.dayOfTheWeek()]) + " " + String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()));

  lcd.setCursor(0, 1);
  lcd.print(String(now.hour()) + ":" + (now.minute() < 10 ? "0" : "") + String(now.minute()) + ":" + (now.second() < 10 ? "0" : "") + String(now.second()));


  // Debug output
  Serial.print("Current Time: ");
  Serial.print(namaHari[now.dayOfTheWeek()]);
  Serial.print(',');    
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(" ");   
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // Alarm Schedule Display
  if ((now.hour() == 06 && now.minute() == 30 ) ||  // Morning Prayer
      (now.hour() == 21 && now.minute() == 30))  // Noon Prayer
  { 
    Serial.println("Triggering audio playback...");
    lcd.clear();
    mp3_play();
    int panjangTeks = strlen(teksBerjalan);
    unsigned long waktuMulai = millis();
    while (millis() - waktuMulai < 120000) { // 90 detik
      for (int i = 0; i < panjangTeks + 16; i++) {
        lcd.setCursor(0, 0);
        for (int j = 0; j < 16; j++) {
          if (i + j < panjangTeks) {
            lcd.print(teksBerjalan[i + j]);
          } else {
            lcd.print(" ");
          }
        }
        delay(50);
      }
    }
    lcd.clear();
  }
  
  delay(1000); // 1-second delay for the loop
}
