/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>
#include <Mouse.h>
#include "bitmaps.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int buttonPin1 = 9;
const int buttonPin2 = 4;
const int buttonPin4 = 5;
const int buttonPin3 = 8;
const int buttonPin5 = 7;
const int buttonPin6 = 6;

#define MODE_DEFAULT 1
#define MODE_GAME 2
#define MODE_INFO 3
#define MODE_MOUSE 4
int mode = MODE_DEFAULT;

int currentDogFrame = 1;

void drawDogAnimation() {
  currentDogFrame++;
  if (currentDogFrame > 10) {
    currentDogFrame = 1;
  }
  display.clearDisplay();
  switch (currentDogFrame) {
    case 1: display.drawBitmap(55, 0, DogFrame1, 80, 32, 1); break;
    case 2: display.drawBitmap(55, 0, DogFrame2, 80, 32, 1); break;
    case 3: display.drawBitmap(55, 0, DogFrame3, 80, 32, 1); break;
    case 4: display.drawBitmap(55, 0, DogFrame4, 80, 32, 1); break;
    case 5: display.drawBitmap(55, 0, DogFrame5, 80, 32, 1); break;
    case 6: display.drawBitmap(55, 0, DogFrame6, 80, 32, 1); break;
    case 7: display.drawBitmap(55, 0, DogFrame7, 80, 32, 1); break;
    case 8: display.drawBitmap(55, 0, DogFrame8, 80, 32, 1); break;
    case 9: display.drawBitmap(55, 0, DogFrame9, 80, 32, 1); break;
    case 10: display.drawBitmap(55, 0, DogFrame10, 80, 32, 1); break;
  }

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  switch (mode) {
    case MODE_DEFAULT:
      display.println("NextWin PrtScr");
      display.println("NextApp MissCtrl");
      display.println("AppWins Fn");
      break;
    case MODE_GAME:
      display.println("Left  Enter");
      display.println("Down  Up");
      display.println("Right Fn");
      break;
    case MODE_INFO:
      display.println("Name   Email");
      display.println("Phone  Addr");
      display.println("UnLock Fn");
      break;
    case MODE_MOUSE:
      display.println("Left  Press");
      display.println("Down  Up");
      display.println("Right Fn");
      break;
  }

  display.display();
}

#define KEY_LEFT_CTRL     0x80
#define KEY_LEFT_SHIFT    0x81
#define KEY_LEFT_ALT      0x82
#define KEY_LEFT_GUI      0x83
#define KEY_UP_ARROW      0xDA
#define KEY_DOWN_ARROW    0xD9
#define KEY_LEFT_ARROW    0xD8
#define KEY_RIGHT_ARROW   0xD7
#define KEY_ESC           0xB1
#define KEY_HOME          0xD2
#define KEY_END           0xD5
#define KEY_INSERT        0xD1
#define KEY_DELETE        0xD4
#define KEY_TAB           0xB3
#define KEY_RETURN        0xB0

void setup() {
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin6, INPUT);
  Keyboard.begin();
  Mouse.begin();

  Serial.begin(9600);

  display.setRotation(2);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

}

void loop() {
  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);
  int buttonState3 = digitalRead(buttonPin3);
  int buttonState4 = digitalRead(buttonPin4);
  int buttonState5 = digitalRead(buttonPin5);
  int buttonState6 = digitalRead(buttonPin6);

  if (buttonState6 == HIGH && buttonState1 == HIGH) {
    mode = MODE_DEFAULT;
    return;
  }
  if (buttonState6 == HIGH && buttonState2 == HIGH) {
    mode = MODE_GAME;
    return;
  }
  if (buttonState6 == HIGH && buttonState3 == HIGH) {
    mode = MODE_INFO;
    return;
  }
  if (buttonState6 == HIGH && buttonState4 == HIGH) {
    mode = MODE_MOUSE;
    return;
  }

  switch (mode) {
    case MODE_DEFAULT:
      keyDefaultMode(buttonState1, buttonState2, buttonState3, buttonState4, buttonState5, buttonState6);
      break;
    case MODE_GAME:
      keyGameMode(buttonState1, buttonState2, buttonState3, buttonState4, buttonState5, buttonState6);
      break;
    case MODE_INFO:
      keyInfoMode(buttonState1, buttonState2, buttonState3, buttonState4, buttonState5, buttonState6);
      break;
    case MODE_MOUSE:
      keyMouseMode(buttonState1, buttonState2, buttonState3, buttonState4, buttonState5, buttonState6);
      break;
  }
  drawDogAnimation();
}

void keyDefaultMode(int buttonState1, int buttonState2, int buttonState3, int buttonState4, int buttonState5, int buttonState6) {
  // display.println("NextWin PrtScr");
  // display.println("NextApp MissCtrl");
  // display.println("AppWins Fn");
  if (buttonState1 == HIGH) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('`');
    delay(100);
    Keyboard.release('`');
  } else {
    Keyboard.release(KEY_LEFT_GUI);
  }

  if (buttonState2 == HIGH) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.write('5');
  } else {
    Keyboard.release(KEY_LEFT_GUI);
    Keyboard.release(KEY_LEFT_SHIFT);
  }

  if (buttonState3 == HIGH) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_TAB);
  } else {
    Keyboard.release(KEY_TAB);
    Keyboard.release(KEY_LEFT_GUI);
  }

  if (buttonState4 == HIGH) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_UP_ARROW);
  } else {
    Keyboard.release(KEY_UP_ARROW);
    Keyboard.release(KEY_LEFT_CTRL);
  }

  if (buttonState5 == HIGH) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_DOWN_ARROW);
  } else {
    Keyboard.release(KEY_DOWN_ARROW);
    Keyboard.release(KEY_LEFT_CTRL);
  }
}

void keyGameMode(int buttonState1, int buttonState2, int buttonState3, int buttonState4, int buttonState5, int buttonState6) {
  // display.println("Left  Enter");
  // display.println("Down  Up");
  // display.println("Right Fn");
  if (buttonState1 == HIGH) {
    Keyboard.press(KEY_LEFT_ARROW);
  } else {
    Keyboard.release(KEY_LEFT_ARROW);
  }

  if (buttonState2 == HIGH) {
    Keyboard.press(KEY_RETURN);
  } else {
    Keyboard.release(KEY_RETURN);
  }

  if (buttonState3 == HIGH) {
    Keyboard.press(KEY_DOWN_ARROW);
  } else {
    Keyboard.release(KEY_DOWN_ARROW);
  }

  if (buttonState4 == HIGH) {
    Keyboard.press(KEY_UP_ARROW);
  } else {
    Keyboard.release(KEY_UP_ARROW);
  }

  if (buttonState5 == HIGH) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.release(KEY_RIGHT_ARROW);
  }
}

int previousButtonState1 = HIGH;
int previousButtonState2 = HIGH;
int previousButtonState3 = HIGH;
int previousButtonState4 = HIGH;
int previousButtonState5 = HIGH;
void keyInfoMode(int buttonState1, int buttonState2, int buttonState3, int buttonState4, int buttonState5, int buttonState6) {
  // display.println("Name   Email");
  // display.println("Phone  Addr");
  // display.println("UnLock Fn");
  if ((buttonState1 != previousButtonState1) && (buttonState1 == HIGH)) {
    Keyboard.print("Taj Bas Thafnh Huy");
  }
  previousButtonState1 = buttonState1;

  if ((buttonState2 != previousButtonState2) && (buttonState2 == HIGH)) {
    Keyboard.print("huytest@gmail.com");
  }
  previousButtonState2 = buttonState2;

  if ((buttonState3 != previousButtonState3) && (buttonState3 == HIGH)) {
    Keyboard.print("0987654321");
  }
  previousButtonState3 = buttonState3;

  if ((buttonState4 != previousButtonState4) && (buttonState4 == HIGH)) {
    Keyboard.print("24 Lee DDinhf Dwowng, Phwowfng Phwowcs Ninh, Quaajn Hair Chaau, TP DDaf Nawxng");
  }
  previousButtonState4 = buttonState4;

  if ((buttonState5 != previousButtonState5) && (buttonState5 == HIGH)) {
    Keyboard.print("qweQWE!@#");
  }
  previousButtonState5 = buttonState5;
}

int mouseAcceleration = 0;
void keyMouseMode(int buttonState1, int buttonState2, int buttonState3, int buttonState4, int buttonState5, int buttonState6) {
  bool hasPressed = false;
  int mouseX = 0;
  int mouseY = 0;

  if (buttonState1 == HIGH) {
    mouseX = -1 * mouseAcceleration;
    hasPressed = true;
  }

  if (buttonState2 == HIGH) {
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.press(MOUSE_LEFT);
    }
    hasPressed = true;
  } else {
    if (Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.release(MOUSE_LEFT);
    }
  }

  if (buttonState3 == HIGH) {
    if (buttonState6 == HIGH) {
      Mouse.move(0, 0, 1);
    } else {
      mouseY = 1 * mouseAcceleration;
    }
    hasPressed = true;
  }

  if (buttonState4 == HIGH) {
    if (buttonState6 == HIGH) {
      Mouse.move(0, 0, -1);
    } else {
      mouseY = -1 * mouseAcceleration;
    }
    hasPressed = true;
  }

  if (buttonState5 == HIGH) {
    mouseX = 1 * mouseAcceleration;
    hasPressed = true;
  }

  if (!hasPressed) {
    mouseAcceleration = 0;
  } else if (mouseAcceleration < 15) {
    mouseAcceleration++;
  }

  if (hasPressed) {
    Mouse.move(mouseX, mouseY, 0);
  }

  delay(5);
}
