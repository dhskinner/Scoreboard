#include <ScoreboardWebClass.h>

#ifdef ESP8266
// ---------------------------------------------------------------------------------
void WebLedStart ( uint8_t pin )
{
    // Set up a LED
    onboardLed.Start ( pin );
}
void WebLedOn()
{
    onboardLed.On();
    i2cio.LedOn ( I2C_LED_BLUE_PIN );
}
void WebLedOff()
{
    onboardLed.Off();
    i2cio.LedOff ( I2C_LED_BLUE_PIN );
}
void WebLedToggle()
{
    i2cio.LedToggle ( I2C_LED_BLUE_PIN );
}
void WebLedFlash()
{
    // Flash the onboard LED
    onboardLed.Flash();
    i2cio.LedOff ( I2C_LED_BLUE_PIN );
}
void WebLedBlink()
{
    // Set the led to blinking
    onboardLed.Blink();
    i2cio.LedOff ( I2C_LED_BLUE_PIN );
}
// ---------------------------------------------------------------------------------
ScoreboardWebClass::ScoreboardWebClass ( uint16_t listenport ) : _server ( listenport )
{
    _isAccessPoint = true;
}
// ---------------------------------------------------------------------------------
void ScoreboardWebClass::Start ( uint8_t ledpin, bool isAccessPoint )
{
    WebLedStart ( ledpin );

    WSUtility::PrintSystemSpecs();

    _isAccessPoint = isAccessPoint;
    if ( _isAccessPoint )
        this->ConnectAP();		// Start the access point and captive portal
    else
        this->ConnectWifi();	// Connect to Wifi

    // Setup file system
    SPIFFS.begin();
    {
        Dir dir = SPIFFS.openDir ( "/" );

        USTREAM ( F ( "Found files:" ) << endl );
        while ( dir.next() )
            USTREAM ( F ( "  - " ) << dir.fileName() << F ( " (" ) << dir.fileSize() << F ( " bytes)" ) << endl );
        USTREAM ( endl );
    }

    // Setup callbacks - can be HTTP_GET, HTTP_PUT, HTTP_POST
    _server.on ( "/", std::bind ( &ScoreboardWebClass::HandleRoot, this ) );
    _server.on ( "/json", std::bind ( &ScoreboardWebClass::HandleGetJSON, this ) );
    _server.on ( "/score", std::bind ( &ScoreboardWebClass::HandleSetScore, this ) );
    _server.on ( "/timer", std::bind ( &ScoreboardWebClass::HandleSetTimer, this ) );
    _server.on ( "/clock", std::bind ( &ScoreboardWebClass::HandleSetClock, this ) );
    _server.on ( "/mode", std::bind ( &ScoreboardWebClass::HandleSetMode, this ) );
    _server.on ( "/restart", std::bind ( &ScoreboardWebClass::HandleRestart, this ) );
    //_server.on ( "/index.htm", std::bind ( &ScoreboardWebClass::HandleRoot, this ) );
    //_server.on ( "/index.html", std::bind ( &ScoreboardWebClass::HandleRoot, this ) );
    //_server.on ( "/apple-touch-icon.png", std::bind ( &ScoreboardWebClass::HandleAppleTouchIcon, this ) );
    //_server.on ( "/favicon-16x16.png", std::bind ( &ScoreboardWebClass::HandleFavicon16, this ) );
    //_server.on ( "/favicon-32x32.png", std::bind ( &ScoreboardWebClass::HandleFavicon32, this ) );
    //_server.on ( "/browserconfig.xml", std::bind ( &ScoreboardWebClass::HandleBrowserConfig, this ) );
    //_server.on ( "/favicon.ico", std::bind ( &ScoreboardWebClass::HandleFavicon, this ) );
    //_server.on ( "/logo.jpg", std::bind ( &ScoreboardWebClass::HandleLogo, this ) );
    //_server.on ( "/manifest.json", std::bind ( &ScoreboardWebClass::HandleManifest, this ) );
    //_server.on ( "/mstile-150x150.png", std::bind ( &ScoreboardWebClass::HandleMstile, this ) );
    //_server.on ( "/safari-pinned-tab.svg", std::bind ( &ScoreboardWebClass::HandleSafari, this ) );

    _server.on ( "/hotspot-detect.html", std::bind ( &ScoreboardWebClass::HandleCNA, this ) );
    _server.on ( "/library/test/success.html", std::bind ( &ScoreboardWebClass::HandleCNA, this ) );

    _server.serveStatic ( "/redirect.html", SPIFFS, "/redirect.html" );
    _server.serveStatic ( "/safari-pinned-tab.svg", SPIFFS, "/safari-pinned-tab.svg", "max-age=86400" );
    _server.serveStatic ( "/mstile-150x150.png", SPIFFS, "/mstile-150x150.png", "max-age=86400" );
    _server.serveStatic ( "/manifest.json", SPIFFS, "/manifest.json", "max-age=86400" );
    _server.serveStatic ( "/logo.jpg", SPIFFS, "/logo.jpg", "max-age=86400" );
    _server.serveStatic ( "/favicon.ico", SPIFFS, "/favicon.ico", "max-age=86400" );
    _server.serveStatic ( "/browserconfig.xml", SPIFFS, "/browserconfig.xml", "max-age=86400" );
    _server.serveStatic ( "/favicon-32x32.png", SPIFFS, "/favicon-32x32.png", "max-age=86400" );
    _server.serveStatic ( "/favicon-16x16.png", SPIFFS, "/favicon-16x16.png", "max-age=86400" );
    _server.serveStatic ( "/apple-touch-icon.png", SPIFFS, "/apple-touch-icon.png", "max-age=86400" );
    _server.serveStatic ( "/index.html", SPIFFS, "/index.html" );
    _server.serveStatic ( "/index.htm", SPIFFS, "/index.html" );
    //server.serveStatic("/fonts", SPIFFS, "/fonts", "max-age=86400");
    //server.serveStatic("/js", SPIFFS, "/js");
    //server.serveStatic("/css", SPIFFS, "/css", "max-age=86400");
    //server.serveStatic("/images", SPIFFS, "/images", "max-age=86400");
    //server.serveStatic("/", SPIFFS, "/index.htm");

    _server.onNotFound ( std::bind ( &ScoreboardWebClass::HandleNotFound, this ) );
    _server.begin();

    UPRINTLN ( F ( "HTTP server started" ) );
}

// ---------------------------------------------------------------------------------
// Handle requests from Apple and Android devices while in CNA
void ScoreboardWebClass::HandleCNA()
{
    WebLedOn();
    _server.send_P ( 200, ( PGM_P ) SUCCESS_HTML_CONTENT, ( PGM_P ) SUCCESS_HTML, SUCCESS_HTML_SIZE );
    WebLedBlink();
}
// ---------------------------------------------------------------------------------
void ScoreboardWebClass::ConnectAP()
{
    UPRINTLN ( "Starting access point" );
    WebLedFlash();

    // Start the access point
    IPAddress accessPointIP ( WEB_AP_IPADDRESS_1, WEB_AP_IPADDRESS_2, WEB_AP_IPADDRESS_3, WEB_AP_IPADDRESS_4 );
    WiFi.mode ( WIFI_AP );
    WiFi.softAPConfig ( accessPointIP, accessPointIP, IPAddress ( 255, 255, 255, 0 ) );
    WiFi.softAP ( WEB_AP_SSID, WEB_AP_PASSWORD );

    // if DNSServer is started with "*" for domain name, it will reply with the provided IP to all DNS requests
    _dnsServer.start ( WEB_AP_DNS_PORT, "*", accessPointIP );
    USTREAM ( F ( "Started access point with captive portal at " ) << accessPointIP << endl );

    WebLedBlink();
}
// ---------------------------------------------------------------------------------
void ScoreboardWebClass::ConnectWifi()
{
    // Connect to WiFi
    UPRINTLN ( "Connecting to wifi" );
    WebLedFlash();

    WiFi.begin ( WEB_STA_SSID, WEB_STA_PASSWORD );
    while ( WiFi.status() != WL_CONNECTED )
    {
        delay ( 500 );
        UPRINT ( "." );
        WebLedToggle();
    }

    USTREAM ( endl << F ( "Connected to " ) << WEB_STA_SSID << endl );
    USTREAM ( F ( "IP address: " ) << WiFi.localIP() << endl );

    // Start mDNS
    if ( MDNS.begin ( WEB_MDNS_HOSTNAME ) )
    {
        UPRINTLN ( F ( "MDNS responder started" ) );
    }

    WebLedBlink();
}
// ---------------------------------------------------------------------------------
bool ScoreboardWebClass::Connected()
{
    if ( WiFi.status() == WL_CONNECTED || _isAccessPoint == true )
        return true;

    return false;
}
// ---------------------------------------------------------------------------------
// Request format for root is:
// http://url/
// http://url/index.htm
// http://url/index.html
void ScoreboardWebClass::HandleRoot()
{
    WebLedOn();
    this->HandleFileRead ( F ( "/index.html" ) );
    WebLedBlink();
}
// ---------------------------------------------------------------------------------
void ScoreboardWebClass::Process()
{
    if ( _isAccessPoint )
        _dnsServer.processNextRequest();
    _server.handleClient();
}

// ---------------------------------------------------------------------------------
// Request format for json or jsonp is HTTP_GET:
// http://url/json?field=[n] (for json)
// http://url/json?field=[n]&jsonp=[callbackfunction] (for jsonp)
void ScoreboardWebClass::HandleGetJSON()
{
    String str;
    uint8_t fieldID;

    WebLedOn();

    // get the requested field?
    fieldID = ParseField();
    if ( fieldID == ERROR_VALUE )
    {
        WebLedBlink();
        return;
    }

    // get json
    str = FormatJSON ( fieldID, true );
    _server.send ( HTTP_STATUS_OK, str_content_json, str );
    WebLedBlink();
}
// ---------------------------------------------------------------------------------
// Format to set the score is HTTP_PUT:
// http://url/score?field=n&score1=[n]&score2=[n]
void ScoreboardWebClass::HandleSetScore()
{
    String arg;
    uint8_t value;
    uint8_t fieldID;

    WebLedOn();

    // get the requested field?
    fieldID = ParseField();
    if ( fieldID == ERROR_VALUE )
    {
        WebLedBlink();
        return;
    }

    // set score 1?
    arg = _server.arg ( str_score1 );
    value = arg.toInt();
    if ( arg != String() )
    {
        if ( value <= MAX_SCORE )
            scoreboard[fieldID].Score1 ( value );
        else
        {
            _server.send ( HTTP_STATUS_BAD_REQUEST );
            WebLedBlink();
            return;
        }
    }

    // set score 2?
    arg = _server.arg ( str_score2 );
    value = arg.toInt();
    if ( arg != String() )
    {
        if ( value <= MAX_SCORE )
            scoreboard[fieldID].Score2 ( value );
        else
        {
            _server.send ( HTTP_STATUS_BAD_REQUEST );
            WebLedBlink();
            return;
        }
    }

    // send a response
    SendRadioCommand ( SET_SCORE, fieldID );
    WebLedBlink();
}
// ---------------------------------------------------------------------------------
// Format to set the timer is HTTP_PUT:
// http://url/timer?field=n&mode=[stop,pause,run]
void ScoreboardWebClass::HandleSetTimer()
{
    String arg;
    uint8_t fieldID;

    WebLedOn();

    // get the requested field?
    fieldID = ParseField();
    if ( fieldID == ERROR_VALUE )
    {
        WebLedBlink();
        return;
    }

    // get the new timer state
    arg = _server.arg ( str_mode );
    if ( arg == str_run )
        scoreboard[fieldID].Run();
    else if ( arg == str_pause )
        scoreboard[fieldID].Pause();
    else if ( arg == str_stop )
        scoreboard[fieldID].Stop();
    else
    {
        _server.send ( HTTP_STATUS_BAD_REQUEST );
        WebLedBlink();
        return;
    }

    // send a response
    SendRadioCommand ( SET_TIMER, fieldID );
    WebLedBlink();
}
// ---------------------------------------------------------------------------------
// Format to set the clock is HTTP_PUT:
// http://url/clock?field=n&mode=[on,off]
void ScoreboardWebClass::HandleSetClock()
{
    String arg;
    uint8_t fieldID;

    WebLedOn();

    // get the requested field?
    fieldID = ParseField();
    if ( fieldID == ERROR_VALUE )
    {
        WebLedBlink();
        return;
    }

    // get the clock
    arg = _server.arg ( str_clock );
    if ( arg == str_on )
        scoreboard[fieldID].Clock ( true );
    else if ( arg == str_off )
        scoreboard[fieldID].Clock ( false );
    else
    {
        _server.send ( HTTP_STATUS_BAD_REQUEST );
        WebLedBlink();
        return;
    }

    // send a response
    SendRadioCommand ( SET_MODE, fieldID );
    WebLedBlink();
}
// ---------------------------------------------------------------------------------
// Format to set the mode is HTTP_PUT:
// http://url/mode?[clock=on,off]&[dual=on,off]&[swap=on,off]&[brightness=0-6]
void ScoreboardWebClass::HandleSetMode()
{
    String arg;
    uint8_t value;
    uint8_t fieldID;

    WebLedOn();

    // mode flags are always set for FIELD1 (any fieldID from JSON request is ignored)
    fieldID = FIELD1;

    // get the clock
    arg = _server.arg ( str_clock );
    if ( arg == str_on )
        scoreboard[fieldID].Clock ( true );
    else if ( arg == str_off )
        scoreboard[fieldID].Clock ( false );
    else
    {
        _server.send ( HTTP_STATUS_BAD_REQUEST );
        WebLedBlink();
        return;
    }

    // get dual mode
    arg = _server.arg ( str_dual );
    if ( arg == str_on )
        scoreboard[fieldID].Dual ( true );
    else if ( arg == str_off )
        scoreboard[fieldID].Dual ( false );
    else if ( arg != String() )
    {
        _server.send ( HTTP_STATUS_BAD_REQUEST );
        WebLedBlink();
        return;
    }

    // get swap mode
    arg = _server.arg ( str_swap );
    if ( arg == str_on )
        scoreboard[fieldID].Swap ( true );
    else if ( arg == str_off )
        scoreboard[fieldID].Swap ( false );
    else if ( arg != String() )
    {
        _server.send ( HTTP_STATUS_BAD_REQUEST );
        WebLedBlink();
        return;
    }

    // get brightness
    arg = _server.arg ( str_bright );
    value = arg.toInt();
    if ( arg != String() )
    {
        if ( value <= FLAG_BRIGHTNESS_6 )
            scoreboard[fieldID].Brightness ( value );
        else
        {
            _server.send ( HTTP_STATUS_BAD_REQUEST );
            WebLedBlink();
            return;
        }
    }

    // Send a response
    SendRadioCommand ( SET_MODE, fieldID );
    WebLedBlink();
}
// ---------------------------------------------------------------------------------
// Format IS:
// http://url/restart
void ScoreboardWebClass::HandleRestart()
{
    _server.send ( HTTP_STATUS_OK, str_content_plain, str_restart );
    ESP.restart();
}
// ---------------------------------------------------------------------------------
void ScoreboardWebClass::HandleNotFound()
{
    WebLedOn();
    //called when the url is not defined - check if there is a file to download?
    if ( !this->HandleFileRead ( _server.uri() ) )
    {
#if defined(SERIAL_DEBUG) || defined (CONSOLE_DEBUG)
        String message = F ( "File Not Found\n\r" );
        message += F ( "URI: " );
        message += _server.uri();
        message += F ( "\nMethod: " );
        message += ( _server.method() == HTTP_GET ) ? "GET" : "POST";
        message += F ( "\nArguments: " );
        message += _server.args();
        message += F ( "\n" );
        for ( uint8_t i = 0; i < _server.args(); i++ )
            message += " " + _server.argName ( i ) + ": " + _server.arg ( i ) + "\n";
        UPRINTLN ( message );
#endif
        // _server.send ( HTTP_STATUS_NOT_FOUND, str_content_plain, message );
        //_server.send_P ( 200, ( PGM_P ) REDIRECT_HTML_CONTENT, ( PGM_P ) REDIRECT_HTML, REDIRECT_HTML_SIZE );
        this->HandleFileRead ( F ( "/redirect.html" ) );
    }
    WebLedBlink();
}
// ---------------------------------------------------------------------------------
bool ScoreboardWebClass::HandleFileRead ( String path )
{
    File file = SPIFFS.open ( path, "r" );
    if ( file )		// Was the file found?
    {
        size_t sent = _server.streamFile ( file, FormatContentType ( path ) );
        file.close();
        return true;
    }
    return false;
}
// ---------------------------------------------------------------------------------
// send the command over the radio to the scoreboard
void ScoreboardWebClass::SendRadioCommand ( MessageID messageID, uint8_t fieldID )
{
    String str;

    if ( radio.Send ( messageID, RF24_SCOREBOARD_ADDRESS, fieldID ) )
        str = FormatJSON ( fieldID, true );
    else
        str = FormatJSON ( fieldID, false );

    _server.send ( HTTP_STATUS_OK, str_content_json, str );
}
// ---------------------------------------------------------------------------------
uint8_t ScoreboardWebClass::ParseField()
{
    uint8_t fieldID;
    String str;

    str = _server.arg ( str_field );
    fieldID = ( uint8_t ) str.toInt() - 1;
    // Is there a valid fieldID?
    if ( str == String() || fieldID >= FIELDS )
    {
        _server.send ( HTTP_STATUS_BAD_REQUEST );
        return ERROR_VALUE;
    }
    return fieldID;
}
// ---------------------------------------------------------------------------------
String ScoreboardWebClass::FormatJSON ( uint8_t fieldID, bool isOK )
{
    String out;
    ScoreboardDatagram datagram;
    DynamicJsonBuffer jsonBuffer;

    // Get the scoreboard data and convert it to JSON
    scoreboard[fieldID].GetDatagram ( &datagram );
    JsonObject& json = jsonBuffer.createObject();
    json[str_success] = isOK;
    json[str_msgid] = ( uint8_t ) BROADCAST;
    json[str_field] = fieldID + 1;
    json[str_score1] = scoreboard[fieldID].Score1();
    json[str_score2] = scoreboard[fieldID].Score2();
    json[str_matchtime] = scoreboard[fieldID].MatchTime();
    switch ( scoreboard[fieldID].MatchTimeMode() )
    {
    case FLAG_TIMER_RUN:
        json[str_timer] = str_run;
        break;
    case FLAG_TIMER_PAUSE:
        json[str_timer] = str_pause;
        break;
    case FLAG_TIMER_STOP:
    default:
        json[str_timer] = str_stop;
        break;
    }
    json[str_clock] = scoreboard[fieldID].Clock() ? str_on : str_off;
    json[str_dual] = scoreboard[fieldID].Dual() ? str_on : str_off;
    json[str_swap] = scoreboard[fieldID].Swap() ? str_on : str_off;
    json[str_bright] = scoreboard[fieldID].Brightness();
    json[str_signal] = radio.SignalQuality();
    json[str_battery] = i2cbattery.Charge();

    // is there a callback, to return jsonp?
    out = _server.arg ( str_jsonp );
    if ( out != String() )
    {
        out += F ( "(" );
        json.printTo ( out );
        out += F ( ")" );
    }
    else
        // otherwise return plain json
        json.printTo ( out );

    return out;
}
// ---------------------------------------------------------------------------------
String ScoreboardWebClass::FormatBytes ( size_t bytes )
{
    if ( bytes < 1024 )
        return String ( bytes ) + "B";
    else if ( bytes < ( 1024 * 1024 ) )
        return String ( bytes / 1024.0 ) + "KB";
    else if ( bytes < ( 1024 * 1024 * 1024 ) )
        return String ( bytes / 1024.0 / 1024.0 ) + "MB";
    else
        return String ( bytes / 1024.0 / 1024.0 / 1024.0 ) + "GB";
}
// ---------------------------------------------------------------------------------
String ScoreboardWebClass::FormatContentType ( String filename )
{
    if ( _server.hasArg ( "download" ) ) return str_content_stream;
    else if ( filename.endsWith ( ".htm" ) ) return str_content_html;
    else if ( filename.endsWith ( ".html" ) ) return str_content_html;
    else if ( filename.endsWith ( ".css" ) ) return str_content_css;
    else if ( filename.endsWith ( ".js" ) ) return str_content_js;
    else if ( filename.endsWith ( ".png" ) ) return str_content_png;
    else if ( filename.endsWith ( ".gif" ) ) return str_content_gif;
    else if ( filename.endsWith ( ".jpg" ) ) return str_content_jpg;
    else if ( filename.endsWith ( ".ico" ) ) return str_content_ico;
    else if ( filename.endsWith ( ".xml" ) ) return str_content_xml;
    else if ( filename.endsWith ( ".pdf" ) ) return str_content_pdf;
    else if ( filename.endsWith ( ".zip" ) ) return str_content_zip;
    else if ( filename.endsWith ( ".gz" ) ) return str_content_gzip;
    return str_content_plain;
}


/*
// ---------------------------------------------------------------------------------
// Request format for root is:
// http://url/
// http://url/index.htm
// http://url/index.html
void ScoreboardWebClass::HandleRoot()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) INDEX_HTML_CONTENT, ( PGM_P ) INDEX_HTML, INDEX_HTML_SIZE );
WebLedBlink();
}
// ---------------------------------------------------------------------------------
// apple-touch-icon.png
void ScoreboardWebClass::HandleAppleTouchIcon()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) APPLE_TOUCH_ICON_PNG_CONTENT, ( PGM_P ) APPLE_TOUCH_ICON_PNG, APPLE_TOUCH_ICON_PNG_SIZE );
WebLedBlink();
}
// ---------------------------------------------------------------------------------
// favicon-16x16.png
void ScoreboardWebClass::HandleFavicon16()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) FAVICON_16X16_PNG_CONTENT, ( PGM_P ) FAVICON_16X16_PNG, FAVICON_16X16_PNG_SIZE );
WebLedBlink();
}
// ---------------------------------------------------------------------------------
// favicon-32x32.png
void ScoreboardWebClass::HandleFavicon32()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) FAVICON_32X32_PNG_CONTENT, ( PGM_P ) FAVICON_32X32_PNG, FAVICON_32X32_PNG_SIZE );
WebLedBlink();
}
// ---------------------------------------------------------------------------------
// browserconfig.xml
void ScoreboardWebClass::HandleBrowserConfig()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) BROWSERCONFIG_XML_CONTENT, ( PGM_P ) BROWSERCONFIG_XML, BROWSERCONFIG_XML_SIZE );
WebLedBlink();
}
// ---------------------------------------------------------------------------------
// favicon.ico
void ScoreboardWebClass::HandleFavicon()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) FAVICON_ICO_CONTENT, ( PGM_P ) FAVICON_ICO, FAVICON_ICO_SIZE );
WebLedBlink();
}
// ---------------------------------------------------------------------------------
// logo.jpg
void ScoreboardWebClass::HandleLogo()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) LOGO_JPG_CONTENT, ( PGM_P ) LOGO_JPG, LOGO_JPG_SIZE );
WebLedBlink();
}
// ---------------------------------------------------------------------------------
// manifest.json
void ScoreboardWebClass::HandleManifest()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) MANIFEST_JSON_CONTENT, ( PGM_P ) MANIFEST_JSON, MANIFEST_JSON_SIZE );
WebLedBlink();
}
// ---------------------------------------------------------------------------------
// mstile-150x150.png
void ScoreboardWebClass::HandleMstile()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) MSTILE_150X150_PNG_CONTENT, ( PGM_P ) MSTILE_150X150_PNG, MSTILE_150X150_PNG_SIZE );
WebLedBlink();
}
// ---------------------------------------------------------------------------------
// safari-pinned-tab.svg
void ScoreboardWebClass::HandleSafari()
{
WebLedOn();
_server.send_P ( 200, ( PGM_P ) SAFARI_PINNED_TAB_SVG_CONTENT, ( PGM_P ) SAFARI_PINNED_TAB_SVG, SAFARI_PINNED_TAB_SVG_SIZE );
WebLedBlink();
}
*/
#endif
