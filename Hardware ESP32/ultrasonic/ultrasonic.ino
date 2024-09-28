const int trigPin = 5; 
const int echoPin = 18;
const int buzzerPin = 15;
unsigned long currentTime, previousTime, delayTime;


long duration;
int distance;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  currentTime = millis();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 100 && distance >= 2) {
    delayTime = map(distance, 2, 100, 200, 400); 

    digitalWrite(buzzerPin, HIGH);

    if (currentTime - previousTime >= delayTime) {
      digitalWrite(buzzerPin, LOW);
      previousTime = currentTime;
    }
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(100);
}