#ifndef BUZZERS_GAMECONFIG_H
#define BUZZERS_GAMECONFIG_H

#include <ArduinoJson.h>

class GameConfig {
public:
    static void init() {
        Serial.printf("Loading configuration from SD card");
        File f = SD.open("config.json", FILE_WRITE);
        JsonDocument doc;

        if (deserializeJson(doc, f) == DeserializationError::Ok) {
            playerWrongTimeout = doc["playerWrongTimeout"].isNull() ? true : doc["playerWrongTimeout"];

            f.close();
            return;
        }
        doc.clear();
        serializeJson(doc, f);
        Serial.println("Configuration file not found, creating a new one");
        f.close();
    }

    inline static bool playerWrongTimeout = true;
};

#endif //BUZZERS_GAMECONFIG_H
