#include "twitchstreamerinfo.h"
#include "debugSerial.h"
#include <stdint.h>

#define SSID                        ("Zachmann")
#define PASSWORD                    ("bollY62417")
#define TIMEOUT_WIFI_MS             (5000)
#define TIMEOUT_STREAM_STATUS_MS    (5000)
#define DELAY_STREAM_STATUS_MS      (200)

#define STREAM_STATUS_HOST          ("wind-bow.glitch.me")
#define STREAM_STATUS_SUBPATH       ("/twitch-api/streams/")

void TwitchStreamerInfo::init()
{
    timeStamp = millis();
}

void TwitchStreamerInfo::run()
{
    static updateState_t updateState = STATE_CHECKING_WIFI_CONNECTION;
    static uint16_t timeOld, timeNew = millis();
    timeNew = millis();

    switch(updateState)
    {

    case STATE_CHECKING_WIFI_CONNECTION:
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
        break;
    }

    case STATE_CONNECTING_TO_WIFI:
    {
        debugPrint("Connecting to ");
        debugPrint(SSID);
        debugPrint("\n");
        WiFi.begin(SSID, PASSWORD);
        updateState = STATE_WAITING_FOR_WIFI_CONNECTION;
        break;
    }

    case STATE_WAITING_FOR_WIFI_CONNECTION:
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
        break;
    }

    case STATE_CONNECTING_TO_STREAM_STATUS:
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
        break;
    }

    case STATE_REQUESTING_PATH_STREAM_STATUS:
    {
        debugPrint("Requsting URL: ");
        debugPrint(STREAM_STATUS_SUBPATH);
        debugPrint(twitchName);
        debugPrint("\n");

        client.print(String("GET ") + STREAM_STATUS_SUBPATH + twitchName + " HTTP/1.1\r\n" +
                       "Host: " + STREAM_STATUS_HOST + "\r\n" +
                       "Connection: close\r\n\r\n");

        updateState = STATE_WAITING_FOR_STREAM_STATUS;
        break;
    }

    case STATE_WAITING_FOR_STREAM_STATUS:
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
        break;
    }

    case STATE_PARSING_JSON_STREAM_STATUS:
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
              JsonVariant tempVariant = root["stream"]["channel"]["_id"];
              if(tempVariant.success())
              {
                  online = true;
                  twitchId = String(tempVariant.as<char*>());
                  debugPrint("Is online, stream id: ");
                  debugPrint(twitchId);
                  debugPrint("\n");

                  tempVariant = root["stream"]["channel"]["game"];
                  if(tempVariant.success())
                  {
                      twitchGameName = String(tempVariant.as<char*>());
                      debugPrint("Is playing: ");
                      debugPrint(twitchGameName);
                      debugPrint("\n");
                  }
                  else
                  {
                      debugPrint("Could not find game name\n");
                      twitchGameName = "";
                  }

              }
              else
              {
                  debugPrint("Not Online\n");
                  online = false;
                  twitchId = "";
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

        break;
    }

    case STATE_ERROR:
    {
        debugPrint("Error State: ");
        debugPrint(error);
        debugPrint("\n");
        debugPrint("jumping back to start\n");
        updateState = STATE_CHECKING_WIFI_CONNECTION;
        error = ERROR_NO_ERROR;
        break;
    }

    default:
    {
        debugPrint("Not implemented yet, delaying and jumping back to start");
        delay(10000);
        updateState = STATE_CHECKING_WIFI_CONNECTION;
        break;
    }




    }
    timeOld = timeNew;
}
