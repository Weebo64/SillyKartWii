# Frenzy Mode UI Test Guide

## Was wurde implementiert

Der Frenzy-Modus zeigt jetzt einen **"FRENZY!"** Text auf dem Bildschirm an, wenn ein Spieler im Frenzy-Modus ist (Star-State).

## Wie es funktioniert

- **Text:** "FRENZY!" (BMG ID: 0xD000)
- **Position:** Oben-mittig auf dem Bildschirm (nutzt das ghostMessage Control)
- **Anzeige:** Nur während des aktiven Frenzy (450 Frames / ~7.5 Sekunden)
- **Verschwindet:** Automatisch wenn der Frenzy-Modus endet

## Test-Schritte

1. **Kompiliere das Projekt**
   ```powershell
   # Im Projektverzeichnis
   .\build.bat
   # oder dein Build-Befehl
   ```

2. **Starte das Spiel**
   - Wähle einen beliebigen Spielmodus (GP, VS, Time Trial, etc.)
   - Starte ein Rennen

3. **Aktiviere einen Frenzy**
   - Fahre durch eine Item-Box
   - Mit 100% Wahrscheinlichkeit für Spieler (zum Testen) sollte fast jede Item-Box einen Frenzy auslösen
   - **WICHTIG:** Der Frenzy wird nur für Items aktiviert, die NICHT auf der Blacklist stehen

4. **Was du sehen solltest:**
   - ✅ Wenn Frenzy startet:
     - Star-Musik spielt
     - Star-Effekt (Unverwundbarkeit)
     - **"FRENZY!" Text erscheint oben auf dem Bildschirm**
     - Items werden automatisch nachgefüllt
     - Tricks funktionieren überall
   
   - ✅ Wenn Frenzy endet:
     - **"FRENZY!" Text verschwindet**
     - Star-Effekt endet
     - Normale Item-Mechanik kehrt zurück

5. **Debug-Ausgabe (optional)**
   - Wenn du Dolphin mit Gecko-Codes oder einen Debug-Build nutzt:
   - Schau in die Konsole für:
     ```
     FRENZY STARTED!
     FRENZY ENDED!
     ```

## Bekannte Einschränkungen (Test-Version)

- ❌ Text-Position ist fest (ghostMessage-Position)
- ❌ Keine Animations/Effekte
- ❌ Kein Timer-Countdown sichtbar
- ❌ Split-Screen: Nur Spieler 1 (HUD Slot 0) bekommt die Anzeige
- ❌ Text könnte mit anderen Ghost-Nachrichten kollidieren

## Wenn der Text NICHT erscheint

### Mögliche Ursachen:

1. **Kein RaceHUD geladen**
   - Lösung: Nur im Rennen testen, nicht im Menü

2. **ghostMessage existiert nicht**
   - Einige Modi haben kein ghostMessage Control
   - Versuche einen anderen Spielmodus (GP oder VS empfohlen)

3. **Item ist auf der Blacklist**
   - Blacklist: Bullet Bill, Golden Mushroom, Thunder Cloud, POW, Lightning, Blooper
   - Wenn ein blacklisted Item erscheint, wird der Frenzy auf die NÄCHSTE Item-Box verschoben

4. **Frenzy-Limit erreicht**
   - Max 10 Frenzies pro Spieler pro Rennen
   - Max 2 simultane Frenzies im ganzen Rennen
   - Zum Testen: Starte ein neues Rennen

## Debug-Checks

Falls der Text nicht angezeigt wird, prüfe in `FrenzyMode.cpp`:

```cpp
// Zeile ~150-180 in ShowFrenzyText()
// Füge Debug-Prints hinzu:
OSReport("ShowFrenzyText called: slot=%d, show=%d\n", hudSlot, show);
OSReport("RaceHUD found: %p\n", raceHUD);
OSReport("ghostMessage: %p\n", raceHUD ? raceHUD->ghostMessage : nullptr);
```

## Nächste Schritte (nach erfolgreichem Test)

Wenn der Text erfolgreich angezeigt wird, können wir:

1. ✨ Custom Layout erstellen (eigene Position, Größe, Farbe)
2. ⏱️ Timer-Countdown hinzufügen (z.B. "FRENZY! 5.2s")
3. 🎨 Animationen/Effekte (Pulsieren, Glow, etc.)
4. 👥 Split-Screen Support (alle 4 HUD Slots)
5. 🎯 Bessere Integration (eigenes UI-Element statt ghostMessage)

## Test-Konfiguration zurücksetzen

Nach dem Testen kannst du die Wahrscheinlichkeiten wieder anpassen:

```cpp
// In FrenzyMode.hpp, Zeile ~17-18
static const u8 FRENZY_PROBABILITY_PLAYER = 20;  // 20% statt 100%
static const u8 FRENZY_PROBABILITY_CPU = 10;     // 10% für CPUs
```

## Fragen/Probleme?

- Text erscheint nicht? → Prüfe Debug-Ausgabe
- Text an falscher Position? → Erwartet, wir verwenden ghostMessage
- Text verschwindet zu früh? → FRENZY_DURATION in FrenzyMode.hpp anpassen
- Crash? → Prüfe, ob alle Includes korrekt sind

Viel Erfolg beim Testen! 🎮
