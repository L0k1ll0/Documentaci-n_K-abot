#include <Servo.h>

// Definir pines
const int trigPin = 2;        // Pin del Trig del sensor ultrasónico
const int echoPin = 3;        // Pin del Echo del sensor ultrasónico
const int baseServoPin = 9;   // Pin del servo de la base
const int armServo1Pin = 10;  // Pin del primer servo del brazo
const int armServo2Pin = 11;  // Pin del segundo servo del brazo

// Crear objetos Servo
Servo baseServo;              // Servo para la base
Servo armServo1;              // Primer servo del brazo
Servo armServo2;              // Segundo servo del brazo

// Variables
long duration;                // Duración del pulso del sensor ultrasónico
int distance;                 // Distancia medida por el sensor ultrasónico
int maxDistance = 30;         // Máxima distancia de seguimiento en cm
int minDistance = 10;         // Mínima distancia para evadir en cm
int pos = 0;                  // Posición actual del servo de la base

void setup() {
  // Iniciar comunicación serial
  Serial.begin(9600);

  // Configurar pines
  pinMode(trigPin, OUTPUT);   // Configurar el pin del Trig como salida
  pinMode(echoPin, INPUT);    // Configurar el pin del Echo como entrada

  // Adjuntar servos a pines
  baseServo.attach(baseServoPin);  // Conectar el servo de la base al pin correspondiente
  armServo1.attach(armServo1Pin);  // Conectar el primer servo del brazo al pin correspondiente
  armServo2.attach(armServo2Pin);  // Conectar el segundo servo del brazo al pin correspondiente

  // Inicializar servos
  baseServo.write(90);  // Posición inicial del servo de la base
  armServo1.write(90);  // Posición inicial del primer servo del brazo
  armServo2.write(90);  // Posición inicial del segundo servo del brazo
}

void loop() {
  // Girar la base en 180 grados
  for (pos = 0; pos <= 180; pos += 1) {
    baseServo.write(pos);      // Mover el servo de la base a la posición actual
    delay(10);                 // Retardo para controlar la velocidad del movimiento
    distance = lectura_distancia(); // Obtener la distancia medida por el sensor

    if (distance <= maxDistance) {
      // Si detecta un objeto, sigue el objeto
      detectar_objeto();
    }
  }

  for (pos = 180; pos >= 0; pos -= 1) {
    baseServo.write(pos);      // Mover el servo de la base a la posición actual
    delay(10);                 // Retardo para controlar la velocidad del movimiento
    distance = lectura_distancia(); // Obtener la distancia medida por el sensor

    if (distance <= maxDistance) {
      // Si detecta un objeto, sigue el objeto
      detectar_objeto();
    }
  }
}

int lectura_distancia() {
  // Enviar pulso ultrasónico
  digitalWrite(trigPin, LOW);  // Asegurar que el Trig esté en LOW
  delayMicroseconds(2);        // Esperar 2 microsegundos
  digitalWrite(trigPin, HIGH); // Poner el Trig en HIGH
  delayMicroseconds(10);       // Esperar 10 microsegundos
  digitalWrite(trigPin, LOW);  // Poner el Trig en LOW

  // Leer el tiempo del pulso
  duration = pulseIn(echoPin, HIGH); // Leer el tiempo que tarda en llegar el pulso

  // Calcular la distancia en cm
  distance = duration * 0.034 / 2;   // Convertir tiempo en distancia

  return distance;                   // Retornar la distancia medida
}

void detectar_objeto() {
  while (true) {
    distance = lectura_distancia();  // Obtener la distancia medida por el sensor

    if (distance > maxDistance) {
      break;                         // Si el objeto se aleja, volver a buscar
    } else if (distance < minDistance) {
      armServo1.write(0);            // Mover el primer servo del brazo para evadir
      armServo2.write(180);          // Mover el segundo servo del brazo en dirección opuesta
    } else {
      // Seguir el objeto
      armServo1.write(180);          // Mover el primer servo del brazo para seguir el objeto
      armServo2.write(0);            // Mover el segundo servo del brazo en dirección opuesta
    }

    // Girar la base para seguir el objeto
    baseServo.write(pos);            // Mantener la posición del servo de la base
    delay(5);                        // Retardo para controlar la velocidad del movimiento
  }
}
