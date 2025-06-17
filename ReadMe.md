**Spielanleitung zu Conway's Game of Life**
=============================================================
Kurz vorab: Für den Spielstart sind einige Anforderungen nötig:

1. `sudo apt install libsdl2-mixer-2.0-0 libsdl2-mixer-dev` (da unser Spiel Hintergrundmusik enthält)

2. Spielverständnis: [https://de.wikipedia.org/wiki/Conways_Spiel_des_Lebens](url)


Allgemeine Informationen
==============================================

Wir unterscheiden zwischen drei Phasen:
- 1. Menüphase
- 2. Bearbeitungsphase (plant phase)
- 3. aktive Spielphase 
__________________________________________________________
**1. Menüphase:**

In dieser Phase kann man die Größe des Spiels, sowie die Größe des Kästchens einstellen. Dabei gibt man einfach die gwünschte Zahl ein für jede Einstellung an und bestätigt mit der Enter-Taste.

> _**Achtung:**_ Es ist nicht nötig, die Spielgröße einzustellen, da es bereits eine Standardeinstellung hat. Dazu einfach drei Mal die Enter-Taste tippen. 


__________________________________________________________
**2. Bearbeitungsphase:**

In dieser Phase kann man seiner Kreativität freien Lauf lassen.
Diese Phase findet einmalig nach der Menüphase statt oder nachdem man die Stopp-Taste drückt. Man erkennt die Phase auch daran, dass die lebendigen Zellen grau statt schwarz sind.

> Mit Linksklick wird eine Zelle erzeugt/gelöscht.


**3. aktive Spielphase**

In dieser Phase findet die Entwicklung der Zellen statt. Diese startet nachdem die grüne Starttaste gedrückt wird und endet, wenn die rote Stoptaste gedrückt wurde.

> **_Achtung :_** Das Spiel endet nicht, nachdem alle Zellen gestorben sind.

__________________________________________________________
Einzelne Bedeutung der Knöpfe:
===============================================
- grün == Start 
-  rot == Stop
- blau == Reset
- pink == Manuell/Automatisch (_**Achtung :**_  Funktioniert erst, nachdem grüner Knopf gedrückt wurde)(weitere Informationen siehe unten)
- grau1 == Pfeil unten: save (Spielstand wird gespeichert)
- grau2 == Pfeil oben : load (Spielstand wird geladen)

___________________________________________________________
Informationen zum **grünen** Knopf:  **Start**

- startet den Spielverlauf
- falls roter Knopf (stop) gedrückt wurde, kann man mit grün wieder starten
__________________________________________________________
Informationen zum **roten** Knopf:  **Stop**

- hält den Spielverlauf an 
- in der angehaltenen Phase kann man wieder Zellen setzen oder löschen
- 
__________________________________________________________
Informationen zum **blauen** Knopf:  **Reset**

- setzt alle Zellen auf weiß bzw. tötet alle Zellen
- kann jederzeit genutzt werden
__________________________________________________________
Informationen zum **pinken** Knopf: **Manuell/Automatisch**

- wechselt zwischen automatischen und manuellen Modus
    1. Automatisch: Spiel läuft von alleine
    2. Manuell: eine Entwicklung erfolgt pro Linksklick 
- funktioniert erst, nachdem der grüne Knopf gedrückt wurde
- das Spiel beginnt automatisch im manuellen Modus
__________________________________________________________
Informationen zum ersten **grauen** Knopf: **save**

- speichert den Spielstand (Es wird empfohlen zu speichern, nachdem der Stopknopf gedrückt wurde)
- kann nur einen Spielstand speichern  
__________________________________________________________
Informationen zum zweiten **grauen** Knopf:  **load**

- lädt einen gespeicherten Spielstand (Es wird empfohlen zu laden, nachdem der Stopknopf gedrückt wurde)
- kann so oft aufgerufen werden, solange ein gespeicherter Spielstand existiert
__________________________________________________________
