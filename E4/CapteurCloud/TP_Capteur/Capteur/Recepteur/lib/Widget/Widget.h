#ifndef WIDGET_H
#define WIDGET_H

#define mPI 3.1415926535
#define mHALF_PI 1.5707963267
#define mTWO_PI 6.2831853071

#define chardegree (char)247
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern float TEMPERATURE;
extern float HUMIDITY;


//*********************
// gauge widget temperature
//*********************
class Gauge
{

public:
    // constructor
    Gauge(Adafruit_GFX *pdisplay, uint16_t centerX, uint16_t centerY, uint16_t radius);
    void draw();
    float _calculX(float rayon, float angle);
    float _calculY(float rayon, float angle);

private:
    void _drawBackground();
    void _drawtemp();
    uint16_t _radius;
    uint16_t _centerX;
    uint16_t _centerY;
    float _temp;
    Adafruit_GFX *_canvas;
};


class Gauge_hum
{
public:
    // constructor
    Gauge_hum(Adafruit_GFX *pdisplay);
    void draw();

private:
    void _drawBackground();
    void _drawhum();
    float _hum;
    Adafruit_GFX *_canvas;
};

#endif