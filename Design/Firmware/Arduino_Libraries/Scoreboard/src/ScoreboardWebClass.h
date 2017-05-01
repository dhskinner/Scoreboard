/*
// Request format for root is:
// http://url/

Request format for json or jsonp is HTTP_GET:
	http://url/json?field=[n] (for json)
	http://url/json?field=[n]&callback=[callbackfunction] (for jsonp) or
	http://url/jsonp?field=[n]&callback=[callbackfunction] (also for jsonp)
*/

#ifndef _SCOREBOARDWEBCLASS_h
#define _SCOREBOARDWEBCLASS_h

#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG
#endif

#ifndef CONSOLE_DEBUG
#define CONSOLE_DEBUG
#endif

#ifdef ESP8266

extern "C" {
#include "user_interface.h"
}

#include <Arduino.h>
#include <ArduinoJson.h>		// Download from https://github.com/bblanchon/ArduinoJson
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#include <WSUtility.h>
#include <ScoreboardDefines.h>
#include <ScoreboardClass.h>
#include <ScoreboardIOClass.h>
#include <ScoreboardI2CClass.h>
#include <ScoreboardRadioClass.h>
#include <ScoreboardWebContent.h>

// ---------------------------------------------------------------------------------
extern ScoreboardClass scoreboard[FIELDS];
extern ScoreboardRadioClass radio;
extern ScoreboardIOLEDClass onboardLed;
// ---------------------------------------------------------------------------------

class ScoreboardWebClass
{
protected:
    ESP8266WebServer _server;
    DNSServer _dnsServer;
    bool _isAccessPoint;

    uint8_t ParseField();

    void HandleRoot();
    void HandleGetJSON();
    void HandleSetScore();
    void HandleSetTimer();
    void HandleSetClock();
    void HandleSetMode();
    void HandleRestart();
    void HandleNotFound();
    bool HandleFileRead ( String path );
    void SendRadioCommand ( MessageID messageID, uint8_t fieldID );
	void HandleCNA();

    //void HandleAppleTouchIcon();		// apple-touch-icon.png
    //void HandleFavicon16();			// favicon-16x16.png
    //void HandleFavicon32();			// favicon-32x32.png
    //void HandleBrowserConfig();		// browserconfig.xml
    //void HandleFavicon();				// favicon.ico
    //void HandleLogo();				// logo.jpg
    //void HandleManifest();			// manifest.json
    //void HandleMstile();				// mstile-150x150.png
    //void HandleSafari();				// safari-pinned-tab.svg

    String FormatJSON ( uint8_t fieldID, bool isOK );
    String FormatBytes ( size_t bytes );
    String FormatContentType ( String filename );

public:
    ScoreboardWebClass ( uint16_t listenport );
    void Start ( uint8_t ledpin, bool isAccessPoint = true );
    void ConnectWifi();
    void ConnectAP();
    bool Connected();
    void Process();
};

#endif

#endif

