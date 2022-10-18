#include <Arduino.h>
#include "addon.h"

TFT_eSPI tft = TFT_eSPI();
barra barras = barra(5, 5, 8, 40, TFT_GREENYELLOW, 13); //(upper_left_corner_posx, upper_left_corner_posy,width,length,color,pin attach)
barra barrad = barra(230, 5, 8, 40, TFT_ORANGE, 15);
palla pallina = palla(120, 67, 3, TFT_WHITE, 0); //(center_posx,center_posy,radius,color,buzzer channel)

int caso = 0;
int maxpoint = 0;
int oldmaxpoint = 0;
unsigned long lastpress = 0;

void setup()
{
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(3);
  resetScreen(tft);
  pinMode(0, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  tft.drawString("TTGO T1 PONG", 30, 60, 4);
  tft.drawString("made by Anto", 10, 120, 2);
  delay(1500);
  resetScreen(tft);
  pinMode(32, OUTPUT);   // pin of the buzzer
  ledcSetup(0, 5000, 8); // channel( to be provided to palla constructor),frequency(to set sound),resolution in bit
  ledcAttachPin(32, 0);  // pin and channel
  // Serial.begin(9600);
}

void loop()
{
  switch (caso)
  {
  case 0:
    tft.drawString("PRESS A BUTTTON", 10, 30, 4);
    tft.drawString("TO START", 70, 70, 4);
    tft.drawString("Select maximum score: ", 10, 100, 2);
    maxpoint = map(analogRead(15), 0, 4095, 1, 30);
    if (maxpoint != oldmaxpoint)
    {
      tft.fillRect(200, 90, 50, 50, TFT_BLACK);
      tft.drawString(String(maxpoint), 200, 100, 2);
      oldmaxpoint = maxpoint;
    }
    // Serial.print(digitalRead(0));
    // Serial.print("  ");
    // Serial.println(digitalRead(35));
    if (digitalRead(0) == 0 || digitalRead(35) == 0)
    {
      resetScreen(tft);
      delay(500);
      pallina.resPalla();
      pallina.color = TFT_WHITE;
      if (maxpoint == 17)
      {
        caso = 4;
        break;
      }
      caso = 1;
    }
    break;
  case 1:
    drawDline(tft);
    barras.disbarra(tft);
    barrad.disbarra(tft);
    pallina.dPalla(tft, barras, barrad);
    if (barras.points == maxpoint - 1 || barrad.points == maxpoint - 1)
    {
      pallina.color = TFT_RED;
    }
    if (barras.points >= maxpoint)
    {
      delay(500);
      resetScreen(tft);
      caso = 2;
    }
    if (barrad.points >= maxpoint)
    {
      delay(500);
      resetScreen(tft);
      caso = 3;
    }
    delay(8);
    break;
  case 2:
    barrad.points = 0;
    barras.points = 0;
    tft.drawString("PLAYER 1 WON!", 30, 60, 4);
    tft.drawString("Press a key to restart", 30, 110, 2);
    if (digitalRead(0) == 0 || digitalRead(35) == 0)
    {
      resetScreen(tft);
      delay(500);
      pallina.resPalla();
      caso = 0;
    }
    break;
  case 3:
    barrad.points = 0;
    barras.points = 0;
    tft.drawString("PLAYER 2 WON!", 30, 60, 4);
    tft.drawString("Press a key to restart", 30, 110, 2);
    if (digitalRead(0) == 0 || digitalRead(35) == 0)
    {
      resetScreen(tft);
      delay(500);
      pallina.resPalla();
      caso = 0;
    }
    break;
  case 4:
    drawDline(tft);
    barras.disbarraauto(tft, pallina);
    barrad.disbarra(tft);
    pallina.dPalla(tft, barras, barrad);
    if (barras.points == maxpoint - 1 || barrad.points == maxpoint - 1)
    {
      pallina.color = TFT_RED;
    }
    if (barras.points >= maxpoint)
    {
      delay(500);
      resetScreen(tft);
      caso = 2;
    }
    if (barrad.points >= maxpoint)
    {
      delay(500);
      resetScreen(tft);
      caso = 3;
    }
    delay(8);
    if (random(0, 50) == 5)
    {
      barras.randomseed = random(20, 120);
      // Serial.println(barras.randomseed);
    }

    break;
  }
} 