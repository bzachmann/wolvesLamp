#ifndef TWITCHSTREAMERINFO_H
#define TWITCHSTREAMERINFO_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

class TwitchStreamerInfo
{
public:
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
        ERROR_INVALID_STATE,
        NUM_ERRORS
    };

private:

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
        STATE_DONE,
        STATE_ERROR,
        NUM_UPDATE_STATES
    };

public:    
    TwitchStreamerInfo(String twitchName, String twitchId, float hours) :
        online(false),
        hosting(false),
        done(false),
        twitchName(twitchName),
        twitchId(twitchId),
        targetStreamHours(hours),
        twitchGameName(""),
        hostName(""),
        hostGameName(""),
        updateState(STATE_CHECKING_WIFI_CONNECTION),
        upTimeMs(0),
        timeOld(0),
        timeNew(0){}

    void init();
    void run();

    void reset();
    bool isDone();
    bool isLive();
    bool isHosting();
    bool isInError();
    uint8_t getError();
    String getGameName();
    String getHostName();
    String getHostGameName();
    uint32_t getUpTimeSeconds();
    uint8_t getProgress();

private:
    void updateStateMachine();
    void actionStateCheckingWifiConnection();
    void actionStateConnectingToWifi();
    void actionStateWaitingForWifiConnection();
    void actionStateConnectingToStreamStatus();
    void actionStateRequestingPathStreamStatus();
    void actionStateWaitingForStreamStatus();
    void actionStateParsingJsonStreamStatus();
    void actionStateConnectingToHostLookup();
    void actionStateRequestingPathHostLookup();
    void actionStateWaitingForHostLookup();
    void actionStateParsingJsonHostLookup();
    void actionStateConnectingToHostStatus();
    void actionStateRequestingPathHostStatus();
    void actionStateWaitingForHostStatus();
    void actionStateParsingJsonHostStatus();

private:
    bool online;
    bool hosting;
    bool done;
    uint8_t error;

    String const twitchName;
    String const twitchId;
    float const targetStreamHours;
    String twitchGameName;
    String hostName;
    String hostDisplayName;
    String hostGameName;
    WiFiClientSecure client;
    DynamicJsonBuffer jsonBuffer;

    updateState_t updateState;

    uint32_t upTimeMs;
    uint32_t timeOld;
    uint32_t timeNew;
};

#endif // TWITCHSTREAMERINFO_H
