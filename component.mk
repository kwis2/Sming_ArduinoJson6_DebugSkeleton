COM_PORT = /dev/ttyUSB0
RBOOT_ENABLED		= 1
RBOOT_BIG_FLASH		= 1
## two rom mode (where two roms sit in the same 1mb block of flash)
#RBOOT_TWO_ROMS		?= 1

SPI_SIZE			= 4M

SPIFF_SIZE			= 65536

ARDUINO_LIBRARIES := ArduinoJson6

COMPONENT_SRCDIRS := app

COMPONENT_INCDIRS := include