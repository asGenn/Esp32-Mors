#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Ticker.h>

#define LED_PIN 2 // LED'in bağlı olduğu pin
#define DOT_DELAY 250 // Nokta sinyali için bekleme süresi
#define DASH_DELAY 3 * DOT_DELAY // Çizgi sinyali için bekleme süresi
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
  String response;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://app-3odszoonka-uc.a.run.app/delete-message";
    
    http.begin(url);
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.GET();
    if (httpCode > 0) {
      if(httpCode==200){
        isThereMessage = false;
        
        return;
      }
    } else {
      Serial.println("Error code: " + http.errorToString(httpCode));
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  
}

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

// Morse kodu olarak gelen bir metni LED üzerinden yanıp söndürme fonksiyonu
void morseBlink(String text) {
  for (int i = 0; i < text.length(); i++) { // Metnin her bir karakterini dön
    char c = text.charAt(i); // Karakteri al
    switch (c) {
      case 'A':
      case 'a':
        blink(".-");
        break;
      case 'B':
      case 'b':
        blink("-...");
        break;
      case 'C':
      case 'c':
        blink("-.-.");
        break;
      case 'D':
      case 'd':
        blink("-..");
        break;
      case 'E':
      case 'e':
        blink(".");
        break;
      case 'F':
      case 'f':
        blink("..-.");
        break;
      case 'G':
      case 'g':
        blink("--.");
        break;
      case 'H':
      case 'h':
        blink("....");
        break;
      case 'I':
      case 'i':
        blink("..");
        break;
      case 'J':
      case 'j':
        blink(".---");
        break;
      case 'K':
      case 'k':
        blink("-.-");
        break;
      case 'L':
      case 'l':
        blink(".-..");
        break;
      case 'M':
      case 'm':
        blink("--");
        break;
      case 'N':
      case 'n':
        blink("-.");
        break;
      case 'O':
      case 'o':
        blink("---");
        break;
      case 'P':
      case 'p':
        blink(".--.");
        break;
      case 'Q':
      case 'q':
        blink("--.-");
        break;
      case 'R':
      case 'r':
        blink(".-.");
        break;
      case 'S':
      case 's':
        blink("...");
        break;
      case 'T':
      case 't':
        blink("-");
        break;
      case 'U':
      case 'u':
        blink("..-");
        break;
      case 'V':
      case 'v':
        blink("...-");
        break;
      case 'W':
      case 'w':
        blink(".--");
        break;
      case 'X':
      case 'x':
        blink("-..-");
        break;
      case 'Y':
      case 'y':
        blink("-.--");
        break;
      case 'Z':
      case 'z':
        blink("--..");
        break;
      case '0':
        blink("-----");
        break;
      case '1':
        blink(".----");
        break;
      case '2':
        blink("..---");
        break;
      case '3':
        blink("...--");
        break;
      case '4':
        blink("....-");
        break;
      case '5':
        blink(".....");
        break;
      case '6':
        blink("-....");
        break;
      case '7':
        blink("--...");
        break;
      case '8':
        blink("---..");
        break;
      case '9':
        blink("----.");
        break;
      case '.':
        blink(".-.-.-");
        break;
      case ',':
        blink("--..--");
        break;
      case '?':
        blink("..--..");
        break;
      case '\'':
        blink(".----.");
        break;
      case '!':
        blink("-.-.--");
        break;
      case '/':
        blink("-..-.");
        break;
      case '(':
      case ')':
        blink("-.--.");
        break;
      case '&':
        blink(".-...");
        break;
      case ':':
        blink("---...");
        break;
      case ';':
        blink("-.-.-.");
        break;
      case '=':
        blink("-...-");
        break;
      case '+':
        blink(".-.-.");
        break;
      case '-':
        blink("-....-");
        break;
      case '_':
        blink("..--.-");
        break;
      case '"':
        blink(".-..-.");
        break;
      case '$':
        blink("...-..-");
        break;
      case '@':
        blink(".--.-.");
        break;
      case ' ':
        delay(3 * DOT_DELAY); // Kelimeler arası boşluk
        break;
      default:
        // Bilinmeyen karakterler için hiçbir şey yapma
        break;
    }
    delay(DOT_DELAY); // Harfler arası boşluk
  }
}

void checkFunction() {

  if(isThereMessage){
    Serial.println(message);
    
  }else{
    message = getMessage();
  }
  // Burada yapmak istediğiniz işlemleri gerçekleştirin
}

// Morse kodu olarak gelen bir karakteri LED üzerinden yanıp söndürme fonksiyonu
void blink(String morse) {
  for (int i = 0; i < morse.length(); i++) { // Morse kodunun her bir karakterini dön
    char signal = morse.charAt(i); // Sinyali al
    if (signal == '.') { // Nokta sinyali
      digitalWrite(LED_PIN, HIGH); // LED'i yak
      delay(DOT_DELAY); // Bekle
    } else if (signal == '-') { // Çizgi sinyali
      digitalWrite(LED_PIN, HIGH); // LED'i yak
      delay(DASH_DELAY); // Bekle
    }
    digitalWrite(LED_PIN, LOW); // LED'i söndür
    delay(DOT_DELAY); // Sinyaller arası boşluk
  }
}
