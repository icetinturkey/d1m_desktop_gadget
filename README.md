
# Arduino D1 Mini Wifi Desktop Gadget
## 1) Bitmiş hali
###### ![Image of Yaktocat](https://binkod.com.tr/img/d1mgadget.jpg)
## 2) Malzeme listesi
###### Wemos D1 Mini (US $1.96) https://www.aliexpress.com/item/32651747570.html
###### DHT22 Digital Temperature Sensor (US $2.00) https://www.aliexpress.com/item/1005001933682381.html
###### TM1637 LED Display Module (US $0.77) https://www.aliexpress.com/item/4000587257525.html
###### SH1106 OLED Module 1.3 inch (US $2.90) https://www.aliexpress.com/item/1005001950055514.html
###### WS2812B Addressable LED (US $1.22) https://www.aliexpress.com/item/4001346270449.html
###### :warning: D1 mini ve oled ekran haricindeki tüm malzemeler keyfidir. Isı sensörünü ucuz olsun diye dht11 kullanabilirsiniz.
## 3) Kodu hazırlamak
###### Repodaki d1m_gadget.ino dosyasındaki bazı verileri değiştirmelisiniz.
###### Değiştilecekler;
###### {WIFI_KULLANICI_ADI} => Bağlanacağı wifinin adı.
###### {WIFI_SIFRE} => Bağlanacağı wifinin şifresi.
###### {TIMEZONE_APIKEY} => timezonedb.com Adresinden aldığınız api key. Ücretsizdir.
###### {OPENWEATHER_APIKEY} => openweathermap.org Adresinden aldığınız api key. Ücretsizdir.
###### :warning: Ayrıca eksik donanım varsa onun kodlarınıda silin.
## 4) Kodlar
###### Hazırladığınız d1m_gadget.ino dosyasını wemosa kurun, bu kadar. 
###### :warning: Cihaz listesinde d1 mini'yi görebilmek için önce github.com/esp8266/Arduino bunu kurmalısınız.
