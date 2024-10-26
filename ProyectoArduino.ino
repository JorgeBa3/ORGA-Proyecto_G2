#include <LiquidCrystal.h>
#include <SoftwareSerial.h> // Asegúrate de incluir esta librería

// Pines para controlar los flip-flops (minas)
int flipFlopPins[16] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

// Pines para control adicional
const int pin18 = 18;  // Conectado al CLK de los flip-flops 74LS174
const int pin19 = 19;  // Siempre positivo (HIGH)

// Pines para el Bluetooth
const int bluetoothRx = A15; // Pin RX del módulo Bluetooth
const int bluetoothTx = A14; // Pin TX del módulo Bluetooth
SoftwareSerial bluetooth(bluetoothRx, bluetoothTx); // Crear objeto para la comunicación Bluetooth

//leds de estado de juego
const int pinLEDJugar = A9; // Pin para estado Jugando
const int pinLEDGameOver = A10; // Pin para estado Game Over
const int pinLEDGanar = A11; // Pin para estado Ganaste

// Pines de entrada para leer el estado de las bombas (flip-flops)
int inputPins[16] = {53,51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 48};

// Array para almacenar el estado de cada bomba (flip-flop)
bool estadosBombas[16] = {false};  // Todos los flip-flops comienzan apagados

// Array para almacenar posiciones jugadas
bool posicionesJugadas[16] = {false};  // Inicialmente, ninguna posición ha sido jugada

// Pines para la LCD
int rs = A0;
int e = A1;
int d4 = A2;
int d5 = A3;
int d6 = A4;
int d7 = A5;

LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

// Variables para puntaje
int puntaje = 0; // Inicializar el puntaje
int totalBombas = 0; // Cambiar a variable para contar las bombas encendidas
const int totalPosiciones = 16; // Total de posiciones
int totalSeguras = totalPosiciones; // Inicialmente, todas las posiciones son seguras

enum EstadoJuego {JUGANDO,CONFIGURANDO,GAME_OVER,GANASTE};

EstadoJuego estadoActual = CONFIGURANDO; // Estado inicial

// Función para actualizar la pantalla LCD
void actualizarLCD() {
    lcd.clear();  // Limpiar la pantalla
    lcd.setCursor(0, 1);  // Establecer el cursor en la primera línea

    switch (estadoActual) {
        case JUGANDO:
            lcd.setCursor(0, 0);
            lcd.print("Estado: Jugando");
            lcd.setCursor(0, 1);
            lcd.print("Puntaje: " + String(puntaje) + "/" + String(totalSeguras)); // Mostrar puntaje
            break;
        case CONFIGURANDO:
            lcd.print("Estado: Configurando");
            break;
        case GAME_OVER:
            lcd.print("Estado: Game Over");
            break;
        case GANASTE:
            lcd.print("Estado: Ganaste");
            break;
    }

    lcd.setCursor(0, 0); // Cambiar a la segunda línea
    lcd.print("Proyecto Final");
}

void setup() {
  Serial.begin(9600);  // Inicializar la comunicación serial
  bluetooth.begin(9600); // Inicializar la comunicación con el módulo Bluetooth
  lcd.begin(16, 2);    // Inicializar la LCD

  // Inicializar los pines de los flip-flops como salida y apagarlos al inicio
  for (int i = 0; i < 16; i++) {
    pinMode(flipFlopPins[i], OUTPUT);
    digitalWrite(flipFlopPins[i], LOW);  // Apagar todas las salidas
  }

  // Inicializar los pines de entrada
  for (int i = 0; i < 16; i++) {
    pinMode(inputPins[i], INPUT);
  }

  // Inicializar pines de LEDs
  pinMode(pinLEDJugar, OUTPUT);
  pinMode(pinLEDGameOver, OUTPUT);
  pinMode(pinLEDGanar, OUTPUT);

  // Inicializar el pin 19 en HIGH (siempre positivo)
  pinMode(pin19, OUTPUT);
  digitalWrite(pin19, HIGH);

  // Inicializar el pin 18 como salida y comenzar en HIGH
  pinMode(pin18, OUTPUT);
  digitalWrite(pin18, HIGH);  // Pin 18 comienza alto

  // Inicializar LEDs en LOW
  digitalWrite(pinLEDJugar, LOW);
  digitalWrite(pinLEDGameOver, LOW);
  digitalWrite(pinLEDGanar, LOW);

  actualizarLCD(); // Mostrar el estado inicial en la LCD
}

String obtenerNombreEstado(EstadoJuego estado) {
    switch (estado) {
        case JUGANDO:
            return "Jugando";
        case CONFIGURANDO:
            return "Configurando";
        case GAME_OVER:
            return "Game Over";
        case GANASTE:
            return "Ganaste";
        default:
            return "Estado desconocido";
    }
}

void actualizarLEDS() {
    // Apagar todos los LEDs inicialmente
    digitalWrite(pinLEDJugar, LOW);
    digitalWrite(pinLEDGameOver, LOW);
    digitalWrite(pinLEDGanar, LOW);

    // Encender el LED correspondiente según el estado
    switch (estadoActual) {
        case JUGANDO:
            digitalWrite(pinLEDJugar, HIGH); // Encender LED para Jugando
            break;
        case CONFIGURANDO:
            // No encender LEDs en estado Configurando
            break;
        case GAME_OVER:
            digitalWrite(pinLEDGameOver, HIGH); // Encender LED para Game Over
            break;
        case GANASTE:
            digitalWrite(pinLEDGanar, HIGH); // Encender LED para Ganaste
            break;
    }
}

// Función para contar cuántas bombas están encendidas
int contarBombasEncendidas() {
    int contador = 0;
    for (int i = 0; i < 16; i++) {
        if (estadosBombas[i]) { // Si la bomba está encendida
            contador++;
        }
    }
    return contador; // Devolver el total de bombas encendidas
}

// Función para verificar si una posición ya fue jugada
bool posicionYaJugada(int posicion) {
    return posicionesJugadas[posicion - 1];
}

// Función para marcar una posición como jugada
void marcarPosicionJugada(int posicion) {
    posicionesJugadas[posicion - 1] = true;
}

void loop() {
  // En el loop, en la sección donde se lee el Bluetooth:
    if (bluetooth.available() > 0) {
        String mensaje = bluetooth.readStringUntil('\n');
        Serial.println(mensaje);

        if (estadoActual == CONFIGURANDO) {
            Serial.println("Error: Modo configuración. Solo se permiten configuraciones.");
            return;
        }

        int bomba = mensaje.toInt();

        if (bomba >= 1 && bomba <= 16) {
            if (posicionYaJugada(bomba)) {
                Serial.println("Error: La posición ya fue jugada.");
                return;
            }

            int pin = inputPins[bomba - 1];
            int estadoBomba = digitalRead(pin);

            if (estadoBomba == HIGH) {
                Serial.println("¡Hay bomba en la posición " + String(bomba) + "!");
                estadoActual = GAME_OVER;
                actualizarLCD();
            } else {
                Serial.println("No hay bomba en la posición " + String(bomba) + ".");
                puntaje++;
                marcarPosicionJugada(bomba);
                actualizarLCD();

                if (puntaje >= totalSeguras) {
                    estadoActual = GANASTE;
                    Serial.println("¡Ganaste!");
                }
            }
        } else {
            Serial.println("Entrada inválida. Ingrese un número entre 1 y 16.");
        }
    }


  // Actualiza el conteo de bombas
  totalBombas = contarBombasEncendidas(); // Actualizar el total de bombas encendidas
  totalSeguras = totalPosiciones - totalBombas; // Calcular el total de posiciones seguras

  // Revisar si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Leer el comando del puerto serial

    input.trim();  // Eliminar espacios en blanco y saltos de línea extras

  if (input.equals("reset")) {
      // Reseteo de flip-flops
      Serial.println("Reseteando flip-flops");

      // Apagar todos los flip-flops y resetear su estado
      for (int i = 0; i < 16; i++) {
          digitalWrite(flipFlopPins[i], LOW);  // Apagar todos los pines
          estadosBombas[i] = false;  // Restablecer el estado de todas las bombas
      }

      // Enviar un pulso de reset
      digitalWrite(pin19, LOW);  // Poner el pin 19 en LOW (reset)
      delay(100);
      digitalWrite(pin19, HIGH); // Volver el pin 19 a HIGH

      // Restablecer el puntaje y cambiar el estado del juego
      puntaje = 0; // Restablecer el puntaje
      estadoActual = CONFIGURANDO; // Cambiar a estado CONFIGURANDO
      Serial.println("Juego reseteado. Estado cambiado a CONFIGURANDO.");
      actualizarLCD(); // Actualizar la pantalla LCD
    } else if (input.equals("b")) {
      //int bombasEncendidas = contarBombasEncendidas(); // Contar bombas encendidas
      Serial.print("Bombas encendidas: ");
      Serial.println(totalBombas); // Imprimir el número de bombas encendidas

    } else if (input.equals("S")) {
      // Comando 'S' para devolver el estado actual de las bombas
      String bombStates = "";
      for (int i = 0; i < 16; i++) {
        int state = digitalRead(inputPins[i]);  // Leer el estado de cada pin
        bombStates += String(state);  // Añadir el estado a la cadena
        if (i < 15) {
          bombStates += ",";  // Añadir coma entre los estados, excepto después del último
        }
      }
      Serial.println(bombStates);  // Enviar el estado de las bombas como una cadena de números separados por comas

    } else if (input.equals("config")) {
        estadoActual = CONFIGURANDO; // Cambiar a estado "Jugando"
        Serial.println("Configurando.");
        actualizarLCD(); // Actualizar la pantalla LCD
        actualizarLEDS(); // Actualizar los LEDs

    }  else if (input.equals("start")) {
        estadoActual = JUGANDO; // Cambiar a estado "Jugando"
        Serial.println("Juego iniciado.");
        actualizarLCD(); // Actualizar la pantalla LCD
        actualizarLEDS(); // Actualizar los LEDs

    } else if (input.equals("gameover")) {
        estadoActual = GAME_OVER; // Cambiar a estado "Game Over"
        Serial.println("Juego terminado.");
        actualizarLCD(); // Actualizar la pantalla LCD
        actualizarLEDS(); // Actualizar los LEDs

    } else if (input.equals("ganaste")) {
        estadoActual = GANASTE; // Cambiar a estado "Ganaste"
        Serial.println("¡Ganaste!");
        actualizarLCD(); // Actualizar la pantalla LCD
        actualizarLEDS(); // Actualizar los LEDs

    }else if (input.equals("estados")) {
    Serial.println(obtenerNombreEstado(estadoActual)); // Imprime el nombre del estado actual
} else {
      // Comando para activar una bomba
      int bomba = input.toInt();  // Convertir el input a número entero

      // Verificar si el número está entre 1 y 16
      if (bomba >= 1 && bomba <= 16) {

        // Validar que el estado actual permita agregar bombas
        if (estadoActual != CONFIGURANDO) {
            Serial.println("Error: No se pueden agregar bombas en este estado.");
            return; // No procesar más comandos
        }
        
        int pin = flipFlopPins[bomba - 1];  // Convertir la posición a pin (restar 1)

        if (!estadosBombas[bomba - 1]) {  // Solo activar si no está ya activado
          estadosBombas[bomba - 1] = true;  // Cambiar el estado de la bomba a activo
          Serial.print("Activando bomba en la posición: ");
          Serial.println(bomba);

          // Enviar un pulso de reloj (CLK) y activar el flip-flop correspondiente
          digitalWrite(pin18, LOW);  // Bajar el CLK para flip-flops
          delay(10);                 // Añadir un pequeño retardo para asegurar la activación
          digitalWrite(pin, HIGH);   // Encender el flip-flop correspondiente
          delay(100);
          digitalWrite(pin18, HIGH); // Subir el CLK (volver a HIGH)
        } else {
          Serial.print("La bomba en la posición ");
          Serial.print(bomba);
          Serial.println(" ya está activada.");
        }
      } else {
        Serial.println("Entrada inválida. Ingrese un número entre 1 y 16 o 'reset'.");
      }
    }
  }
  if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');  // Leer el comando del puerto serial
      input.trim();  // Eliminar espacios en blanco

      if (input.equals("reset")) {
          // ... (tu código existente)

      } else if (input.equals("start")) {
          estadoActual = JUGANDO; // Cambiar a estado "Jugando"
          Serial.println("Juego iniciado.");
          actualizarLCD(); // Actualizar la pantalla LCD

      } else if (input.equals("gameover")) {
          estadoActual = GAME_OVER; // Cambiar a estado "Game Over"
          Serial.println("Juego terminado.");
          actualizarLCD(); // Actualizar la pantalla LCD

      } else if (input.equals("ganaste")) {
          estadoActual = GANASTE; // Cambiar a estado "Ganaste"
          Serial.println("¡Ganaste!");
          actualizarLCD(); // Actualizar la pantalla LCD
      } else if (input.equals("config")) {
          estadoActual = CONFIGURANDO; // Cambiar a estado "Configurando"
          Serial.println("Configurando...");
          actualizarLCD(); // Actualizar la pantalla LCD
      } else {
          // Manejo de activación de bombas
          int bomba = input.toInt();
          // ... (tu código existente)
      }
  }
  // Actualizar los LEDs de estado
  actualizarLEDS();
  delay(100);  // Retraso para no saturar la lectura del bucle
}