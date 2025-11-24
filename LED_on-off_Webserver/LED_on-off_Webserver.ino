/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

#define RED_LED 4
#define GREEN_LED 16
#define BLUE_LED 17


// Set these to your desired credentials.
const char *ssid = "led_controller";
const char *password = "87654321";

NetworkServer server(80);

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  // Set high to turn off at startup
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while (1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            const char htmlResponse[] PROGMEM = R"html(
              <!DOCTYPE html>
              <html>

              <head>
                  <title>LED Control</title>
                  <style>
                      body {
                          background-color: #050505;
                          color: white;
                          font-size: 22px;
                      }

                      button {
                          padding: 8px 16px;
                          margin: 4px;
                          font-size: 16px;
                          font-weight: bold;
                          cursor: pointer;
                          border: none;
                          border-radius: 4px;
                      }

                      .on {
                          background-color: #0CFF27;
                          color: white;
                      }

                      .off {
                          background-color: #FF290D;
                          color: white;
                      }
                  </style>
              </head>

              <body>
                  <h1>LED Control Panel</h1>
                  <p>
                      <span style="color: red; font-weight: bold;">RED LED</span>
                      <button class="on" onclick="location.href='/RO'">ON</button>
                      <button class="off" onclick="location.href='/RF'">OFF</button>
                  </p>
                  <p>
                      <span style="color: green; font-weight: bold;">GREEN LED</span>
                      <button class="on" onclick="location.href='/GO'">ON</button>
                      <button class="off" onclick="location.href='/GF'">OFF</button>
                  </p>
                  <p>
                      <span style="color: blue; font-weight: bold;">BLUE LED</span>
                      <button class="on" onclick="location.href='/BO'">ON</button>
                      <button class="off" onclick="location.href='/BF'">OFF</button>
                  </p>
              </body>

              </html>
            )html";
            
            client.print(htmlResponse);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /*" or "GET /*":
        // inverted logic for RGB LEDs on CYD
        if (currentLine.endsWith("GET /RF")) {
          digitalWrite(RED_LED, HIGH);
        }
        if (currentLine.endsWith("GET /RO")) {
          digitalWrite(RED_LED, LOW);
        }
        if (currentLine.endsWith("GET /GF")) {
          digitalWrite(GREEN_LED, HIGH);
        }
        if (currentLine.endsWith("GET /GO")) {
          digitalWrite(GREEN_LED, LOW);
        }
        if (currentLine.endsWith("GET /BF")) {
          digitalWrite(BLUE_LED, HIGH);
        }
        if (currentLine.endsWith("GET /BO")) {
          digitalWrite(BLUE_LED, LOW);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
