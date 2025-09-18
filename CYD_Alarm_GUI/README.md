# CYD_Alarmclock_GUI


### How to read/write ```*.bin``` file to/from ESP32

```bash
# Read the bin file from an ESP32
esptool --chip esp32 --port /dev/ttyUSB0 --baud 460800  read-flash 0x00000 0x400000 CYD_Alarmclock_GUI_2025-07-05.bin

# Write a bin file to an ESP32. Run for the location where you've got the "CYD_Alarmclock_GUI_2025-07-05.bin"
esptool --chip esp32 --port /dev/ttyUSB0 --baud 460800  write-flash 0x00000 CYD_Alarmclock_GUI_2025-07-05.bin
```