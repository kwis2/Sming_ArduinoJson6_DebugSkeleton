#include <SmingCore.h>
#include <Debug.h>
#include "ArduinoJson.h"

#define MQTT_CONF_FILENAME "test"

const uint8_t ConfigJsonBufferSize = 200;

void init()
{
	Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1,
				 SERIAL_FULL);		// 115200 by default, GPIO1,GPIO3, see Serial.swap(), HardwareSerial
	Serial.systemDebugOutput(true); // Debug output to serial

	spiffs_mount_manual(0x100000, SPIFF_SIZE);
	int check = SPIFFS_check(&_filesystemStorageHandle);
	debugf("FS check res: %d", check);

	Serial.commandProcessing(true); //also executes commandHandler.registerSystemCommands(); ?

	Debug.setDebug(Serial);
	Debug.initCommand();
	Debug.start();
	Debug.printf("This is the debug output\r\n");

	StaticJsonDocument<ConfigJsonBufferSize> root;

	root["User"] = "abcdefghijklmnopqrtsuvwxyz";
	root["Password"] = "abcdefghijklmnopqrtsuvwxyz";
	root["DeviceTopic"] = "abcdefghijklmnopqrtsuvwxyz";
	String jsonString;
	serializeJson(root, jsonString);

	fileSetContent(MQTT_CONF_FILENAME, jsonString);
	String jsonString2 = fileGetContent(MQTT_CONF_FILENAME);

	StaticJsonDocument<ConfigJsonBufferSize> root2;
	DeserializationError error = deserializeJson(root2,jsonString2);
	debugf("JSON deserial err: %s", error.c_str());

	Serial.printf("%s\r\n", (const char*)root2["User"]);
	Serial.printf("%s\r\n", (const char*)root2["Password"]);
	Serial.printf("%s\r\n", (const char*)root2["DeviceTopic"]);
}
