#include "WiFiTask.h"

DNSServer dnsServer;
WebServer server(80);

IotWebConf *iotWebConf;

/**
 * Handle web requests to "/" path.
 */
    void handleRoot()
{
    // -- Let IotWebConf test and handle captive portal requests.
    if (iotWebConf->handleCaptivePortal())
    {
        // -- Captive portal request were already served.
        return;
    }
    String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
    s += "<title>IotWebConf 01 Minimal</title></head><body>Hello world!";
    s += "Go to <a href='config'>configure page</a> to change settings.";
    s += "</body></html>\n";

    server.send(200, "text/html", s);
}

void WiFiTask::setup()
{
    // -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.  It's the password too.
    char thingName[33];
    snprintf(thingName, 33, "smartclock-%X", ESP.getChipId());

    iotWebConf = new IotWebConf(thingName, &dnsServer, &server, "password");
    // -- Initializing the configuration.
    iotWebConf->init();

    // -- Set up required URL handlers on the web server.
    server.on("/", handleRoot);
    server.on("/config", [] { iotWebConf->handleConfig(); });
    server.onNotFound([]() { iotWebConf->handleNotFound(); });
    wifi = iotWebConf;
}

void WiFiTask::loop()
{
    iotWebConf->doLoop();
    yield();
}

byte WiFiTask::WiFiState()
{
    return iotWebConf->getState();
}