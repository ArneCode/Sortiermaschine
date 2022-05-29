/**
   @file sketch.ino
   @brief Hauptdatei, wichtigste Funktionen sind setup() und loop()
*/
#include "customServo.h"
#include "animLcd.h"
#include "callHandler.h"
#include "header.h"
/**
   @brief Gibt an, wie lange die Ladeanimation beim "Hochfahren" dauert

*/
const int LOADING_DURATION = 3000;
/**
   @brief Winkel des linken Lochs in Grad

*/
const int ANGLE_LEFT_HOLE = 180;
/**
   @brief Winkel des rechten Lochs in Grad

*/
const int ANGLE_RIGHT_HOLE = 90;
/**
   @brief Winkel der Ablagefläche für neue Bälle, die sortiert werden sollen

*/
const int ANGLE_CENTER = 130;
/**
   @brief Der kleinste sichere Winkel
   @warning Wenn dieser Winkel nicht eingehalten wird schlägt der Arm gegen den Stopper
*/
const int ANGLE_MIN = 45;
/**
   @brief Der Pin an dem der Servomotor angeschlossen ist

*/
const int PIN_SERVO = 6;
/**
   @brief Der Pin an dem der Start/Stop Knopf angeschlossen ist

*/
const int PIN_STOPBUTTON = 13;
/**
   @brief Der Pin um die Rotfärbung der RGB-Led zu steuern

*/
const int PIN_RED = 11;
/**
   @brief Der Pin um die Grünfärbung der RGB-Led zu steuern

*/
const int PIN_GREEN = 10;
/**
   @brief Der Pin um die Blaufärbung der RGB-Led zu steuern

*/
const int PIN_BLUE = 9;
/**
   @brief Die normale Geschwindigkeit des Servos

*/
const float SERVO_SPEED_DEFAULT = 0.01f;
/**
   @brief Die "schnelle" Geschwindigkeit des Servos
   @warning Servo bewegt sich hier nicht mit Maximalgeschwindigkeit. Wenn diese Geschwindigkeit schneller eingestellt wird als der Servo tatsächlich ist, hört er möglicherweise auf sich zu bewegen, bevor er an seinem Ziel angekommen ist
   Alternativ könnte CustomServo::writeDirect benutzt werden
*/
const float SERVO_SPEED_FAST = 0.5f;
/**
   @brief Der animierbare Lcd

*/
AnimatableLcd lcd(0x27, 16, 2);
/**
   @brief Die CallHandler Instanz

*/
CallHandler callHandler;
/**
   @brief Der Servo, eine CustomServo Instanz

*/
CustomServo servo;

/**
   @brief Die Anzahl weißer Bälle, die schon sortiert wurden
*/
int nWhite = 0;
/**
   @brief Die Anzahl schwarzer Bälle, die schon sortiert wurden
*/
int nBlack = 0;
/**
   @brief Die Anzahl orangener Bälle, die schon sortiert/entfernt wurden
*/
int nOrange = 0;
/**
   @brief Sagt aus, ob das Display flackern und die Led blinken soll
   @details Wird auf true gesetzt, wenn ein Orangener Ball entdeckt wird
*/
bool doFlicker = false;

/**
   @brief Kleine Klasse die Knopfdrücke verarbeitet

*/
class ButtonHandler { //handels button clicks
    /**
       @brief Der Pin an dem der Knopf angeschlossen ist

    */
    int pin;
    /**
       @brief Gibt an, ob der Knopf momentan Gedrückt ist

    */
    bool isPressed = false;
  public:
    /**
       @brief Die Funktion die bei einem Klick, d.h. einem Drücken und loslassen des Knopfes ausgeführt wird

    */
    void (*onclick)();
    ButtonHandler() {}
    /**
       @brief Erstellt ein neues ButtonHandler Objekt

       @param pin Der Pin an dem der Knopf angeschlossen ist
       @param onclick Die Funktion die bei einem Klick, d.h. einem Drücken und loslassen des Knopfes ausgeführt wird
    */
    ButtonHandler(int pin, void (*onclick)()): pin(pin), onclick(onclick) {
      pinMode(pin, INPUT_PULLUP);
    }
    /**
       @brief Prüft, ob der Knopf gedrückt/losgelassen wurde
       @details Wird von loop() aufgerufen und ruft die ButtonHandler::onclick Funktion auf, wenn ein Klick festgestellt wurde
    */
    void update()
    {
      bool isPressedNew = digitalRead(pin) == HIGH;
      if (isPressedNew != isPressed) { //is not being pressed now, but was being pressed
        if (isPressed) {
          Serial.println("click");
          onclick();
        }
      }
      isPressed = isPressedNew;
    }
};
/**
   @brief Farben, werden für die Messungen des Reflexoptokopplers benutzt

*/
enum Farbe
{
  WHITE,
  BLACK,
  ORANGE,
  NOTHING
};
Farbe mesureColor(); //sonst erkennt Arduino Farbe nicht als typ an (https://forum.arduino.cc/t/syntax-for-a-function-returning-an-enumerated-type/107241)
/**
   @brief Misst mithilfe des Reflexoptokopplers die Farbe des Balls.

   @return Farbe Die Farbe des Balls
*/
Farbe mesureColor()
{
  //return ORANGE;//inputs hardcoden, für Testzwecke
  //int hue = random(0, 1000); //inputs simulieren
  int hue=analogRead(A0);//tatsächlich Farbe messen
  if (hue <= 100) {
    return ORANGE;
  }
  if (hue < 500) {
    return WHITE;
  }
  if (hue < 830) {
    return NOTHING;
  }
  return BLACK;
}
/**
   @brief Setzt die Farbe der RGB-Led

   @param r Rot (0-255)
   @param g Grün (0-255)
   @param b Blau (0-255)
*/
void setLedColor(unsigned char r, unsigned char g, unsigned char b) //unsigned char: 0-255
{
  analogWrite(PIN_RED, r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE, b);
}
/**
   @brief wird ausgeführt wenn der Stop-Knopf gedrückt wird

*/
void stopButtonClicked()
{
  static bool isStopped = false;//wird nur einmal initialisiert
  isStopped = !isStopped;
  if (isStopped) {
    Serial.println("stopping servo");
    servo.stop();
    callHandler.deleteCalls();
    /*static*/ auto call = new Callable*[1] {
      new LcdDotAnim("gestoppt, warte auf start", &lcd, 100000000000000) //ja, sollte ich vermutlich besser implementieren
    };
    callHandler.setCalls(call, 1);
    setLedColor(255, 0, 0);
    doFlicker = false;
  } else {
    callHandler.running = false;
  }
}
/**
   @brief der Stop Knopf

*/
ButtonHandler stopButton;
/**
   @brief Hilfs Funktion, Methoden können nicht als Funktionsparameter benutzt werden
   @return true
   @return false
*/
bool servoIsDone()
{
  return servo.isDone();
}
/**
   @brief 2 Befehle, gibt "gehe zurück" auf dem Bildschirm aus und geht zurück

*/
#define GEH_ZURUECK \
  new LcdDotAnim("Gehe zur\365ck",&lcd),\
  new FuncCall([](){\
    servo.setSpeed(SERVO_SPEED_FAST);\
    servo.write(ANGLE_CENTER);\
  },&servoIsDone)
/**
   @brief Bewegt einen Ball zum Loch, legt ihn Ab und geht zurück

   @tparam angle Der Winkel als Template, da Lambdas (Funktionen die als Parameter weitegegeben werden) keine Variablen von Außen beinhalten dürfen
   @param name Die Farbe des Balls
*/
template <short angle>
void legBallAb(String name, String richtung)
{
  callHandler.deleteCalls();
  /*static*/ auto calls = new Callable*[6] {
    //static so that the space for the calls is only allocated once (https://cpp4arduino.com/2018/11/06/what-is-heap-fragmentation.html),
    //didn't end up being necessary because at there is only one object in heap at one point in time (callHandler.deleteCalls())
    // new so that it is allocated on the heap
    //auto automatically sets the type, in this case Callable*[] (Callable**)
    new LcdString("Ball erkannt, vorsicht", &lcd, 1000), //objects get upcasted to Callable*
    new LcdDotAnim(name + "er Ball, drehe " + richtung, &lcd, 0),
    new FuncCall([]() {
      servo.write(angle);
    }, &servoIsDone),
    new LcdString("Angekommen", &lcd, 1000),
    GEH_ZURUECK //2 elemente
  };
  callHandler.setCalls(calls, 6); //if the number is too large the program crashes
}
/**
   @brief Wird am Anfang des Programms aufgerufen

*/
void setup()
{
  Serial.begin(9600);
  Serial.println("setup");
  servo.attach(PIN_SERVO);
  lcd.init();
  servo.writeDirect(ANGLE_CENTER);
  callHandler.setCalls(new Callable*[1] {
    new LcdLoadingAnim("Lade", &lcd, LOADING_DURATION),
  }, 1);
  randomSeed(analogRead(A1));
  stopButton = ButtonHandler(PIN_STOPBUTTON, &stopButtonClicked);
}


/**
   @brief Wird immer wieder ausgeführt
   @details Hier werden alle möglichen Objekte wie der Servo, der Lcd usw. aktualisiert und die Aktionen (Farbe des Balls messen, Bewegung des Servos Starten etc. ) koordiniert
   @image html "loop Programmablaufplan.svg" "vereinfachter Programablaufplan für die loop() Funktion" size_inidication=size
   @image latex "loop Programmablaufplan.png" "vereinfachter Programablaufplan für die loop() Funktion" size_inidication=size
*/
void loop()
{
  callHandler.update();
  lcd.update();
  servo.updatePos();
  stopButton.update();
  if (doFlicker) {
    unsigned short b = (millis() / 2) % 513; //helligkeit: 0-512
    if (b > 255) {
      b = 511 - b; //wenn b größer als 255, wird die helligkeit kleiner, Werte über 255 werden also "gespiegelt"
    }
    setLedColor(b, b / 2, 0); //orange
    if (random(3) == 0) {
      lcd.noBacklight();
    } else {
      lcd.backlight();
    }
  }
  if (callHandler.running) {
    return;
  }
  Farbe farbe = mesureColor();
  servo.setSpeed(SERVO_SPEED_DEFAULT);
  switch (farbe) {
    case WHITE:
      {
        nWhite++;
        Serial.println("white");
        legBallAb<ANGLE_LEFT_HOLE>("Wei\342", "Links");
        setLedColor(255, 255, 255);
        break;
      }
    case BLACK:
      {
        nBlack++;
        Serial.println("black");
        legBallAb<ANGLE_RIGHT_HOLE>("Schwarz", "Rechts");
        setLedColor(0, 0, 255);
        break;
      }
    case NOTHING:
      {
        Serial.println("nothing");
        callHandler.deleteCalls();
        /*static*/ auto callsNothing = new Callable*[1] {
          new LcdString(String("Ball einlegen W:") + nWhite + String(" S:") + nBlack + String(" O:") + nOrange, &lcd, 1000)
        };
        callHandler.setCalls(callsNothing, 1);
        setLedColor(0, 255, 0);
        break;
      }
    case ORANGE:
      {
        doFlicker = true;
        nOrange++;
        Serial.println("orange");
        callHandler.deleteCalls();
        /*static*/ auto callsOrange = new Callable*[10] {
          new LcdString("Oran\2er B\2ll", &lcd, 1000),
          new LcdString("\2\2\2\2O\2\2OR\2\2R\2\2A\2\2N\2\2G\2\2\2E\2\2\2\2!\2\2", &lcd, 0),
          new FuncCall([]() {
            servo.write(ANGLE_RIGHT_HOLE);
          }, &servoIsDone),
          new LcdString("Fehler \2rk\2nnt", &lcd, 1000),
          new FuncCall([]() {
            servo.setSpeed(SERVO_SPEED_FAST);
            servo.write(ANGLE_MIN);
          }, &servoIsDone),
          new FuncCall([]() {
            servo.setSpeed(SERVO_SPEED_DEFAULT);
          }, &servoIsDone),
          new FuncCall([]() {
            doFlicker = false;
            setLedColor(0, 255, 0);
            lcd.backlight();
          }),
          new LcdString("Fehler beseitigt", &lcd, 2000),
          GEH_ZURUECK
        };
        callHandler.setCalls(callsOrange, 10);
        break;
      }
  }
}