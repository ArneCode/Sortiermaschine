Hallo, dies ist die \htmlonly Online -\endhtmlonly Dokumentation für den Code der Sortiermaschine von Johannes und Arne   
\latexonly \href {https://arnecode.github.io/Sortiermaschine}{Online Dokumentation} \endlatexonly
\htmlonly <a href="https://arnecode.github.io/Sortiermaschine/refman.pdf">Dokumentation als PDF \endhtmlonly
# Zur Dokumentation
\htmlonly Diese Seite hat eine Suchfunktion. Dort kann nach Klassen/Variabeln/Funktionen etc. gesucht werden  \endhtmlonly
Ein guter Ort um zu starten ist die sketch.ino Datei. Dort wird wird die Logik des gesamten Programms zusammengeführt. Für eine Liste aller Dateien bitte im Menü unter Dateien nachschauen.
Für die loop() Funktion ist auch ein Programmablaufplan vorhanden
## Klassen
Für Liste aller Klassen bitte im Menü unter Klassen nachschauen. 
Ein paar wichtige Klassen in diesem Projekt sind:
- AnimatableLcd - ermöglicht es Animationen auf dem Lcd-Display anzuzeigen
- CallHandler - lässt Calls nacheinander laufen
- CustomServo - Servo, bei dem die Geschwindigkeit gesteuert werden kann  
## Code
Der Code ist interaktiv, man kann Variabeln, Funktionen Methoden und Klassen anklicken um zu Ihrer Beschreibung zu gelangen. \htmlonly Oder man kann mit der Maus über dem Begriff "schweben" und ein Tooltip wird angezeigt \endhtmlonly

# Begründungen - Code 
@note Ich empfehle sich vor diesem Abschnitt ein wenig die Dokumentation zu "erforschen"

## Warum werden Calls im Heap gespeichert
Calls (CallHandler::callPtrs) werden mithilfe des ```new``` Keywords im <a href="https://www.geeksforgeeks.org/stack-vs-heap-memory-allocation/" target="_blank">Heap</a> gespeichert um sie weiter benutzen zu können nachdem die Funktion in der sie instanziert wurden abgeschlossen ist. Sie müssen deshalb aber auch manuell mit CallHandler::deleteCalls gelöscht werden.
## Warum verschiedene Callable Klassen? 

Es wäre möglich gewesen statt mehrerer Callable Klassen einfach eine zu benutzen und dann die Art des Calls in einer Variable zu speichern. Der Nachteil dieser Methode wäre, dass bei jeder Funktion die etwas mit der Klasse zu tun hat (Callable::run, Callable::isDone, etc.) überprüft werden müsste, was die Art des Calls ist. Das würde zu einer schlechteren Lesbarkeit des Codes führen. Das war in einer älteren Version der Fall.
<details>
<summary>Code</summary>
\code{.cpp}
class LcdHandler {
  public:
    enum AnimType {
      DOT,
      LOADING,
      NO_ANIMATION
    };
    struct LcdString {
      long duration;
      AnimType animType;
      String text;
      LcdString(String text, long duration, AnimType animType = NO_ANIMATION): text(text), duration(duration), animType(animType) {}
      operator String() const {
        return text;
      }
    };
    bool running = false;
  private:
    LcdString * currString;
    long t;
    LcdString * lastString;
    LcdString * lcdStrings;
    long stepDuration;
    long lastRefresh;
    void( * callback)(); //function pointer
  public:
    ~LcdHandler() {
      delete[] lcdStrings;
    }
    void init() {
      lcd.init();
      lcd.backlight();
      lcd.createChar(0, loading_empty_c);
      lcd.createChar(1, loading_full_c);
    }
    void setStrings(LcdString newLcdStrings[], size_t numStrings, void( * newCallback)() = NULL, int newStepDuration = 1000) {
      delete[] lcdStrings; //Speicherplatz frei machen
      lcdStrings = newLcdStrings;
      currString = newLcdStrings;
      lastString = newLcdStrings + numStrings - 1;
      t = millis();
      stepDuration = newStepDuration;
      lastRefresh = millis();
      running = true;
      callback = newCallback;
      prepareAnimation(currString);
    }
    void prepareAnimation(LcdString* currString) {
      switch (currString->animType) {
        case LOADING:
          stepDuration = currString->duration / 9;
          if (currString->text.length() > 16) {
            Serial.print("text given for loading animation is to long, text: ");
            Serial.println(*currString);
          }
          printCentered(*currString);
          lcd.setCursor(LOADING_BAR_OFFSET, 1);
          for (int i = 0; i < 8; i++) {
            lcd.write(0);
          }
          lcd.print("0% ");
          break;
        case DOT:
          printPretty(currString->text + String("   "));
          break;
        default:
          printPretty(*currString);
      }
    }
    void printCentered(String text, int length = -1, int row = 0) { //length<=16
      if (length == -1) {
        length = text.length();
      }
      int offset = (16 - length) / 2; //rundet immer ab, da int
      lcd.setCursor(offset, row);
      lcd.print(text);
    }
    void printPretty(String text) { //handelt zeilenumbrüche und schreibt zentriert
      lcd.clear();
      int length = text.length();
      if (length <= 16) {
        printCentered(text, length);
        return 0;
      }
      int spacePos = -1;
      for (int i = 15; i >= 0; i--) {
        if (text[i] == ' ') {
          spacePos = i;
          break;
        }
      }
      String row1, row2;
      if (spacePos != -1) {
        row1 = text.substring(0, spacePos);
        row2 = text.substring(spacePos + 1);
      } else {
        row1 = text.substring(0, 16);
        row2 = text.substring(16);
      }
      printCentered(row1, row1.length(), 0);
      printCentered(row2, row2.length(), 1);
    }
    void printAnimated() {
      long time = millis();
      AnimType type = currString->animType;
      if (type == DOT) {
        if ((time - lastRefresh) < stepDuration) {
          return;
        }
        lastRefresh = time;
        int numDots = ((time - t) / stepDuration) % 4;
        char dots[4];
        for (int i = 0; i < 3; i++) {
          if (i < numDots) {
            dots[i] = '.';
          } else {
            dots[i] = ' ';
          }
        }
        dots[3] = '\0';
        printPretty(currString->text + dots);
      } else if (type == LOADING) {
        short percent = (time - t) * 100 / currString->duration;
        if (time - lastRefresh > stepDuration) {
          short nToFill = percent * 8 / 100;
          if (nToFill == 0) {
            return;
          }
          lcd.setCursor(nToFill + LOADING_BAR_OFFSET - 1, 1);
          lcd.write(1);
          lastRefresh = time;
        }
        lcd.setCursor(8 + LOADING_BAR_OFFSET, 1);
        lcd.print(percent);
        lcd.print("%");
      } else {
        Serial.println("unknown animation type");
      }
    }
    void animate() {
      if (!running) {
        return;
      }
      int timePassed = millis() - t;
      if (timePassed > currString -> duration) {
        if (currString + 1 > lastString) {
          running = false;
          if (callback != NULL) {
            callback();
          }
          return;
        }
        currString++;
        t = millis();
        prepareAnimation(currString);
        return;
      }
      if (currString->animType == NO_ANIMATION) {
        return;
      }
      printAnimated();
    }
};
\endcode
</details>
 

# Optimierungsideen – Code 

Beim Ausführen des Programms mangelte es manchmal an Speicherplatz. Um dieses Problem zu umgehen hätte man statt der [Arduino String Klasse](https://www.arduino.cc/reference/de/language/variables/data-types/stringobject/) auch C-Strings also char [] benutzen können. Das hätte den Code leider aber auch an ein Paar stellen komplizierter gemacht. Ein anderer Fehler der sehr häufig auftrat war, dass mit Speicheradressen falsch umgegangen wurde und dadurch das Programm ohne jegliche Fehlermeldung abstürzte, oft sogar an völlig anderen Stellen. Eine mögliche Lösung wäre es hier eine Programmiersprache zu benutzen, die Speicherplatzfehler beim kompilieren, das heißt beim umwandeln des Programmcodes in Maschinencode aufdeckt. Eine Möglichkeit dafür wäre diese [Arduino Bibliothek](https://github.com/Rahix/avr-hal) für Rust ([Beispiel](https://creativcoder.dev/rust-on-arduino-uno)). Solche Bibliotheken sind aber auch weniger Dokumentiert und haben weniger Features als die Standard C++ Arduino Bibliothek und deshalb alles in allem für ein kleines Schulprojekt keine gute Option. 