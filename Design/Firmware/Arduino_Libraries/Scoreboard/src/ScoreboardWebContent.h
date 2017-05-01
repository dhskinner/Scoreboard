#ifndef _SCOREBOARDWEBCONTENT_h
#define _SCOREBOARDWEBCONTENT_h

#include <Arduino.h>

// These are defined as const char strings to save memory
// Not all of these are used (but the linker will strip out any unused strings)
static const char str_fieldtitle[] = "Field ";
static const char str_mode[] = "mode";
static const char str_jsonp[] = "jsonp";
static const char str_callback[] = "callback";
static const char str_field[] = "field";
static const char str_score1[] = "score1";
static const char str_score2[] = "score2";
static const char str_systime[] = "systime";
static const char str_matchtime[] = "matchtime";
static const char str_timer[] = "timer";
static const char str_run[] = "run";
static const char str_pause[] = "pause";
static const char str_stop[] = "stop";
static const char str_dual[] = "dual";
static const char str_swap[] = "swap";
static const char str_bright[] = "bright";
static const char str_signal[] = "signal";
static const char str_battery[] = "battery";
static const char str_clock[] = "clock";
static const char str_success[] = "success";
static const char str_ok[] = "ok";
static const char str_error[] = "error";
static const char str_msgid[] = "msgid";
static const char str_on[] = "on";
static const char str_off[] = "off";
static const char str_index[] = "/index.html";
static const char str_restart[] = "Restarting...";

static const char str_content_stream[] = "application/octet-stream";
static const char str_content_json[] = "application/json";
static const char str_content_js[] = "application/javascript";
static const char str_content_html[] = "text/html";
static const char str_content_css[] = "text/css";
static const char str_content_plain[] = "text/plain";
static const char str_content_png[] = "image/png";
static const char str_content_gif[] = "image/gif";
static const char str_content_jpg[] = "image/jpeg";
static const char str_content_ico[] = "image/x-icon";
static const char str_content_xml[] = "text/xml";
static const char str_content_pdf[] = "application/x-pdf";
static const char str_content_zip[] = "application/x-zip";
static const char str_content_gzip[] = "application/x-gzip";

#define HTTP_STATUS_OK					200
#define HTTP_STATUS_NOT_FOUND			404
#define HTTP_STATUS_BAD_REQUEST			400
#define HTTP_STATUS_SERVER_ERROR		500
#define HTTP_STATUS_SERVICE_UNAVAILABLE	503
#define HTTP_STATUS_NETWORK_TIMEOUT		599

static const uint8_t SUCCESS_HTML_CONTENT[] PROGMEM = { "text/html" };
static const uint8_t SUCCESS_HTML[] PROGMEM = { "<HTML><HEAD><TITLE>Success</TITLE></HEAD><BODY>Success</BODY></HTML>" };
static const uint32_t SUCCESS_HTML_SIZE = 68;

// External web files
/*
#include <index_html.h>
#include <apple-touch-icon_png.h>
#include <browserconfig_xml.h>
#include <favicon_ico.h>
#include <favicon-16x16_png.h>
#include <favicon-32x32_png.h>
#include <logo_jpg.h>
#include <manifest_json.h>
#include <mstile-150x150_png.h>
#include <safari-pinned-tab_svg.h>

#define FILENAME_STRINGS				10
#define FILENAME_MAX_LENGTH				25
#define FILETYPE_MAX_LENGTH				25

static const uint8_t REDIRECT_HTML_CONTENT[] PROGMEM = { "text/html" };
static const uint8_t REDIRECT_HTML[] PROGMEM = { "<!DOCTYPE html><html><head><title>Scoreboard</title></head><body><h1>Scoreboard</h1><p>Please open your web browser and go to http://scoreboard.local</p></body></html>" };
static const uint32_t REDIRECT_HTML_SIZE = 167;

static const char * str_filedata[] = 
{
	index_html,
	apple_touch_icon_png,
	browserconfig_xml,
	favicon_ico,
	favicon_16x16_png,
	favicon_32x32_png,
	logo_jpg,
	manifest_json,
	mstile_150x150_png,
	safari_pinned_tab_svg
};
*/
/*
static char const * const str_filename[]=
{
	"/index.html",
	"/apple_touch_icon.png",
	"/browserconfig.xml",
	"/favicon.ico",
	"/favicon_16x16.png",
	"/favicon_32x32.png",
	"/logo.jpg",
	"/manifest.json",
	"/mstile_150x150.png",
	"/safari_pinned_tab.svg"
};
*/
/*
static char const * const str_filetype[] =
{
	"text/html",
	"image/png",
	"text/xml",
	"image/x-icon",
	"image/png",
	"image/png",
	"image/jpeg",
	"application/json",
	"image/png",
	"application/octet-stream"
};
*/

#endif

