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
Calls (CallHandler::callPtrs) werden im [Heap](https://www.geeksforgeeks.org/stack-vs-heap-memory-allocation/) gespeichert um sie weiter benutzen zu können nachdem die Funktion in der sie instanziert wurden abgeschlossen ist
## Warum verschiedene Callable Klassen? 

Es wäre möglich gewesen statt mehrerer Callable Klassen einfach eine zu benutzen und dann die Art des Calls in einer Variable zu speichern. Der Nachteil dieser Methode wäre, dass bei jeder Funktion die etwas mit der Klasse zu tun hat (Callable::run, Callable::isDone, etc.) überprüft werden müsste, was die Art des Calls ist. Das würde zu einer schlechteren Lesbarkeit des Codes führen. 

 

# Optimierungsideen – Code 

Beim Ausführen des Programms mangelte es manchmal an Speicherplatz. Um dieses Problem zu umgehen hätte man statt der [Arduino String Klasse](https://www.arduino.cc/reference/de/language/variables/data-types/stringobject/) auch C-Strings also char [] benutzen können. Das hätte den Code leider aber auch an ein Paar stellen komplizierter gemacht. Ein anderer Fehler der sehr häufig auftrat war, dass mit Speicheradressen falsch umgegangen wurde und dadurch das Programm ohne jegliche Fehlermeldung abstürzte, oft sogar an völlig anderen Stellen. Eine mögliche Lösung wäre es hier eine Programmiersprache zu benutzen, die Speicherplatzfehler beim kompilieren, das heißt beim umwandeln des Programmcodes in Maschinencode aufdeckt. Eine Möglichkeit dafür wäre diese [Arduino Bibliothek](https://github.com/Rahix/avr-hal) für Rust ([Beispiel](https://creativcoder.dev/rust-on-arduino-uno)). Solche Bibliotheken sind aber auch weniger Dokumentiert und haben weniger Features als die Standard C++ Arduino Bibliothek und deshalb alles in allem für ein kleines Schulprojekt keine gute Option. 