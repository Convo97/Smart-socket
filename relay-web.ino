#include <WiFi.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

WiFiServer server(80);
const int controlPin = 13;  // GPIO 15

void setup() {
    Serial.begin(115200);
    
    pinMode(controlPin, OUTPUT);
    digitalWrite(controlPin, LOW);  // Start with LOW signal

    // Start ESP32 as an Access Point
    WiFi.softAP(ssid, password);
    Serial.println("ESP32 Access Point Started!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.begin();  // Start web server
}

void loop() {
    WiFiClient client = server.available(); // Check for incoming client

    if (client) {
        Serial.println("New Client Connected!");
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();

        // Control GPIO 15 based on request
        if (request.indexOf("/HIGH") != -1) {
            digitalWrite(controlPin, HIGH);
            Serial.println("GPIO 15 HIGH");
        } 
        else if (request.indexOf("/LOW") != -1) {
            digitalWrite(controlPin, LOW);
            Serial.println("GPIO 15 LOW");
        }

        // Send HTML response with buttons
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<html><body>");
        client.println("<h2>ESP32 GPIO 15 Control</h2>");
        client.println("<p><a href='/HIGH'><button style='width:100px;height:50px;'>HIGH</button></a></p>");
        client.println("<p><a href='/LOW'><button style='width:100px;height:50px;'>LOW</button></a></p>");
        client.println("</body></html>");

        client.stop();
        Serial.println("Client Disconnected.");
    }
}
