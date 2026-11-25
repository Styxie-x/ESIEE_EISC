#include <Widget.h>

Gauge::Gauge(Adafruit_GFX *pdisplay, uint16_t centerX, uint16_t centerY, uint16_t radius)
{
    _radius = radius;
    _centerX = centerX;
    _centerY = centerY;
    _canvas = pdisplay;
    _temp = 0;
}

void Gauge ::_drawBackground()
{
    _canvas->setCursor(38, 2);
    _canvas->setTextSize(1);
    _canvas->setTextColor(WHITE);
    _canvas->print(F("TEMPERATURE"));

    // Demi-cercle propre pour la jauge de température.
    for (uint8_t i = 0; i <= 60; i += 10) {
        float angle = PI + (PI * i / 60.0f);
        int16_t x1 = _centerX + (int16_t)(_radius * cos(angle));
        int16_t y1 = _centerY + (int16_t)(_radius * sin(angle));
        int16_t x2 = _centerX + (int16_t)((_radius - 5) * cos(angle));
        int16_t y2 = _centerY + (int16_t)((_radius - 5) * sin(angle));
        _canvas->drawLine(x1, y1, x2, y2, WHITE);
    }

    _canvas->drawCircle(_centerX, _centerY, _radius, WHITE);
    _canvas->fillCircle(_centerX, _centerY, 4, WHITE);
}

void Gauge ::draw()
{
    _canvas->fillScreen(0);
    _drawBackground();
    _drawtemp();
}

void Gauge ::_drawtemp()
{
    float temp = TEMPERATURE;
    if (temp < 0.0f) temp = 0.0f;
    if (temp > 60.0f) temp = 60.0f;

    float angle = PI + (PI * temp / 60.0f);
    _canvas->drawLine(_centerX, _centerY,
                      _calculX(_radius - 7, angle),
                      _calculY(_radius - 7, angle), WHITE);

    _canvas->fillRect(43, 43, 42, 13, BLACK);
    _canvas->setCursor(44, 45);
    _canvas->setTextSize(2);
    _canvas->setTextColor(WHITE);
    _canvas->print(TEMPERATURE, 1);
    _canvas->setTextSize(1);
    _canvas->print(F(" C"));
}
float Gauge ::_calculX(float rayon, float angle)
{
    return _centerX + (rayon * cos(angle));
}
float Gauge ::_calculY(float rayon, float angle)
{
    return _centerY + (rayon * sin(angle));
}

Gauge_hum::Gauge_hum(Adafruit_GFX *pdisplay)
{
    _hum = 0;
    _canvas = pdisplay;
}

void Gauge_hum ::_drawBackground()
{
    _canvas->setCursor(48, 2);
    _canvas->setTextSize(1);
    _canvas->setTextColor(WHITE);
    _canvas->print(F("HUMIDITE"));

    // Jauge verticale simple, sans texte secondaire ni marqueurs parasites.
    _canvas->drawRect(54, 16, 20, 38, WHITE);
}

void Gauge_hum ::draw()
{
    _canvas->fillScreen(0);
    _drawBackground();
    _drawhum();
}

void Gauge_hum ::_drawhum()
{
    float humidity = HUMIDITY;
    if (humidity < 0.0f) humidity = 0.0f;
    if (humidity > 100.0f) humidity = 100.0f;

    int16_t hauteur = (int16_t)(humidity * 36.0f / 100.0f);
    if (hauteur > 0) {
        _canvas->fillRect(56, 52 - hauteur, 16, hauteur, WHITE);
    }

    // Une seule valeur entière, effacée dans une zone fixe avant impression.
    _canvas->fillRect(78, 30, 48, 18, BLACK);
    _canvas->setCursor(82, 34);
    _canvas->setTextSize(2);
    _canvas->setTextColor(WHITE);
    _canvas->print((int)(humidity + 0.5f));
    _canvas->setTextSize(1);
    _canvas->print(F(" %"));
}