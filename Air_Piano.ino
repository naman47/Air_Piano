//IP: http://192.168.43.209/listen
//192.168.43.195
//Port: 80
 
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701


const int trigPinC = 5;
const int echoPinC = 15;

const int trigPinD = 12;
const int echoPinD = 14;

const int trigPinE = 16;
const int echoPinE = 17;

const int trigPinF = 18;
const int echoPinF = 19;

const int trigPinG = 2;
const int echoPinG = 4;

const int trigPinA = 23;
const int echoPinA = 22;

const int trigPinB = 13;
const int echoPinB = 21;

String pianoMode;
String listenTune;
char A = 'A';
char B = 'B';
char C = 'C';
char D = 'D';
char E = 'E';
char F = 'F';
char G = 'G';
String result;
int tuneLen;
int i;
//int j = 0;
const int TONE_OUTPUT_PIN = 27;
int channel = 0;

const int TONE_PWM_CHANNEL = 0;

long durationC;
float distanceCmC;
float distanceInchC;

long durationD;
float distanceCmD;
float distanceInchD;

long durationE;
float distanceCmE;
float distanceInchE;

long durationF;
float distanceCmF;
float distanceInchF;

long durationG;
float distanceCmG;
float distanceInchG;

long durationA;
float distanceCmA;
float distanceInchA;

long durationB;
float distanceCmB;
float distanceInchB;

const byte output = 27;

const char* ssid = "Galaxy M51C1EA";
const char* password = "naman2547";
// This is GandiStandardSSLCA2.pem, the root Certificate Authority that signed 
// the server certifcate for the demo server https://jigsaw.w3.org in this
// example. This certificate is valid until Sep 11 23:59:59 2024 GMT
const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"-----END CERTIFICATE-----\n";




void setClock() {
  configTime(0, 0, "pool.ntp.org");

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2) {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
}




WiFiMulti WiFiMulti;
String recordTune;




void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  // wait for WiFi connection
  Serial.print("Waiting for WiFi to connect...");
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }
  Serial.println(" connected");

  setClock();
  
  pinMode(trigPinC, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinC, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinD, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinD, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinE, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinE, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinF, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinF, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinG, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinG, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinA, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinA, INPUT); // Sets the echoPin as an Input
  
  pinMode(trigPinB, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinB, INPUT); // Sets the echoPin as an Input
  
  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);

  /*pianoMode = postRequest();

  if(pianoMode == "")
  {
    ESP.restart();
  }*/
}




void loop() {
  //if ((pianoMode.indexOf("Play") || pianoMode.indexOf("play")) > 0)
  //{
    // Clears the trigPin
    digitalWrite(trigPinC, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPinC, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinC, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    durationC = pulseIn(echoPinC, HIGH);
    
    digitalWrite(trigPinD, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinD, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinD, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    durationD = pulseIn(echoPinD, HIGH);
  
    digitalWrite(trigPinE, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinE, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinE, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    durationE = pulseIn(echoPinE, HIGH);
  
    digitalWrite(trigPinF, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinF, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinF, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    durationF = pulseIn(echoPinF, HIGH);
  
    digitalWrite(trigPinG, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinG, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinG, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    durationG = pulseIn(echoPinG, HIGH);
    
    digitalWrite(trigPinA, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinA, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinA, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    durationA = pulseIn(echoPinA, HIGH);
  
    digitalWrite(trigPinB, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinB, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinB, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    durationB = pulseIn(echoPinB, HIGH);
    
    // Calculate the distance
    distanceCmC = durationC * SOUND_SPEED/2;
    distanceInchC = distanceCmC * CM_TO_INCH;
  
    distanceCmD = durationD * SOUND_SPEED/2;
    distanceInchD = distanceCmD * CM_TO_INCH;
  
    distanceCmE = durationE * SOUND_SPEED/2;
    distanceInchE = distanceCmE * CM_TO_INCH;
  
    distanceCmF = durationF * SOUND_SPEED/2;
    distanceInchF = distanceCmF * CM_TO_INCH;
  
    distanceCmG = durationG * SOUND_SPEED/2;
    distanceInchG = distanceCmG * CM_TO_INCH;
  
    distanceCmA = durationA * SOUND_SPEED/2;
    distanceInchA = distanceCmA * CM_TO_INCH;
  
    distanceCmB = durationB * SOUND_SPEED/2;
    distanceInchB = distanceCmB * CM_TO_INCH;
    
    if (distanceCmC < 10)
    {
      Serial.print("C");
      alarmC();
      recordTune.concat("C");
    }
  
    
    else if (distanceCmD < 10)
    {
      Serial.print("D");
      alarmD();
      recordTune.concat("D");
    }
  
    
    else if (distanceCmE < 10)
    {
      Serial.print("E");
      alarmE();
      recordTune.concat("E");
    }
  
    
    else if (distanceCmF < 10)
    {
      Serial.print("F");
      alarmF();
      recordTune.concat("F");
    }
  
    
    else if (distanceCmG < 10)
    {
      Serial.print("G");
      alarmG();
      recordTune.concat("G");
    }
  
    
    else if (distanceCmA < 10)
    {
      Serial.print("A");
      alarmA();
      recordTune.concat("A");
    }
  
    
    else if (distanceCmB < 10)
    {
      Serial.print("B");
      alarmB();
      recordTune.concat("B");
    }
    if(recordTune.length() == 28){
      saveData();
      recordTune = "";
      ESP.restart();
    }
  /*}
  else if (pianoModes1.indexOf("Listen") > 0)
  {
    while (listenTune == "")
    {
      listenTune = playTune();
    }
    tuneLen = listenTune.length();
    int j = listenTune.indexOf("_");
    for ( i = j; i < tuneLen; i++){
      if (listenTune[i] == C)
      {
        alarmC();
      }
      else if (listenTune[i] == D)
      {
        alarmD();
      }
      else if (listenTune[i] == E)
      {
        alarmE();
      }
      else if (listenTune[i] == F)
      {
        alarmF();
      }
      else if (listenTune[i] == F)
      {
        alarmG();
      }
      else if (listenTune[i] == A)
      {
        alarmA();
      }
      else if (listenTune[i] == B)
      {
        alarmB();
      }
    }
    listenTune = "";
    ESP.restart();
   }*/
}




void alarmC()
{
  // Plays the middle C scale
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 4);
  delay(500);
  ledcWrite(channel, 0);
}




void alarmD()
{
  // Plays the middle D scale
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_D, 4);
  delay(500);
  ledcWrite(channel, 0);
}




void alarmE()
{
  // Plays the middle E scale
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_E, 4);
  delay(500);
  ledcWrite(channel, 0);
}




void alarmF()
{
  // Plays the middle F scale
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_F, 4);
  delay(500);
  ledcWrite(channel, 0);
}




void alarmG()
{
  // Plays the middle G scale
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_G, 4);
  delay(500);
  ledcWrite(channel, 0);
}




void alarmA()
{
  // Plays the middle A scale
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_A, 4);
  delay(500);
  ledcWrite(channel, 0);
}




void alarmB()
{
  // Plays the middle B scale
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_B, 4);
  delay(500);
  ledcWrite(channel, 0);
}





/*String postRequest(){
  WiFiClientSecure *client = new WiFiClientSecure;
  String payload;
  if(client) {
    client -> setCACert(rootCACertificate);

    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
      HTTPClient https;
  
      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, "https://karthikcm.com/php/test.php")) {  // HTTPS
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        https.addHeader("Content-Type", "text/plain");
        int httpCode = https.POST("What do you want to do?");
        payload = https.getString();        
        Serial.println(payload);
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            Serial.printf("[HTTPS] GET... Success");
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
  
        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }

      // End extra scoping block
    }
  
    delete client;
  } else {
    Serial.println("Unable to create client");
  }

  Serial.println();
  Serial.println("Waiting 10s before the next round...");
  delay(10000);

  return (payload);
}*/




/*String playTune(){
  WiFiClientSecure *client = new WiFiClientSecure;
  String payload;
  if(client) {
    client -> setCACert(rootCACertificate);

    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
      HTTPClient https;
  
      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, "https://karthikcm.com/php/test.php")) {  // HTTPS
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        https.addHeader("Content-Type", "text/plain");
        int httpCode = https.POST("What do you want to do?");
        payload = https.getString();        
        Serial.println(payload);
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            Serial.printf("[HTTPS] GET... Success");
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
  
        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }

      // End extra scoping block
    }
  
    delete client;
  } else {
    Serial.println("Unable to create client");
  }

  Serial.println();
  Serial.println("Waiting 5s before the next round...");
  delay(5000);

  return (payload);
}*/




String saveData(){
  WiFiClientSecure *client = new WiFiClientSecure;
  String payload;
  if(client) {
    client -> setCACert(rootCACertificate);

    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
      HTTPClient https;
  
      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, "https://karthikcm.com/php/get_song.php")) {  // HTTPS
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        // https.addHeader("Content-Type", "application/json");
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");
        
        Serial.println("{\"song_notes\":\""+ recordTune + "\"}");
        String httpsRequestData = "song_notes="+recordTune;
        Serial.println("Data sent = start------->" + httpsRequestData + "<--------end");
        int httpCode = https.POST(httpsRequestData);
        payload = https.getString();        
        Serial.println(payload);
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            Serial.printf("[HTTPS] GET... Success");
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
  
        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }

      // End extra scoping block
    }
  
    delete client;
  } else {
    Serial.println("Unable to create client");
  }

  Serial.println();
  Serial.println("Waiting 5s before the next round...");
  delay(5000);

  return (payload);
}
