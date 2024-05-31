# ESP32 ve Bulut Servisler Kullanılarak Geliştirilen IoT Mesajlaşma Uygulaması


## Projeyi Yapanlar

Yakuphan IŞIK - 223405096

Abdulsamet GENÇ - 203405028

Yuşa Umut YERLİÇAY - 213405081


## Giriş

Bu rapor, ESP32 mikrodenetleyicisi kullanılarak geliştirilen bir IoT uygulamasının açıklamasını sunmaktadır. Bu uygulama, Firebase Cloud Functions ve Firestore entegrasyonu ile mesaj alıp, bu mesajları LED ışıkları kullanarak Morse alfabesi ile göstermek üzere tasarlanmıştır. Rapor, uygulamanın çalışma prensibini, kullanılan teknolojileri ve kodların açıklamasını içermektedir.

## Uygulamanın Amacı

Uygulamanın amacı, Firebase'den alınan mesajları ESP32'ye göndermek ve bu mesajları LED'ler aracılığıyla Morse alfabesş kullanarak görsel olarak iletmektir. Bu sayede uzaktan mesaj alımı ve iletimi sağlanmaktadır.

## Kullanılan Teknolojiler

- **ESP32 Mikrodenetleyicisi**: Wi-Fi ve kodlanabilir arayüz özellikleri ile IoT projeleri için ideal bir platformdur.
- **Firebase Cloud Functions**: Sunucusuz kod çalıştırma olanağı sağlayarak, Firebase ortamında güvenli ve ölçeklenebilir fonksiyonlar oluşturur.
- **Firestore**: Google'ın NoSQL veritabanı hizmeti olup, gerçek zamanlı veri senkronizasyonu sağlar.
- **Arduino IDE**: ESP32 için kullanılan geliştirme ortamıdır.

## Donanım Bileşenleri

- **ESP32 Geliştirme Kartı**
- **LED'ler**: Mesajları Morse kodu ile göstermek ve mesaj geldiğini görüntülemek için kullanılan ışık kaynakları.
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
Bu bölümde, ESP32'nin Wi-Fi ağına bağlanması sağlanır ve bağlantı başarılı olduğunda IP adresi seri monitöre yazdırılır. Ayrıca, periyodik olarak mesaj kontrolü yapmak üzere bir timer başlatılır.

### Mesaj Kontrol ve Morse Kodu İletimi
```cpp
bool isThereMessage = false;
String message = "";

void loop() {
  if (digitalRead(BOOT_BTN) == LOW && isThereMessage) {
    delay(50);
    digitalWrite(LED_2,LOW);
    morseBlink(message);
    deleteMessage();
  }
}
```
Düğmeye basıldığında, alınan mesaj Morse alfabesi ile LED'ler aracılığıyla gösterilir ve ardından mesaj silinir.

 ### Mesaj Getirme ve Silme Fonksiyonları
```cpp
String getMessage() {
  String response;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://app-3odszoonka-uc.a.run.app/latest-message";
    
    http.begin(url);
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.GET();

    if (httpCode > 0) {
      if(httpCode==200){
        response = http.getString();
        Serial.println("HTTP Response code: " + String(httpCode));
        Serial.println("Response: " + response);
        isThereMessage = true;
        digitalWrite(LED_2,HIGH);
      }else{
        Serial.println("Henüz bir mesaj yok.");
      }
    } else {
      Serial.println("Error code: " + http.errorToString(httpCode));
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  return parseJson(response);
}

void deleteMessage() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://app-3odszoonka-uc.a.run.app/delete-message";
    
    http.begin(url);
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.GET();
    if (httpCode > 0 && httpCode == 200) {
      isThereMessage = false;
    } else {
      Serial.println("Error code: " + http.errorToString(httpCode));
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
```
Bu fonksiyonlar, Firebase'den en son mesajı getirir ve işlem sonrası mesajı siler.
### JSON Ayrıştırma
``` cpp
String parseJson(String input){
    JSONVar myObject = JSON.parse(input);
    if (JSON.typeof(myObject) == "undefined") {
      return "";
    }
    if (myObject.hasOwnProperty("text")) {
      Serial.print("myObject[\"text\"] = ");
      Serial.println((String)myObject["text"]);
    }
    return (String)myObject["text"];
}
```
Bu fonksiyon, gelen JSON verisini ayrıştırarak mesajın metin kısmını döndürür.
### Morse Kodu Fonksiyonları
```cpp
void morseBlink(String text) {
  for (int i = 0; i < text.length(); i++) {
    char c = text.charAt(i);
    switch (c) {
      // Her karakter için Morse kodu
      // ...
    }
    delay(DOT_DELAY); // Harfler arası boşluk
  }
}

void blink(String morse) {
  for (int i = 0; i < morse.length(); i++) {
    char signal = morse.charAt(i);
    if (signal == '.') {
      digitalWrite(LED_PIN, HIGH);
      delay(DOT_DELAY);
    } else if (signal == '-') {
      digitalWrite(LED_PIN, HIGH);
      delay(DASH_DELAY);
    }
    digitalWrite(LED_PIN, LOW);
    delay(DOT_DELAY);
  }
}
```
Bu fonksiyonlar, mesajı Morse koduna çevirir ve LED'ler aracılığıyla sinyaller gönderir.
## Sonuç
Bu uygulama, ESP32 ve Firebase entegrasyonu ile uzaktan mesaj iletimi ve görsel sinyalizasyonunu başarılı bir şekilde gerçekleştirmektedir. Wi-Fi bağlantısı ve Firebase Cloud Functions kullanılarak esnek ve ölçeklenebilir bir çözüm sunulmuştur. Geliştirilen sistem, IoT uygulamaları için pratik bir örnek oluşturmaktadır.
