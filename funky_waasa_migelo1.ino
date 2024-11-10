// MMA7361 pins (connected to analog inputs)
#define X_PIN A0
#define Y_PIN A1
#define Z_PIN A2

// Ultrasonic pins
#define TRIG_PIN 9
#define ECHO_PIN 10

// Set the threshold values
int bpmThreshold = 100;  // Set threshold for BPM
int distanceThreshold = 20;  // Set distance threshold for ultrasonic sensor

void setup() {
  Serial.begin(9600);

  // Setup Ultrasonic Sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Setup LED Pin
  pinMode(13, OUTPUT);

  // Setup Potentiometer for BPM
  pinMode(A3, INPUT);
}

void loop() {
  // Simulate BPM (using potentiometer value as input)
  int bpm = analogRead(A3);  // Read potentiometer to simulate BPM
  bpm = map(bpm, 0, 1023, 60, 120);  // Map potentiometer value to BPM range (60 to 120)

  // Measure distance from Ultrasonic Sensor
  long duration, distance;
  
  // Send a pulse to trigger the sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin to get the duration of the pulse
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate the distance in centimeters (speed of sound is 343 m/s)
  distance = (duration / 2) * 0.0343;

  // Read Accelerometer values (X, Y, Z axis)
  int xValue = analogRead(X_PIN);  
  int yValue = analogRead(Y_PIN);  
  int zValue = analogRead(Z_PIN);  

  // Convert accelerometer values to a more readable form (optional)
  float x = map(xValue, 0, 1023, -10, 10);
  float y = map(yValue, 0, 1023, -10, 10);
  float z = map(zValue, 0, 1023, -10, 10);

  // Print data to serial monitor
  Serial.print("BPM: ");
  Serial.println(bpm);
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Accelerometer - X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.println(z);

  // Blink LED if BPM exceeds threshold
  if (bpm > bpmThreshold) {
    digitalWrite(13, HIGH);  // Blink LED on
  } else {
    digitalWrite(13, LOW);   // Blink LED off
  }

  delay(1000);  // Delay for a bit
}
