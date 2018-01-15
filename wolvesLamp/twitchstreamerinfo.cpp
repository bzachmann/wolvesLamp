#include "twitchstreamerinfo.h"
#include "debugSerial.h"
#include <stdint.h>

#define SSID                        ("Zachmann")
#define PASSWORD                    ("bollY62417")
#define TIMEOUT_WIFI_MS             (5000)
#define TIMEOUT_STREAM_STATUS_MS    (5000)
#define DELAY_STREAM_STATUS_MS      (200)
#define TIMEOUT_HOST_LOOKUP_MS      (5000)
#define DELAY_HOST_LOOKUP_MS        (200)

#define STREAM_STATUS_HOST          ("wind-bow.glitch.me")
#define STREAM_STATUS_SUBPATH       ("/twitch-api/streams/")
#define HOST_LOOKUP_HOST            ("tmi.twitch.tv")
#define HOST_LOOKUP_SUBPATH         ("/hosts?include_logins=1&host=")

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

        break;
    }

    case STATE_CONNECTING_TO_HOST_LOOKUP:
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

    case STATE_REQUESTING_PATH_HOST_LOOKUP:
    {
        debugPrint("Requsting URL: ");
        debugPrint(HOST_LOOKUP_SUBPATH);
        debugPrint(twitchId);
        debugPrint("\n");

        client.print(String("GET ") + HOST_LOOKUP_SUBPATH + twitchId + " HTTP/1.1\r\n" +
                       "Host: " + HOST_LOOKUP_HOST + "\r\n" +
                       "Connection: close\r\n\r\n");

        updateState = STATE_WAITING_FOR_HOST_LOOKUP;
        break;
    }

    case STATE_WAITING_FOR_HOST_LOOKUP:
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
        break;
    }

    case STATE_PARSING_JSON_HOST_LOOKUP:
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
              JsonVariant tempVariant = root["hosts"]["0"]["target_login"];
              if(tempVariant.success())
              {
                  hosting = true;
                  hostName = String(tempVariant.as<char*>());
                  debugPrint("Is hosting: ");

                  tempVariant = root["hosts"]["0"]["target_display_name"];
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
              }
              else
              {
                  debugPrint("Not hosting\n");
                  hosting = false;
                  hostName = "";
                  hostDisplayName = "";
              }

              updateState = STATE_CONNECTING_TO_HOST_STATUS;
          }
        }
        else
        {
            debugPrint("Host lookup response did not include Json Object\n");
            error = ERROR_NO_JSON_HOST_LOOKUP;
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
        debugPrint("Not implemented yet, delaying and jumping back to start\n");
        delay(10000);
        updateState = STATE_CHECKING_WIFI_CONNECTION;
        break;
    }




    }
    timeOld = timeNew;
}
