#include <SmingCore.h>
#include <Debug.h>
#include "ArduinoJson.h"

#define MQTT_CONF_FILENAME "test"

const uint16_t ConfigJsonBufferSize = 2048;

void init()
{
	Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1,
				 SERIAL_FULL);		// 115200 by default, GPIO1,GPIO3, see Serial.swap(), HardwareSerial
	Serial.systemDebugOutput(true); // Debug output to serial

	spiffs_mount_manual(0x0c0000, SPIFF_SIZE); //ESP8266-01 module
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

	DeserializationError error;

	int element_cntr = 0;

	do {
		element_cntr++;
		fileDelete(MQTT_CONF_FILENAME);
		for(int i = 0; i < element_cntr; i++) {
			root[String(i)] = String(i);
		}
		String jsonString;
		serializeJson(root, jsonString);

		int file_w_bytes = fileSetContent(MQTT_CONF_FILENAME, jsonString);

		debugf("bytes written %d ", file_w_bytes);

		int file_size = fileGetSize(MQTT_CONF_FILENAME);

		debugf("file size %d", file_size);

		String jsonString2 = fileGetContent(MQTT_CONF_FILENAME);

		debugf("string size %d", jsonString2.length());

		StaticJsonDocument<ConfigJsonBufferSize> root2;
		error = deserializeJson(root2, jsonString2);
		debugf("JSON deserial err: %s", error.c_str());

		Serial.printf("%s\r\n", (const char*)root2["User"]);
		Serial.printf("%s\r\n", (const char*)root2["Password"]);
		Serial.printf("%s\r\n", (const char*)root2["DeviceTopic"]);

		for(int i = 0; i < element_cntr; i++) {
			Serial.printf("%d -> %s\r\n", i, (const char*)root2[String(i)]);
		}

	} while(error == 0);
}
