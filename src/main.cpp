#include <WiFi.h>
#include <ESP32Servo.h>

#define SERVO_PIN 33
#define SERVO_PIN2 32
#define SERVO_PIN3 21

const char* ssid = "ESP32AP"; 
const char* password = "Password";
const char* serverIP = "192.168.4.1";
int xval=0;
int yval=0;
int button=1;
int angle=0;
int angley=0;
int angleb=0;

WiFiServer server(80);
WiFiClient client;
Servo myServo; // Create a servo obj 
Servo myServoy;
Servo clamps;

void setup() {
  Serial.begin(9600);
  delay(10);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  myServo.attach(SERVO_PIN);
  myServoy.attach(SERVO_PIN2);
  clamps.attach(SERVO_PIN3);
}

void loop() {

  // Connect to server
  if (client.connect(serverIP, 80)) {
    // Serial.println("Connected to server");
  
    // Send request
    client.println("Hello from ESP32 Client");
    client.flush();

    // Read request 
    String response = client.readStringUntil('\r');
    Serial.println("Response from server: " + response);
    
    // Parse response val
    int firstSpaceIdx = response.indexOf(' ');
    int secondSpaceIdx = response.indexOf(' ', firstSpaceIdx+1);
    int thirdSpaceIdx = response.indexOf(' ', secondSpaceIdx +1);
    int lastSpaceIdx = response.lastIndexOf(' ');
    // Extract the values between the first and second spaces
    String xval = response.substring(firstSpaceIdx + 1, secondSpaceIdx);
    String yval = response.substring(secondSpaceIdx + 7, thirdSpaceIdx+5);
    String buttonval = response.substring(lastSpaceIdx-1);
    int x_value = xval.toInt();
    int y_value = yval.toInt();
    int b_value = buttonval.toInt();

    Serial.println(x_value);
    Serial.println(y_value);
    Serial.println(b_value);

    // Control Servo 
    if (x_value>4094)
    {
      angle = angle + 10;
      myServo.write(angle);
      delay(10);
      Serial.print(angle);
      if (angle>180){
        angle=180;
      }
    }
    else if (x_value<1000)
    {
      angle = angle -10;
      myServo.write(angle);
      delay(10);
      Serial.print(angle);
      if (angle<0){
        angle=0;
      }
    }

    if (y_value>4094)
    {
      angley = angley + 10;
      myServoy.write(angley);
      delay(10);
      Serial.print(angley);
      if (angley>180){
        angley=180;
      }
    }
    else if (y_value<1000)
    {
      angley = angley -10;
      myServoy.write(angley);
      delay(10);
      Serial.print(angley);
      if (angley<0){
        angley=0;
      }
    }

    if (b_value==1)
    {
      clamps.write(0);
      delay(10);
    }
    else if (b_value==0)
    {
      clamps.write(180);
      delay(10);
    }

  } else {
    Serial.println("Connection failed");
  }

}