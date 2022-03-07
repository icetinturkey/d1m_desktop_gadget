#include "TM1637.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <TimeLib.h>
#include <Time.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "DHTesp.h"
#include <FastLED.h>
CRGB leds[2];
DHTesp dht;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
TM1637 tm1637(12,13);
unsigned long lastTimeMillis = 0;
bool nokta;
float ortamsicaklik;
int saniye3sayac;
int havaSayac;
int hava2Sayac;

void setup() {
  nokta=false;
  saniye3sayac=0;
  havaSayac=0;
  hava2Sayac=0;
  ortamsicaklik=0.00;
  tm1637.init();
  tm1637.set(6);
  u8g2.begin();
  dht.setup(0, DHTesp::DHT22);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0,10,"Wifi Begin...");
  u8g2.sendBuffer();
  WiFi.begin("{WIFI_KULLANICI_ADI}", "{WIFI_SIFRE");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  String ipadres = IpAddress2String(WiFi.localIP());
  char ipadresi[15];
  ipadres.toCharArray(ipadresi, 15);
  u8g2.drawStr(0,20,ipadresi);
  u8g2.sendBuffer();
  saatiAyarla();
  delay(1000);
  havaDownload();
  delay(1000);
  FastLED.addLeds<WS2812B, 14, GRB>(leds, 2).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  WiFi.disconnect();
  WiFi.forceSleepBegin();
  delay(1);
}

void loop() {
  if (millis() - lastTimeMillis > 1000) {
    lastTimeMillis = millis();
    saatiYenile();
    ekraniYenile();
    saniye3sayac++;
    if(saniye3sayac>3){
      saniye3sayac=0;
      ortamsicaklik=dht.getTemperature();
    }
  }
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1;
  leds[0] = ColorFromPalette( currentPalette, startIndex, 255, currentBlending);
  leds[1] = ColorFromPalette( currentPalette, startIndex+3, 255, currentBlending);
  FastLED.show();
  FastLED.delay(10);
}

void saatiAyarla(){
  HTTPClient client1;
  client1.begin("http://api.timezonedb.com/v2/get-time-zone?key={TIMEZONE_APIKEY}&format=json&by=zone&zone=Europe/Istanbul");
  int httpCode = client1.GET();
  String payload2 = client1.getString();
  delay(500);
  client1.end();
  DynamicJsonDocument doc2(1024);
  deserializeJson(doc2, payload2);


  long time1 = doc2["timestamp"];
  setTime(time1);
  char clkDown[5];
  String(httpCode).toCharArray(clkDown,5);
  u8g2.drawStr(0,30,clkDown);
  u8g2.sendBuffer();
}


String havaD;String havaB;
String havaC;String havaA;
void havaDownload(){
  HTTPClient client1;
  client1.begin("http://api.openweathermap.org/data/2.5/onecall?lat=40.75&lon=30.36&exclude=current,minutely,hourly,alerts&appid={OPENWEATHER_APIKEY}&units=metric&lang=en");
  int httpCode = client1.GET();
  String payload = client1.getString();
  delay(500);
  client1.end();
  DynamicJsonDocument doc(payload.length());
  deserializeJson(doc, payload);
  
  String hmax = doc["daily"][0]["temp"]["max"];
  String hmin = doc["daily"][0]["temp"]["min"];
  String hcon = doc["daily"][0]["weather"][0]["description"];
  String h2max = doc["daily"][1]["temp"]["max"];
  String h2min = doc["daily"][1]["temp"]["min"];
  String h2con = doc["daily"][1]["weather"][0]["description"];
hmax=String(hmax.toFloat(),1);
hmin=String(hmin.toFloat(),1);
h2max=String(h2max.toFloat(),1);
h2min=String(h2min.toFloat(),1);

    havaD = hmax + "/" + hmin;
  havaC = hcon;

    havaB = h2max + "/" + h2min;
  havaA = h2con;
  char weaDown[5];
  String(httpCode).toCharArray(weaDown,5);
  u8g2.drawStr(0,40,weaDown);
  u8g2.sendBuffer();
  delay(1000);
}

void saatiYenile(){
  int saat = hour();
  int dakika = minute();
  if(saat>7&&saat<19) tm1637.set(4);
  else tm1637.set(0);

  int saat_onlar_basamagi;
  int saat_birler_basamagi;
  int dakika_onlar_basamagi;
  int dakika_birler_basamagi;
  if(saat>9){
    saat_onlar_basamagi = saat/10;
    saat_birler_basamagi = saat-(saat_onlar_basamagi*10);
  }else{
    saat_onlar_basamagi = 0;
    saat_birler_basamagi = saat;
  }
  if(dakika>9){
    dakika_onlar_basamagi = dakika/10;
    dakika_birler_basamagi = dakika-(dakika_onlar_basamagi*10);
  }else{
    dakika_onlar_basamagi = 0;
    dakika_birler_basamagi = dakika;
  }
  if(nokta){
    TimeDisp[0] = saat_onlar_basamagi;
    TimeDisp[1] = saat_birler_basamagi;
    TimeDisp[2] = dakika_onlar_basamagi;
    TimeDisp[3] = dakika_birler_basamagi;
    tm1637.point(POINT_OFF);
    tm1637.display(TimeDisp);
    nokta=false;
  }else{
    TimeDisp[0] = saat_onlar_basamagi;
    TimeDisp[1] = saat_birler_basamagi;
    TimeDisp[2] = dakika_onlar_basamagi;
    TimeDisp[3] = dakika_birler_basamagi;
    tm1637.point(POINT_ON);
    tm1637.display(TimeDisp);
    nokta=true;
  }
}

void ekraniYenile(){
  String day_text;
  int namazaKalan;
  int vakit;
  String on_day_text = String(day())+"/"+String(month())+"/"+String(year());
  switch(weekday()){
    case 1:day_text="PAZAR";break;
    case 2:day_text="PAZARTESI";break;
    case 3:day_text="SALI";break;
    case 4:day_text="CARSAMBA";break;
    case 5:day_text="PERSEMBE";break;
    case 6:day_text="CUMA";break;
    case 7:day_text="CUMARTESI";break;
  }
  char text_dayt[day_text.length()+1];
  day_text.toCharArray(text_dayt, day_text.length()+1);
  char on_text_tarih[on_day_text.length()+1];
  on_day_text.toCharArray(on_text_tarih, on_day_text.length()+1);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_timB10_tf);
  u8g2.drawUTF8(0,10,on_text_tarih);
  u8g2.setFont(u8g2_font_8x13O_tf);
  u8g2.drawUTF8(40,26,text_dayt);
  u8g2.setFont(u8g2_font_timB10_tf);
  char text_osicaklik[5];
  String(ortamsicaklik).toCharArray(text_osicaklik, 5);
  u8g2.drawUTF8(100,10,text_osicaklik);
  
 
  
  char text_havaD[havaD.length()+1];
  havaD.toCharArray(text_havaD,havaD.length()+1);
  u8g2.setFont(u8g2_font_helvB12_te);
  u8g2.drawUTF8(0,45,text_havaD);
  String havaC_buffer;
  int havaC_uzunluk=havaC.length();
  if(havaC_uzunluk>10)
  {
    if(havaSayac>(havaC_uzunluk-10))
      havaSayac=0;
    havaC_buffer=havaC.substring(havaSayac,havaSayac+10);
    havaSayac+=1;
  }else{
    havaC_buffer=havaC;
  }
  char text_havaC[havaC_uzunluk+1];
  havaC_buffer.toCharArray(text_havaC,havaC_uzunluk+1);
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawUTF8(68,43,text_havaC);

  char text_havaB[havaB.length()+1];
  havaB.toCharArray(text_havaB,havaB.length()+1);
  u8g2.setFont(u8g2_font_helvB12_te);
  u8g2.drawUTF8(0,64,text_havaB);
  String havaA_buffer;
  int havaA_uzunluk=havaA.length();
  if(havaA_uzunluk>10)
  {
    if(hava2Sayac>(havaA_uzunluk-10))
      hava2Sayac=0;
    havaA_buffer=havaA.substring(hava2Sayac,hava2Sayac+10);
    hava2Sayac+=1;
  }else{
    havaA_buffer=havaA;
  }
  char text_havaA[havaA_uzunluk+1];
  havaA_buffer.toCharArray(text_havaA,havaA_uzunluk+1);
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawUTF8(68,62,text_havaA);
  
  u8g2.drawLine(0, 13, 128, 13);
  u8g2.drawLine(0, 29, 128, 29);
  u8g2.sendBuffer();
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}
