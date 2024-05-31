# ESP32 ile Firebase Cloud Functions ve Firestore Kullanarak Yapılan Uygulama

## Giriş

Bu rapor, ESP32 mikrodenetleyicisi kullanılarak yapılan bir IoT uygulamasını açıklamaktadır. Bu uygulama, Firebase Cloud Functions ve Firestore entegrasyonu ile mesaj alıp, bu mesajları LED ışıkları kullanarak Morse kodu ile göstermek üzere tasarlanmıştır. Rapor, uygulamanın çalışma prensibini, kullanılan teknolojileri ve kodların açıklamasını içermektedir.

## Uygulamanın Amacı

Uygulamanın amacı, Firebase'den alınan mesajları ESP32'ye göndermek ve bu mesajları LED'ler aracılığıyla Morse kodu şeklinde görsel olarak iletmektir. Bu sayede uzaktan mesaj alımı ve iletimi sağlanmaktadır.

## Kullanılan Teknolojiler

- **ESP32 Mikrodenetleyicisi**: Wi-Fi ve Bluetooth özellikleri ile IoT projeleri için ideal bir platformdur.
- **Firebase Cloud Functions**: Sunucusuz kod çalıştırma olanağı sağlayarak, Firebase ortamında güvenli ve ölçeklenebilir fonksiyonlar oluşturur.
- **Firestore**: Google'ın NoSQL veritabanı hizmeti olup, gerçek zamanlı veri senkronizasyonu sağlar.
- **Arduino IDE**: ESP32 için kullanılan geliştirme ortamıdır.

## Donanım Bileşenleri

- **ESP32 Geliştirme Kartı**
- **LED'ler**: Mesajları Morse kodu ile göstermek için kullanılan ışık kaynakları.
- **Düğme (BOOT_BTN)**: Mesajları görüntülemek için kullanılan tetikleyici.

## Yazılım Mimarisi

Uygulama, ESP32'nin Wi-Fi ağına bağlanmasıyla başlar. Firebase Cloud Functions üzerinden alınan mesajlar Firestore'da saklanır ve ESP32'ye çekilir. Çekilen mesajlar, Morse koduna dönüştürülerek LED'ler aracılığıyla görselleştirilir.

## Arduino'da Kullanılan Kütüphaneler

Uygulamanın geliştirilmesinde kullanılan temel kütüphaneler şunlardır:

- **WiFi.h**: ESP32'nin Wi-Fi bağlantısını yönetmek için kullanılır.
- **HTTPClient.h**: HTTP isteklerini göndermek ve yanıtları almak için kullanılır.
- **Arduino_JSON.h**: JSON verilerini ayrıştırmak ve oluşturmak için kullanılır.
- **Ticker.h**: Periyodik görevlerin zamanlanması için kullanılır.

## Kod Açıklaması

### Bağlantı ve Başlatma

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Ticker.h>

#define LED_PIN 2 
#define DOT_DELAY 250 
#define DASH_DELAY 3 * DOT_DELAY 
#define LED_2 13
#define BOOT_BTN 0

#define WIFI_SSID "Abdulsamet Genç’s iPhone"
#define WIFI_PASSWORD "12345678Ts"

Ticker timer;

void setup(){
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_2,OUTPUT);
  pinMode(BOOT_BTN,INPUT_PULLUP);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  checkFunction();
  timer.attach(5, checkFunction); 
}
```
