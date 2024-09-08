/*
 * kalibrasi untuk rtc d3231
 * by :  ajang rahmad modified by : made agus andi gunawan
 * 
 */
#include "DS3231_Simple.h"

DS3231_Simple Clock;

void setup() 
{
  Serial.begin(9600);
  Clock.begin();
}

void loop()
{ 
  DateTime waktu;
  waktu.Day    = 7;       //atur tanggal
  waktu.Month  = 9;      //atur bulan
  waktu.Year   = 24;      //atur tahun
  waktu.Hour   = 03;      //atur jam
  waktu.Minute = 12;      //atur menit
  waktu.Second = 00;      //atur detik
  Clock.write(waktu);
  
  Serial.print("Waktu telah di atur ke: ");
  Clock.printTo(Serial);
  Serial.println();
  
  Serial.print("Program berakhir (RESET untuk menjalakan lagi)");
  while(1);
}
