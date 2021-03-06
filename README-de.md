# MultiGeiger

Hinweis: dies ist die seitherige Doku im Markdown-Format.

Sie soll mit der Zeit auf https://multigeiger.readthedocs.io/ in Englisch und
Deutsch verfügbar gemacht werden.

# Ecocurious

Der MultiGeiger ist ein Projekt von Ecocurious, Deiner Umwelt-, Natur-  und Technik-Community, siehe https://ecocurious.de/projekte/multigeiger/.

Ziel ist es, ein Bürger-Messnetz für Radioaktivität in Deutschland aufzubauen.

Die MultiGeiger Hard- und Software hat Jürgen Böhringer konzipiert (http://www.boehri.de).

Reinhard/rexfue hat die Software und Platine weitergedacht und kümmert sich um die Einbindung der Sensoren in unsere Map https://ecocurious.de/multigeiger-karte/.

Wir haben die ersten Workshops gestartet, in denen Du die Bauteile und das Gehäuse mit unserer Unterstützung zusammenbauen kannst.

Workshop-Termine findest Du hier:
 * https://www.meetup.com/de-DE/Ecocurious-deine-Umwelt-Natur-und-Technik-Community/
 * https://ecocurious.de/events/

Klingt das interessant für Dich? Dann mach mit, herzliche Einladung!


## Installation
Hier in GitHub die neueste Release ( https://github.com/ecocurious/MultiGeiger/releases ) als Source code (zip) oder Source Code (tar.gz) herunterladen und entpacken.
Mit der Arduino-IDE in dem neuen Verzeichnis die Datei *multigeiger.ino* im Verzeichnis *multigeiger* öffnen.

Die Platine unterstützt zwei verschiedene Heltec-Bausteine, verschiedene Zählrohre und optional einen Temperatur/Luftdruck/Luftfeuchtigkeits-Sensor.
Die Software kann via Netzwerk Daten zu verschiedenen Services senden.

 * **Heltec WiFi Kit 32**
Diese MCU hat ein großes Display und WiFi.
Auf dem Board wird dieser Baustein in die längeren Buchsenleisten gesteckt.

 * **Heltec Wireless Stick**
Diese MCU hat ein sehr kleines Display, dafür aber zusätzlich zu WiFi noch LoRa.
Es wird in die kürzeren Buchsenleisten gesteckt.

Um die Heltec-Boards in der Arduino IDE auswählen zu können, muss https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json in den Preferences unter Additional Boards Manager URLs hinzugefügt werden. Danach können unter Tools->Board->Boards Manager die ESP32 Boards (Name "esp32 by Espressif Systems")
installiert und anschließend unter Tools->Board das Heltec Board ausgewählt werden. Für **beide** Boards muss hier der **Heltec wireless Stick** ausgewählt werden. Die Software erkennt selbstständig, welches Board bestückt ist. Unter Tools muss noch **Flash size: "4MB(32Mb)"** und  **Partition Scheme: "Minimal SPIFFS (Large APPS with OTA)"** eingestellt werden.


Diverse Software-Einstellungen kann man über folgende Dateien machen (siehe Kommentare dort):

 * **./multigeiger/userdefines.h** (immer notwendig, ein Beispiel hierzu wird in userdefines-example.h mitgeliefert)
 * **./platformio.ini** (nur bei platformio, ein Beispiel hierzu wird in platformio-example.ini mitgeliefert)

Es werden alle externen Libraries benötigt, die in der Datei 
```
platformio-example.ini
```
unter dem Abschnitt 
```
lib-deps =
```
aufgelistet sind. Bitte jeweils die aktuellste Version über platform.io / Libraries installieren.

 **Achtung:** Wenn die Arduino-IDE verwendet wird, dann bitte prüfen, dass in der Datei project_config/lmic_project_config.h (in der obersten Ebene in dieser Library) unbedingt
```
die richtigen Configs eingestellt sind. Die Datei muss folgendermassen aussehen:
// project-specific definitions
#define CFG_eu868 1
//#define CFG_us915 1
//#define CFG_au921 1
//#define CFG_as923 1
// #define LMIC_COUNTRY_CODE LMIC_COUNTRY_CODE_JP	/* for as923-JP */
//#define CFG_in866 1
#define CFG_sx1276_radio 1
//#define LMIC_USE_INTERRUPTS
```
Die angegebenen Versionen der Libraries sind die Mindestanforderungen. Wir testen immer auch mit den neuesten Versionen, deshalb bitte immer die aktuellen Versionen installieren und verwenden.\
Falls der Compiler andere Libraries anmahnt, diese bitte in der Arduino IDE per *Sketch -> Include Library -> Manage Libraries ..* installieren.

## Ablauf nach dem Start
Das Gerät baut einen eigenen WLAN-Accesspoint (AP) auf. Die SSID des AP lautet **ESP32-xxxxxxxx**, wobei die xxx
die Chip-ID (bzw. die MAC-Adresse) des WLAN-Chips sind (Beispiel: **ESP32-51564452**).
**Bitte diese Nummer notieren, sie wird später noch gebraucht.**
Dieser Access-Point bleibt für 30sec aktiv. Danach versucht das Gerät, sich mit dem (früher) eingestellten WLAN
zu verbinden. Dieser Verbindungsversuch dauert ebenfalls 30sec. Kommt keine Verbindung zu Stande, wird wieder der
eigene AP für 30sec erzeugt. Wenn das WLAN nicht erreicht werden kann, läuft dieses Spiel endlos.
Solange keine Verbindung zum WLAN besteht, wird auf dem Display in der untersten Zeile ganz klein *connecting ...*
angezeigt. Diese Anzeige verschwindet, sobald eine WLAN-Verbindung hergestellt ist.

## Einstellung des WLAN
Wenn das Gerät den eigenen AP aufgebaut hat, verbindet man sich mit diesem. Entweder mit einem Handy oder einem PC o.ä..
Die Verbindung fragt nach einem Passwort, es lautet **ESP32Geiger**.
Ist die Verbindung mit dem Accesspoint hergestellt, hat man beliebig Zeit, die Daten einzugeben.
Es öffnet sich **automatisch** die Startseite des Gerätes. Es braucht also - in der Regel - nicht extra der Browser aufgerufen werden.
Falls die Startseite ausnahmsweise doch nicht erscheint, so muss mit dem Browser die Adresse **192.168.4.1** aufgerufen werden und nun erscheint die Startseite.
Dort findet man einen Link zur __configure page__ - dort drauf klicken und man kommt zur Einstellungsseite.


Diese hat die folgenden Zeilen:
 * Geiger accesspoint SSID\
 Dies ist die SSID des eingebauten APs und kann zwar geändert werden, sollte aber nicht! Der Sensor wird mit dieser Nummer bei sensor.community (früher: luftdaten.info) angemeldet. Wird sie geändert, muss eine neue Anmeldung erfolgen.
 * Geiger accesspoint password\
 Dies ist das Passwort für den eingebauten AP.\
 Dieses **MUSS** beim ersten Mal geändert werden. Es kann natürlich auch das gleiche Passwort **ESP32Geiger** wieder verwendet werden - wichtig ist nur, dass da was reingeschrieben wird und dass man das **nicht vergessen** darf.
 * WiFi client SSID\
 Hier muss die SSID des WLANs für den Netzwerk/Internet-Zugang eingegeben werden.
 * WiFi client password\
 Und hier das zugehörige Passwort.

Es wird empfohlen, beim WLAN das Gastnetz zu verwenden (falls ein solches existiert). Normalerweise wird das Gastnetz im Router vom normalen Netz abgeschottet und ist damit sicherer.

Ist alles eingegeben, kann man auf **Apply** drücken. Nun werden die eingestellten Daten übernommen und in das interne EEPROM gespeichert. Nun bitte **unbedingt** über **Abbrechen** diese Seite verlassen! Nur dann verlässt das Programm den Config-Mode und verbindet sich mit dem heimischen WLAN. Wenn es kein **Abbrechen** gibt, dann wieder zurück in die WLAN-Einstellungen des Gerätes gehen und da dann das normale Heim-Netzwerk wieder einstellen.

**ACHTUNG ACHTUNG**\
**Beim Update auf die Version 1.13 müssen die WLAN-Daten noch einmal neu eingegeben werden**. Bei zukünftigen Versionen wird das nicht mehr nötig sein.

Weiter können über die Einstell-Seite einige verschiedene Definitionen festgelegt werden:

* Start-Melodie, Lautsprecher-Tick, LED-Tick und Anzeige jeweils an oder aus
* Senden zu sensor.community oder/und zu madavi.de
* bei LoRa-Hardware können hier auch die LoRa-Kenngrößen (DEVEUI, APPEUI und APPKEY) eingegeben werden.

Am Ende der Einstellungsseite gibt es einen Link __Firmware update__ - hiermit kann man die Software auf dem MultiGeiger aktualisieren.
Man braucht dazu die .bin-Datei, wählt diese dann über **Browse...** aus und klickt zum Aktualisieren auf **Update**.
Danach dauert es ca. 30s für das Hochladen und Flashen der Datei.

Der Browser zeigt dann (hoffentlich) **Update Success! Rebooting...** an, der MultiGeiger startet dann neu und ab dann ist die neue
Firmware aktiv.

Erscheint **Update error: ...**, dann hat das Update nicht geklappt - es ist dann die seitherige Firmware weiter aktiv.

### Aufruf aus dem WLAN ###
Die Einstellseite kann zu jeder Zeit aus dem eigenen WLAN heraus aufgerufen werden. Dazu 
wird in der Adresszeile des Browsers http://esp32-xxxxxxx eingegeben. Hier ist xxxxxxx wieder die Chip-ID (siehe oben, Beispiel: **http://esp32-51564452**). Sollte es mit diesem Hostnamen nicht klappen, dann muss die IP-Adresse des Geigerzählers verwendet werden, diese kann aus dem Router ausgelesen werden.
Es erscheint zunächt eine Login-Seite.
Hier ist als Username **admin** und als Passwort das **Geiger accesspoint password** von oben einzugeben. Dann erscheint die gleiche Einstellseite wie oben beschrieben.


## Server
Es werden jeweils einen Messzyklus lang die Impulse gezählt und dann die "Counts per Minute" (cpm) berechnet.
Jeweils nach diesem Zyklus werden die Daten zu den Servern bei *sensor.community* und bei *madavi.de* gesendet.

Bei *sensor.community* werden die Daten gespeichert und stehen am nächsten Tag zum Abruf als CSV-Datei bereit:
http://archive.sensor.community/DATE/DATE_radiation_si22g_sensor_SID.csv
wobei DATE = Datum im Format YYYY-MM-DD ist (beides mal gleich) und SID die Sensornummer des Sensors (**nicht** die ChipID). Bei anderen Sensoren ist der Zählrohr-Name **si22g** durch den entsprechenden Namen zu ersetzen (z.B.: sbm-20 oder sbm-19) 

Bei *madavi* werden die Daten in einer RRD-Datenbank abgelegt und können direkt aktuell als Grafik über diesen Link betrachtet werden:
https://www.madavi.de/sensor/graph.php?sensor=esp32-CHIPID-si22g
Hier ist dann CHIPID die ChipId (also die Ziffern der SSID des internen Accesspoints).

Während der Übertragung der Daten zu den Servern wird in der Statuszeile (unterste Zeile) des Displays kurz der Name des Servers eingeblendet.

## Anmeldung bei sensor.community (luftdaten.info)
Damit die Daten, die der Sensor nach sensor.community schickt, von dem Server auch angenommen werden, muss man sich dort anmelden. Das geschieht über die Seite https://meine.luftdaten.info.
Zuerst über den *Registrieren*-Knopf einen Account anlegen. Dann damit über *Login* einloggen und *Neuen Sensor registrieren* anklicken.
Dann das Formular ausfüllen:
 * Erste Zeile, Sensor ID:\
 Hier die Nummer (nur die Zahlen) der SSID des Sensors eingeben (z.B. bei dem Sensor ESP-51564452 also dann nur 51564452 eingeben)
 * Zweite Zeile, Sensor Board:\
 Hier *esp32* auswählen (über die kleinen Pfeile rechts)
 * Basisinformation:\
 Hier die Adresse eingeben (mit dem Land!). Der interne Name des Sensors kann beliebig vergeben werden, muss aber eingegeben werden. Bitte den Haken bei **Indoor-Sensor** setzen, so lange der Sensor wirklich innen ist.
 * Zusätzliche Informationen:\
 Kann freigelassen werden, darf aber auch ausgefüllt werden.
 * Hardware-Konfiguration:\
 Hier als Sensor-Typ den Eintrag **Radiation Si22G** (oder ggf. entsprechend) auswählen. Für den zweiten Sensor kann DHT22 stehen bleiben, das ist für uns irrelevant.
 * Position\
 Hier bitte die Koordinaten eingeben, so genau wie möglich (oder über den rechten Knopf die Koordinaten rechnen lassen). Dies wird benötigt, um den Sensor später auf der Karte anzeigen zu können.

 Nun mit *Einstellungen speichern* das Ganze beenden. Dann auf der Übersichts-Seite bei diesem Sensor auf *Daten* klicken. Nun steht hinter *Sensor ID* die ID des Sensors. Diese bitte merken: sie wird für die Abfrage bei sensor.community bzw. bei der Anzeige auf https://multigeiger.citysensor.de benötigt.
 
## LoRa-Interface
Um den Multigeiger an TTN ("The Things Network") anzubinden, sind ein paar vorbereitende Schritte durchzuführen:

* Anlegen des TTN-Devices bei *The Things Network*
* Übernehmen der Parameter in den Multigeiger
* Anmelden bei *sensor.community* (ehemals luftdaten.info)
* HTTP-Integration


### Anlegen eines TTN-Devices
Das Gerät muss bei TTN (The Things Network) angemeldet werden. Dazu muss zuerst (falls noch nicht vorhanden) ein Account bei TTN angelegt werden.

#### Account anlegen
Über <https://account.thethingsnetwork.org/register> auf die Webseite zum Account anlegen gehen. Hier dann einen **USERNAME** vergeben, die **EMAIL ADRESSE** eintragen und auch ein **PASSWORD** vergeben. Dann rechts unten über **Create account** den Account anlegen. Danach kann man sich mit den neuen Daten an der Console anmelden (<https://account.thethingsnetwork.org/users/login>).

#### Applikation anlegen
Ist man eingelogged, wird über **APPLICATIONS** und **add applications** die neue Applikation angelegt. Folgende Felder müssen ausgefüllt werden:

* **Application ID:**\
Eine beliebige Bezeichnung für diese Applikation, die darf es aber in dem Netzwerk noch nicht geben (also z.B.: geiger_20200205)
* **Description:**\
Hier kann eine beliebige Beschreibung der Apllikation eingegeben werden.
* **Application EUI:**\
Bleibt frei, die Nummer wird vom TTN-System erzeugt.
* **Handler registration:**\
Der vorausgefüllte Wert (ttn-handler-eu) ist schon richtig und bleibt stehen.

Nun mit **Add application** rechts unten die Apllikation hinzufügen.

#### Device anlegen
Zuletzt muss noch das Device angelegt werden. Dazu in der Übersicht der Applikationen die gerade neu angelegte Applikation auswählen (klicken). Hier dann im mittleren Bereich bei **DEVICES** das Anlegen eines neuen Devices über **register device** starten. Folgende Felder müssen ausgefüllt werden:

* **Device ID:**\
Ein beliebiger Name für das Device. Er muss innerhalb der Applikation eindeutig sein (also z.B.: geiger_01) und nur aus Kleinbuchstaben bestehen.
* **Device EUI:**\
Einmal auf das Symbol ganz links an der Zeile klicken, dann erscheint der Text, dass diese Nummer vom System erzeugt wird. Wir müssen sonst nichts weiter eingeben.
* **App Key:**\
Keine Eingabe nötig
* **App EUI:**\
Bleibt so stehen

Nun rechts unten auf **Register**. Nun ist auch das Device angelegt.

### Übernahme der LoRa-Parameter 
Die LoRa-Parameter müssen noch in das Programm übernommen werden.\
Dazu muss die Konfigurations-Webseite des Geigerzählers aufgerufen werden. Weiter oben ist erklärt, wie die Konfigurations-Seite zu erreichen ist (*Einstellung des WLAN* und *Aufruf aus dem WLAN*))

Nun auf der Konfigurations-Seite durch die Konfiguration gehen bis die Einstellung der LoRa-Parameter angezeigt wird. Nun die 3 Werte (**APPEUI, DEVEUI, APPKEY**) aus der TTN-Console (siehe oben) entnehmen und hier dann eintragen. In der TTN-Console auf das Device gehen, das oben angelegt wurde. Hier sind die 3 Werte zu finden. Es müssen die HEX-Werte **ohne** Leerzeichen eingegeben werden, und zwar so, wie sie in der TTN-Console erscheinen.
Beispiel:\
in der TTN-Console steht

```
Device EUI  00 D0 C0 00 C3 19 7C E8
```
Dann muss folgendes eingeben werden:

```
00D0C000C3197CE8
```
Dies gilt genauso ebenso für die **APPEUI** und den **APPKEY** .

### Anmelden bei sensor.community (ehem. luftdaten.info)
Wenn der Multigeiger seine Daten über TTN auch an *sensor.community* weitergeben soll, muss er dort angemeldet werden.
Die Anmeldung ist ziemlich ähnlich wie oben schon beschrieben. Hier werden nur die Änderungen erläutert (gesamten Ablauf bitte oben nachlesen).

 * Sensor ID:\
 Hier den hinteren Teil der DEVEUI eingeben (wenn also - siehe oben - die DEVEUI *00 D0 C0 00 C3 19 7C E8* heißt, sind die hinteren 4 Byte ( also *C3197CE8*) in dezimal umgerechnet ( also hier 3273227496 ) einzugeben.
 * Sensor Board:\
 Hier **TTN** auswählen (über die kleinen Pfeile rechts).
 

### HTTP-Integration
Um die Daten von TTN an *sensor.community* zu bekommen, muss noch die HTTP-Integration bei TTN eingestellt werden.\
In der TTN-Console *Applications* anklicken und dann die Applikation des Geigerzählers (z.B. *geiger_20200205*) anklicken. Dann rechts oben in der Leiste mit *Overview*, *Devices*, *Payload Formats*, *Integrations*, *Data*, *Settings* das **Integrations** anklicken. Hier dann über **add integration** die **HTTP Integration** auswählen.

Nun die angezeigten Felder ausfüllen:\
 **Process ID:**\
 	Hier einen beliebigen Namen für diese Integration vergeben\
 **Access Key:**\
 	Hier einmal rein klicken und den *default key* auswählen\
 **URL:**\
 	Hier nun die URL zum ttn2luft-Programm eingeben: <https://ttn2luft.citysensor.de>\
 **Method:**\
 	da steht schon *POST*, das lassen\
 **Authorization:**\
 	bleibt leer\
 **Custom Header Name:**\
 	hier kommt der Text **X-SSID** rein\
 **Custom Header value:**\
 	und da dann die SSID des Sensors (also die Nummer, die man bei der Anmeldung bei sensor.community erhalten hat, *NICHT* die Chip-ID).

Dann rechts unten auf **Add integration** klicken. Das war's dann.\
Und so etwa sieht das dann ausgefüllt aus:\
!["HTTP-Integration filled"](images/http_integration_filled.png)

### TTN-Payload (Beispiel)
Um in der TTN-Console nicht nur die Datenbytes sondern lesbare Werte zu sehen, kann ein kleines Script als payload-Decoder eingefügt werden.\
Auf der *The Things Network* Webseite einloggen, über **Applications** die oben angelegte Application suchen und dann anklicken. Hier in der Menü-Zeile den Tab **Payload Formats** auswählen. Dann den folgenden Code in das Feld einfügen (den vorhandenen Code überschreiben):

```
function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};
  if(port == 1) {
  decoded.counts = ((bytes[0]*256 + bytes[1]) * 256 + bytes[2]) * 256  + bytes[3];
  decoded.sample_time = (bytes[4] * 256 + bytes[5]) * 256 + bytes[6];
  decoded.tube = bytes[9];
  var minor = (bytes[7]&0xF)+(bytes[8]>>4) ;
  decoded.sw_version="" + (bytes[7]>>4) + "." + minor + "." + (bytes[8]&0xF);
  }
  if (port === 2) {
    decoded.temp = ((bytes[0] * 256 + bytes[1]) / 10) + "°C";
    decoded.humi = bytes[2] / 2 + "%";
    decoded.press = ((bytes[3] * 256 + bytes[4]) / 10) + "hPa";
  }
  return decoded;
}
```
