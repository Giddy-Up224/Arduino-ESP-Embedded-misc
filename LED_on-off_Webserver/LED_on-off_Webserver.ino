/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "led_controller"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /RO" and "GET /RF" etc. on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)

  Modified by: Giddy-Up224
  Date: Nov. 2025
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
    while (1)
      ;
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
        String req = "";
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();
                req += c;
                if (c == '\n') {
                    if (req.indexOf("GET / ") >= 0 || req.indexOf("GET /index.html") >= 0) {
                        serveHTML(client);
                    } else if (req.indexOf("GET /RO") >= 0) {
                        handleToggle(client, 'R', true);
                    } else if (req.indexOf("GET /RF") >= 0) {
                        handleToggle(client, 'R', false);
                    } else if (req.indexOf("GET /GO") >= 0) {
                        handleToggle(client, 'G', true);
                    } else if (req.indexOf("GET /GF") >= 0) {
                        handleToggle(client, 'G', false);
                    } else if (req.indexOf("GET /BO") >= 0) {
                        handleToggle(client, 'B', true);
                    } else if (req.indexOf("GET /BF") >= 0) {
                        handleToggle(client, 'B', false);
                    }
                    break;
                }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

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
            
          .switch {
              position: relative;
              display: inline-block;
              width: 50px;
              height: 28px;
          }
            
          .switch input {
              opacity: 0;
              width: 0;
              height: 0;
          }
            
          .slider {
              position: absolute;
              cursor: pointer;
              top: 0;
              left: 0;
              right: 0;
              bottom: 0;
              background-color: #888888;
              transition: .4s;
              border-radius: 28px;
          }
            
          .slider:before {
              position: absolute;
              content: "";
              height: 22px;
              width: 22px;
              left: 3px;
              bottom: 3px;
              background-color: white;
              transition: .4s;
              border-radius: 50%;
          }
            
  #red-led-toggle:checked+.slider {
              background-color: red;
          }
            
  #green-led-toggle:checked+.slider {
              background-color: green;
          }
            
  #blue-led-toggle:checked+.slider {
              background-color: blue;
          }
            
          input:checked+.slider:before {
              transform: translateX(22px);
          }
      </style>
  </head>
            
  <body>
      <h1>LED Control Panel</h1>
      <!-- Example toggle switch for RED LED -->
      <p>
          <span style="color: red; font-weight: bold;">RED LED</span>
          <label class="switch">
              <input type="checkbox" id="red-led-toggle" onchange="toggleLED('R', this.checked)">
              <span class="slider"></span>
          </label>
      </p>
      <p>
      <span style="color: green; font-weight: bold;">GREEN LED</span>
      <label class="switch">
          <input type="checkbox" id="green-led-toggle" onchange="toggleLED('G', this.checked)">
          <span class="slider"></span>
      </label>
      </p>
      <p>
      <span style="color: blue; font-weight: bold;">BLUE LED</span>
      <label class="switch">
          <input type="checkbox" id="blue-led-toggle" onchange="toggleLED('B', this.checked)">
          <span class="slider"></span>
      </label>
      </p>
            
      <script>
          function toggleLED(color, state) {
              // color: 'R', 'G', 'B'
              // state: true (ON), false (OFF)
              var url = '/' + color + (state ? 'O' : 'F');
              fetch(url)
          }
      </script>
  </body>
                        
              </html>
            )html";

// Handle LED toggle and send minimal response
void handleToggle(NetworkClient &client, char color, bool on) {
  int pin;
  if (color == 'R') pin = RED_LED;
  else if (color == 'G') pin = GREEN_LED;
  else if (color == 'B') pin = BLUE_LED;
  else return;

  digitalWrite(pin, on ? LOW : HIGH);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/plain");
  client.println();
  client.print(color == 'R' ? "RED " : color == 'G' ? "GREEN "
                                                    : "BLUE ");
  client.print(on ? "ON" : "OFF");
  client.println();
}

// Serve the HTML page
void serveHTML(NetworkClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.print(htmlResponse);
  client.println();
}