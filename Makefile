# see
# http://mjo.tc/atelier/2009/02/arduino-cli.html
# http://emacswiki.org/emacs/ArduinoSupport

# Start serial terminal
# sudo apt-get install screen
# screen [serial-port] [baud-rate] , 9600 is standard  eg
# screen /dev/ttyUSB0

# Get available board tags by
# ard-parse-boards --boards
ARDUINO_DIR = /usr/share/arduino
BOARD_TAG    = atmega328
ARDUINO_PORT = /dev/ttyUSB*
ARDUINO_LIBS = PololuQik SoftwareSerial

# Jeg ved ikke hvilken af følgende der skal ændres:
# standard is 115200
#AVRDUDE_ARD_BAUDRATE = 115200

#MONITOR_BAUDRATE = 9600 #Det er sikkert denne

$(shell etags *.ino *.cpp)

include /usr/share/arduino/Arduino.mk
