# LED_on-off_Webserver

## Hardware

* [CYD](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display) if you want the built-in RGB LEDs. (some other boards may also have these)
* **OR** Add your own LEDS and define the pins. (see lines 18 - 20 in `LED_on-off_Webserver.ino`)

## Compile and Upload

* If you don't already have ESP32 support installed please do so. You can follow this tutorial: [ESP32 in Arduino IDE](https://lastminuteengineers.com/esp32-arduino-ide-tutorial/)
* Compile and upload the project using Arduino IDE (you will need to have your board and port selected. I recommend setting the Upload speed to 921600 for faster uploads: `Tools>Upload Speed>921600`)

## Connect

* Connect to the ESP after uploading the code. (find `led_controller` in your WiFi devices list)
* Enter the password `87654321`
* Open a browser and go to [http://192.168.4.1](http://192.168.4.1)
* Have fun turning the LEDs on and off from your device! 🥳


<img src="img/LED%20Control.png" alt="LED Control" width="300">

## Notes:

I've had a blast modifying this. Please dive in and have some fun!