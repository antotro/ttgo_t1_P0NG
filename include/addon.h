#include <TFT_eSPI.h>

// barra, barras, barrad = bar, bar sx, bar dx
// palla, pallina = ball

class palla; // forward declaration

void resetScreen(TFT_eSPI screen)
{
    screen.fillScreen(TFT_BLACK);
}

void drawDline(TFT_eSPI screen)
{
    for (int i = 0; i < 134; i += 18)
    {
        screen.fillRect(119, i, 2, 9, TFT_WHITE);
    }
}

class barra
{
public:
    // variabili
    int posx;
    int posy;
    int width;
    int length;
    int color;
    int pin;
    int points;
    int shots;
    int randomseed = 40;

    // metodi
    barra(int, int, int, int, int, int);
    void disbarra(TFT_eSPI);
    void barrapunt();
    void disbarraauto(TFT_eSPI, palla);
};

barra::barra(int a, int b, int c, int d, int e, int p)
{
    posx = a;
    posy = b;
    width = c;
    length = d;
    color = e;
    pin = p;
}

void barra::disbarra(TFT_eSPI screen)
{
    screen.fillRect(posx, posy, width, length, TFT_BLACK);
    posy = map(analogRead(pin), 0, 4095, 0, 135 - length);
    screen.fillRect(posx, posy, width, length, color);
}

class palla
{
public:
    double posx;
    double posy;
    double velx;
    double vely;
    int color;
    int rad;
    int buzzer;
    double d;

    palla(double, double, int, int, int);
    void dPalla(TFT_eSPI, barra &, barra &);
    void resPalla();
    void Sound();
};

palla::palla(double a, double b, int c, int d, int bz)
{
    posx = a;
    posy = b;
    color = d;
    rad = c;
    buzzer = bz;
    velx = 0;
    while (velx == 0)
    {
        velx = (int)random(0, 3) - 1; // set a random horizontal direction at startup
    }
}

void palla::dPalla(TFT_eSPI screen, barra &barra1, barra &barra2)
{
    static unsigned long tempo = 0;
    static double level = 1;

    screen.fillCircle(posx, posy, rad, TFT_BLACK);

    if (posx <= barra1.posx + barra1.width + rad)
    {
        d = barra1.posy + barra1.length / 2 - posy;
        if (abs(d) <= barra1.length / 2)
        {
            barra1.shots++;
            if (d >= 0)
            {
                vely = -abs(d) / barra1.length * 2 * 1.5;
            }
            else
            {
                vely = abs(d) / barra1.length * 2 * 1.5;
            }
            velx = sqrt(4 - pow(vely, 2));
            Sound();
        }
        else if (abs(d) > barra1.length / 2)
        {
            level = 1;
            barra2.points++;
            barra1.shots = 0;
            barra2.shots = 0;
            resPalla();
            tempo = millis();
            while (millis() - tempo <= 1500)
            {
                drawDline(screen);
                barra1.disbarra(screen);
                barra2.disbarra(screen);
                screen.drawString("SCORE!", 130, 60, 4);
                delay(8);
            }
            resetScreen(screen);
            tempo = 0;
        }
    }

    if (posx >= barra2.posx - rad)
    {
        d = barra2.posy + barra2.length / 2 - posy;
        if (abs(d) <= barra2.length / 2)
        {
            barra2.shots++;
            if (d >= 0)
            {
                vely = -abs(d) / barra1.length * 2 * 1.5;
            }
            else
            {
                vely = abs(d) / barra1.length * 2 * 1.5;
            }
            velx = -sqrt(4 - pow(vely, 2));
            Sound();
        }
        else if (abs(d) > barra2.length / 2)
        {
            level = 1;
            barra1.points++;
            barra1.shots = 0;
            barra2.shots = 0;
            resPalla();
            tempo = millis();
            while (millis() - tempo <= 1500)
            {
                drawDline(screen);
                barra1.disbarra(screen);
                barra2.disbarra(screen);
                screen.drawString("SCORE!", 15, 60, 4);
                delay(8);
            }
            resetScreen(screen);
            tempo = 0;
        }
    }

    if (posy < rad || posy > 135 - rad)
    {
        vely = -vely;
        Sound();
    }

    screen.drawString(String(barra1.points), 60, 10, 2);
    screen.drawString(String(barra2.points), 180, 10, 2);

    if ((barra1.shots + barra2.shots + 1) % 10 == 0)
    {
        level += 0.01;
        barra1.shots++; // to exit the if statement
    }

    posx += velx * level;
    posy += vely * level;
    // Serial.println(level);
    screen.fillCircle(posx, posy, rad, color);
}

void palla::resPalla()
{
    posx = 120;
    posy = 67;
    vely = 0;
    if (velx > 0)        // reset at an horizontal lower speed towards loser's side
    {
        velx = 1;
    }
    if (velx < 0)
    {
        velx = -1;
    }
}

void barra::disbarraauto(TFT_eSPI screen, palla palla1)
{
    screen.fillRect(posx, posy, width, length, TFT_BLACK);
    if (palla1.posx >= randomseed)   // change the range of ball tracking zone
    {
        posy = palla1.posy - length / 2;
    }
    if (posy < palla1.rad)
    {
        posy = palla1.rad;
    }
    if (posy > 135 - length)
    {
        posy = 135 - length;
    }
    screen.fillRect(posx, posy, width, length, color);
}

void palla::Sound()
{
    static unsigned long time_;
    time_ = millis();
    while (millis() - time_ < 20)
    {
        ledcWrite(buzzer, 150);
    }
    ledcWrite(buzzer, 0);
}