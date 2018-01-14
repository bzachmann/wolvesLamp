#ifndef TWITCHSTREAMERINFO_H
#define TWITCHSTREAMERINFO_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

class TwitchStreamerInfo
{
private:
    enum twitchError_t
    {
        ERROR_NO_ERROR = 0,
        ERROR_WIFI_CONNECTION,
        ERROR_CONNECTION_STREAM_STATUS,
        ERROR_TIMEOUT_STREAM_STATUS,
        ERROR_NO_JSON_STREAM_STATUS,
        ERROR_PARSING_JSON_STREAM_STATUS,
        ERROR_CONNECTION_HOST_LOOKUP,
        ERROR_TIMEOUT_HOST_LOOKUP,
        ERROR_NO_JSON_HOST_LOOKUP,
        ERROR_PARSING_JSON_HOST_LOOKUP,
        ERROR_CONNECTION_HOST_STATUS,
        ERROR_TIMEOUT_HOST_STATUS,
        ERROR_NO_JSON_HOST_STATUS,
        ERROR_PARSING_JSON_HOST_STATUS,
        NUM_ERRORS
    };

    enum updateState_t
    {
        STATE_CHECKING_WIFI_CONNECTION = 0,
        STATE_CONNECTING_TO_WIFI,
        STATE_WAITING_FOR_WIFI_CONNECTION,
        STATE_CONNECTING_TO_STREAM_STATUS,
        STATE_REQUESTING_PATH_STREAM_STATUS,
        STATE_WAITING_FOR_STREAM_STATUS,
        STATE_PARSING_JSON_STREAM_STATUS,
        STATE_CONNECTING_TO_HOST_LOOKUP,
        STATE_REQUESTING_PATH_HOST_LOOKUP,
        STATE_WAITING_FOR_HOST_LOOKUP,
        STATE_PARSING_JSON_HOST_LOOKUP,
        STATE_CONNECTING_TO_HOST_STATUS,
        STATE_REQUESTING_PATH_HOST_STATUS,
        STATE_WAITING_FOR_HOST_STATUS,
        STATE_PARSING_JSON_HOST_STATUS,
        STATE_ERROR,
        NUM_UPDATE_STATES
    };

public:    
    TwitchStreamerInfo(String twitchName) :
        online(false),
        hosting(false),
        twitchName(twitchName),
        twitchId(""),
        twitchGameName(""),
        hostName(""),
        hostId(""),
        hostGameName(""),
        upTimeMs(0),
        timeStamp(0) {}

    void init();
    void run();


private:
    bool online;
    bool hosting;
    uint8_t error;

    String const twitchName;
    String twitchId;
    String twitchGameName;
    String hostName;
    String hostId;
    String hostGameName;
    WiFiClientSecure client;
    DynamicJsonBuffer jsonBuffer;

    uint32_t upTimeMs;
    uint32_t timeStamp;
};

#endif // TWITCHSTREAMERINFO_H
