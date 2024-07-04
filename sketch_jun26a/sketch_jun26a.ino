#include <BleGamepad.h>

// Define los pines de los botones
const int button1 = 12; // Botón del mástil 1
const int button2 = 13; // Botón del mástil 2
const int button3 = 14; // Botón del mástil 3
const int button4 = 27; // Botón del mástil 4
const int button5 = 26; // Botón del mástil 5
const int startButton = 25; // Botón de Start
const int powerButton = 33; // Botón de Power
const int strumUpButton = 32; // Botón de Strum Up
const int strumDownButton = 35; // Botón de Strum Down

// Instancia del gamepad BLE
BleGamepad bleGamepad("Guitarra Botk", "botk", 100);

void setup() {
  // Inicia la emulación del gamepad BLE
  bleGamepad.begin();

  // Configura los pines de los botones como entradas
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);
  pinMode(powerButton, INPUT_PULLUP);
  pinMode(strumUpButton, INPUT_PULLUP);
  pinMode(strumDownButton, INPUT_PULLUP);
}

void loop() {
  if (bleGamepad.isConnected()) {
    // Lee el estado de los botones y envía las señales correspondientes del gamepad
    if (digitalRead(button1) == LOW) {
      bleGamepad.press(BUTTON_1);
    } else {
      bleGamepad.release(BUTTON_1);
    }

    if (digitalRead(button2) == LOW) {
      bleGamepad.press(BUTTON_2);
    } else {
      bleGamepad.release(BUTTON_2);
    }

    if (digitalRead(button3) == LOW) {
      bleGamepad.press(BUTTON_3);
    } else {
      bleGamepad.release(BUTTON_3);
    }

    if (digitalRead(button4) == LOW) {
      bleGamepad.press(BUTTON_4);
    } else {
      bleGamepad.release(BUTTON_4);
    }

    if (digitalRead(button5) == LOW) {
      bleGamepad.press(BUTTON_5);
    } else {
      bleGamepad.release(BUTTON_5);
    }

    if (digitalRead(startButton) == LOW) {
      bleGamepad.press(BUTTON_6); // Mapea este botón a BUTTON_6
    } else {
      bleGamepad.release(BUTTON_6);
    }

    if (digitalRead(powerButton) == LOW) {
      bleGamepad.press(BUTTON_7); // Mapea este botón a BUTTON_7
    } else {
      bleGamepad.release(BUTTON_7);
    }

    if (digitalRead(strumUpButton) == LOW) {
      bleGamepad.press(BUTTON_8); // Mapea este botón a BUTTON_8
    } else {
      bleGamepad.release(BUTTON_8);
    }

    if (digitalRead(strumDownButton) == LOW) {
      bleGamepad.press(BUTTON_9); // Mapea este botón a BUTTON_9
    } else {
      bleGamepad.release(BUTTON_9);
    }
  }

  // Modo de emparejamiento si el botón de Start se mantiene presionado
  if (digitalRead(startButton) == LOW) {
    unsigned long startPressTime = millis();
    while (digitalRead(startButton) == LOW) {
      delay(50);
      if (millis() - startPressTime > 3000) { // Mantener presionado durante 3 segundos
        bleGamepad.end();
        delay(1000); // Espera un segundo antes de reiniciar el modo de emparejamiento
        bleGamepad.begin();
        break;
      }
    }
  }
}
