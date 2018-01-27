#include "twitchstreamerinfo.h"
#include "debugSerial.h"
#include <stdint.h>

#define SSID                        ("Zachmann")
#define PASSWORD                    ("bollY62417")
#define TIMEOUT_WIFI_MS             (10000)
#define TIMEOUT_STREAM_STATUS_MS    (5000)
#define DELAY_STREAM_STATUS_MS      (200)
#define TIMEOUT_HOST_LOOKUP_MS      (5000)
#define DELAY_HOST_LOOKUP_MS        (200)
#define TIMEOUT_HOST_STATUS_MS      (5000)
#define DELAY_HOST_STATUS_MS        (200)

#define STREAM_STATUS_HOST          ("wind-bow.glitch.me")
#define STREAM_STATUS_SUBPATH       ("/twitch-api/streams/")
#define HOST_LOOKUP_HOST            ("tmi.twitch.tv")
#define HOST_LOOKUP_SUBPATH         ("/hosts?include_logins=1&host=")

void TwitchStreamerInfo::init()
{
    timeOld = timeNew = millis();
    WiFi.mode(WIFI_OFF);
}

void TwitchStreamerInfo::run()
{
    timeNew = millis();
    updateStateMachine();

    if(isLive())
    {
        upTimeMs = upTimeMs + (timeNew - timeOld);
    }
    else
    {
        upTimeMs = 0;
    }

    timeOld = timeNew;
}

void TwitchStreamerInfo::reset()
{
    //online = false;
    //hosting = false;
    done = false;
    //twitchGameName = "";
    //hostName = "";
    //hostDisplayName = "";
    //hostGameName = "";
    client.stop();
    jsonBuffer.clear();
    updateState = STATE_CHECKING_WIFI_CONNECTION;
}

bool TwitchStreamerInfo::isDone()
{
    return done;
}

bool TwitchStreamerInfo::isHosting()
{
    return hosting;
}

bool TwitchStreamerInfo::isInError()
{
    return updateState == STATE_ERROR;
}

uint8_t TwitchStreamerInfo::getError()
{
    return error;
}

String TwitchStreamerInfo::getGameName()
{
    return twitchGameName;
}

String TwitchStreamerInfo::getHostName()
{
    String retVal = hostDisplayName;
    if(retVal == "")
    {
        retVal = hostName;
    }
    return retVal;
}

String TwitchStreamerInfo::getHostGameName()
{
    return hostGameName;
}

uint32_t TwitchStreamerInfo::getUpTimeSeconds()
{
    return upTimeMs / 1000;
}

uint8_t TwitchStreamerInfo::getProgress()
{
    uint32_t targetSeconds = targetStreamHours * 3600.0;
    float progressPercentage = getUpTimeSeconds()/(float)targetSeconds;
    if(progressPercentage > 1.0)
    {
        progressPercentage = 1.0;
    }
    return (uint8_t)(progressPercentage * 255);
}

bool TwitchStreamerInfo::isLive()
{
    return (online && !isHosting());
}

void TwitchStreamerInfo::updateStateMachine()
{
    switch(updateState)
    {
    case STATE_CHECKING_WIFI_CONNECTION:
        actionStateCheckingWifiConnection();
        break;
    case STATE_CONNECTING_TO_WIFI:
        actionStateConnectingToWifi();
        break;
    case STATE_WAITING_FOR_WIFI_CONNECTION:
        actionStateWaitingForWifiConnection();
        break;
    case STATE_CONNECTING_TO_STREAM_STATUS:
        actionStateConnectingToStreamStatus();
        break;
    case STATE_REQUESTING_PATH_STREAM_STATUS:
        actionStateRequestingPathStreamStatus();
        break;
    case STATE_WAITING_FOR_STREAM_STATUS:
        actionStateWaitingForStreamStatus();
        break;
    case STATE_PARSING_JSON_STREAM_STATUS:
        actionStateParsingJsonStreamStatus();
        break;
    case STATE_CONNECTING_TO_HOST_LOOKUP:
        actionStateConnectingToHostLookup();
        break;
    case STATE_REQUESTING_PATH_HOST_LOOKUP:
        actionStateRequestingPathHostLookup();
        break;
    case STATE_WAITING_FOR_HOST_LOOKUP:
        actionStateWaitingForHostLookup();
        break;
    case STATE_PARSING_JSON_HOST_LOOKUP:
        actionStateParsingJsonHostLookup();
        break;
    case STATE_CONNECTING_TO_HOST_STATUS:
        actionStateConnectingToHostStatus();
        break;
    case STATE_REQUESTING_PATH_HOST_STATUS:
        actionStateRequestingPathHostStatus();
        break;
    case STATE_WAITING_FOR_HOST_STATUS:
        actionStateWaitingForHostStatus();
        break;
    case STATE_PARSING_JSON_HOST_STATUS:
        actionStateParsingJsonHostStatus();
        break;
    case STATE_DONE:
        done = true;
        //do nothing, wait for reset
        break;
    case STATE_ERROR:
        break;
    default:
        debugPrint("Invalid state detected\n");
        error = ERROR_INVALID_STATE;
        updateState = STATE_ERROR;
        break;
    }
}


void TwitchStreamerInfo::actionStateCheckingWifiConnection()
{
    debugPrint("Checking Wifi Connection...\n");
    if(WiFi.status() != WL_CONNECTED)
    {
        debugPrint("Not Connected\n");
        updateState = STATE_CONNECTING_TO_WIFI;
    }
    else
    {
        debugPrint("Already Connected\n");
        updateState = STATE_CONNECTING_TO_STREAM_STATUS;
    }
}

void TwitchStreamerInfo::actionStateConnectingToWifi()
{
    debugPrint("Connecting to ");
    debugPrint(SSID);
    debugPrint("\n");
    WiFi.begin(SSID, PASSWORD);
    updateState = STATE_WAITING_FOR_WIFI_CONNECTION;
}

void TwitchStreamerInfo::actionStateWaitingForWifiConnection()
{
    static uint16_t timeout = 0;

    if(WiFi.status() != WL_CONNECTED)
    {
        if(timeout < TIMEOUT_WIFI_MS)
        {
            timeout += (timeNew - timeOld);
        }
        else
        {
            WiFi.mode(WIFI_OFF);
            debugPrint("WiFi connection timeout\n");
            error = ERROR_WIFI_CONNECTION;
            updateState = STATE_ERROR;
            timeout = 0;
        }
    }
    else
    {
        debugPrint("WiFi connected\n");
        debugPrint("It took ");
        debugPrint(timeout);
        debugPrint(" milliseconds to connect\n");
        debugPrint("IP address: ");
        debugPrint(WiFi.localIP());
        debugPrint("\n");
        error = ERROR_NO_ERROR;
        updateState = STATE_CONNECTING_TO_STREAM_STATUS;
        timeout = 0;
    }
}

void TwitchStreamerInfo::actionStateConnectingToStreamStatus()
{
    debugPrint("Connecting to ");
    debugPrint(STREAM_STATUS_HOST);
    debugPrint("\n");
    if(!client.connect(STREAM_STATUS_HOST, 443))
    {
        debugPrint("Connection Failed\n");
        error = ERROR_CONNECTION_STREAM_STATUS;
        updateState = STATE_ERROR;
    }
    else
    {
        debugPrint("Connection Successful\n");
        updateState = STATE_REQUESTING_PATH_STREAM_STATUS;
    }
}

void TwitchStreamerInfo::actionStateRequestingPathStreamStatus()
{
    debugPrint("Requsting URL: ");
    debugPrint(STREAM_STATUS_SUBPATH);
    debugPrint(twitchName);
    debugPrint("\n");

    client.print(String("GET ") + STREAM_STATUS_SUBPATH + twitchName + " HTTP/1.1\r\n" +
                   "Host: " + STREAM_STATUS_HOST + "\r\n" +
                   "Connection: close\r\n\r\n");

    updateState = STATE_WAITING_FOR_STREAM_STATUS;
}

void TwitchStreamerInfo::actionStateWaitingForStreamStatus()
{
    static uint16_t timeout = 0;
    static uint16_t delayTimeout = 0;

    if(client.available() == 0)
    {
        if(timeout < TIMEOUT_STREAM_STATUS_MS)
        {
            timeout += (timeNew - timeOld);
        }
        else
        {
            debugPrint("Stream status request timeout\n");
            error = ERROR_TIMEOUT_STREAM_STATUS;
            updateState = STATE_ERROR;
            client.stop();
            timeout = 0;
        }
    }
    else
    {
        //extra delay here to allow client to populate with all data before parsing
        if(delayTimeout < DELAY_STREAM_STATUS_MS)
        {
            delayTimeout += (timeNew - timeOld);
        }
        else
        {
            debugPrint("Stream status url request successful\n");
            updateState = STATE_PARSING_JSON_STREAM_STATUS;
            delayTimeout = 0;
        }
        timeout = 0;
    }
}

void TwitchStreamerInfo::actionStateParsingJsonStreamStatus()
{
    bool done = false;
    while(client.available() && !done)
    {
        if(client.peek() != '{')
        {
            client.read();
        }
        else
        {
            done = true;
        }
    }

    if(client.available())
    {
      jsonBuffer.clear();
      JsonObject& root = jsonBuffer.parseObject(client);

      if(!root.success())
      {
          debugPrint("Stream status json parsing failed\n");
          error = ERROR_PARSING_JSON_STREAM_STATUS;
          updateState = STATE_ERROR;
      }
      else
      {
          JsonVariant tempVariant = root["stream"]["channel"]["game"];
          if(tempVariant.success())
          {
              online = true;
              twitchGameName = String(tempVariant.as<char*>());
              debugPrint("Is online, playing: ");
              debugPrint(twitchGameName);
              debugPrint("\n");
          }
          else
          {
              debugPrint("Not Online\n");
              online = false;
              twitchGameName = "";
          }

          updateState = STATE_CONNECTING_TO_HOST_LOOKUP;
      }

    }
    else
    {
        debugPrint("Stream status response did not include Json Object\n");
        error = ERROR_NO_JSON_STREAM_STATUS;
        updateState = STATE_ERROR;
    }

    client.stop();
}

void TwitchStreamerInfo::actionStateConnectingToHostLookup()
{
    debugPrint("Connecting to ");
    debugPrint(HOST_LOOKUP_HOST);
    debugPrint("\n");
    if(!client.connect(HOST_LOOKUP_HOST, 443))
    {
        debugPrint("Connection Failed\n");
        error = ERROR_CONNECTION_HOST_LOOKUP;
        updateState = STATE_ERROR;
    }
    else
    {
        debugPrint("Connection Successful\n");
        updateState = STATE_REQUESTING_PATH_HOST_LOOKUP;
    }
}

void TwitchStreamerInfo::actionStateRequestingPathHostLookup()
{
    debugPrint("Requsting URL: ");
    debugPrint(HOST_LOOKUP_SUBPATH);
    debugPrint(twitchId);
    debugPrint("\n");

    client.print(String("GET ") + HOST_LOOKUP_SUBPATH + twitchId + " HTTP/1.1\r\n" +
                   "Host: " + HOST_LOOKUP_HOST + "\r\n" +
                   "Connection: close\r\n\r\n");

    updateState = STATE_WAITING_FOR_HOST_LOOKUP;
}

void TwitchStreamerInfo::actionStateWaitingForHostLookup()
{
    static uint16_t timeout = 0;
    static uint16_t delayTimeout = 0;

    if(client.available() == 0)
    {
        if(timeout < TIMEOUT_HOST_LOOKUP_MS)
        {
            timeout += (timeNew - timeOld);
        }
        else
        {
            debugPrint("Host lookup request timeout\n");
            error = ERROR_TIMEOUT_HOST_LOOKUP;
            updateState = STATE_ERROR;
            client.stop();
            timeout = 0;
        }
    }
    else
    {
        //extra delay here to allow client to populate with all data before parsing
        if(delayTimeout < DELAY_HOST_LOOKUP_MS)
        {
            delayTimeout += (timeNew - timeOld);
        }
        else
        {
            debugPrint("Host lookup url request successful\n");
            updateState = STATE_PARSING_JSON_HOST_LOOKUP;
            delayTimeout = 0;
        }
        timeout = 0;
    }
}

void TwitchStreamerInfo::actionStateParsingJsonHostLookup()
{
    bool done = false;
    while(client.available() && !done)
    {
        if(client.peek() != '{')
        {
            client.read();
        }
        else
        {
            done = true;
        }
    }

    if(client.available())
    {
      jsonBuffer.clear();
      JsonObject& root = jsonBuffer.parseObject(client);

      if(!root.success())
      {
          debugPrint("Host lookup json parsing failed\n");
          error = ERROR_PARSING_JSON_HOST_LOOKUP;
          updateState = STATE_ERROR;
      }
      else
      {
          JsonVariant tempVariant = root["hosts"][0]["target_login"];
          if(tempVariant.success())
          {
              hosting = true;
              hostName = String(tempVariant.as<char*>());
              debugPrint("Is hosting: ");

              tempVariant = root["hosts"][0]["target_display_name"];
              if(tempVariant.success())
              {
                  hostDisplayName = String(tempVariant.as<char*>());
                  debugPrint(hostDisplayName);
                  debugPrint("\n");
              }
              else
              {
                  hostDisplayName = hostName;
                  debugPrint(hostDisplayName);
                  debugPrint(" (<- login name... could not find display name)\n");
              }
              updateState = STATE_CONNECTING_TO_HOST_STATUS;
          }
          else
          {
              debugPrint("Not hosting\n");
              hosting = false;
              hostName = "";
              hostDisplayName = "";
              updateState = STATE_DONE;
          }
      }
    }
    else
    {
        debugPrint("Host lookup response did not include Json Object\n");
        error = ERROR_NO_JSON_HOST_LOOKUP;
        updateState = STATE_ERROR;
    }

    client.stop();
}

void TwitchStreamerInfo::actionStateConnectingToHostStatus()
{
    debugPrint("Connecting to ");
    debugPrint(STREAM_STATUS_HOST);
    debugPrint("\n");
    if(!client.connect(STREAM_STATUS_HOST, 443))
    {
        debugPrint("Connection Failed\n");
        error = ERROR_CONNECTION_HOST_STATUS;
        updateState = STATE_ERROR;
    }
    else
    {
        debugPrint("Connection Successful\n");
        updateState = STATE_REQUESTING_PATH_HOST_STATUS;
    }
}

void TwitchStreamerInfo::actionStateRequestingPathHostStatus()
{
    debugPrint("Requsting URL: ");
    debugPrint(STREAM_STATUS_SUBPATH);
    debugPrint(hostName);
    debugPrint("\n");

    client.print(String("GET ") + STREAM_STATUS_SUBPATH + hostName + " HTTP/1.1\r\n" +
                   "Host: " + STREAM_STATUS_HOST + "\r\n" +
                   "Connection: close\r\n\r\n");

    updateState = STATE_WAITING_FOR_HOST_STATUS;
}

void TwitchStreamerInfo::actionStateWaitingForHostStatus()
{
    static uint16_t timeout = 0;
    static uint16_t delayTimeout = 0;

    if(client.available() == 0)
    {
        if(timeout < TIMEOUT_HOST_STATUS_MS)
        {
            timeout += (timeNew - timeOld);
        }
        else
        {
            debugPrint("Host status request timeout\n");
            error = ERROR_TIMEOUT_HOST_STATUS;
            updateState = STATE_ERROR;
            client.stop();
            timeout = 0;
        }
    }
    else
    {
        //extra delay here to allow client to populate with all data before parsing
        if(delayTimeout < DELAY_HOST_STATUS_MS)
        {
            delayTimeout += (timeNew - timeOld);
        }
        else
        {
            debugPrint("Host status url request successful\n");
            updateState = STATE_PARSING_JSON_HOST_STATUS;
            delayTimeout = 0;
        }
        timeout = 0;
    }
}

void TwitchStreamerInfo::actionStateParsingJsonHostStatus()
{
    bool done = false;
    while(client.available() && !done)
    {
        if(client.peek() != '{')
        {
            client.read();
        }
        else
        {
            done = true;
        }
    }

    if(client.available())
    {
      jsonBuffer.clear();
      JsonObject& root = jsonBuffer.parseObject(client);

      if(!root.success())
      {
          debugPrint("Host status json parsing failed\n");
          error = ERROR_PARSING_JSON_HOST_STATUS;
          updateState = STATE_ERROR;
      }
      else
      {
          JsonVariant tempVariant = root["stream"]["channel"]["game"];
          if(tempVariant.success())
          {
              hostGameName = String(tempVariant.as<char*>());
              debugPrint("Host is playing: ");
              debugPrint(hostGameName);
              debugPrint("\n");
          }
          else
          {
              debugPrint("Could not find host's game name\n");
              hostGameName = "";
          }

          updateState = STATE_DONE;
      }

    }
    else
    {
        debugPrint("Stream status response did not include Json Object\n");
        error = ERROR_NO_JSON_STREAM_STATUS;
        updateState = STATE_ERROR;
    }

    client.stop();
}
