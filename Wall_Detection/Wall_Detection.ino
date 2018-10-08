#define frontDistancePin A4
#define rightDistancePin A5
#define frontLED 8
#define rightLED 2
#define frontDistanceThreshold 150
#define rightDistanceThreshold 200

short frontDistance;
short rightDistance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(frontLED,OUTPUT);
  pinMode(2,OUTPUT);
  digitalWrite(frontLED, LOW);
  digitalWrite(rightLED, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  frontDistance = analogRead(frontDistancePin);
  rightDistance = analogRead(rightDistancePin);
  Serial.print(frontDistance);
  Serial.print(',');
  Serial.println(rightDistance);
  if (frontDistance > frontDistanceThreshold) {
    digitalWrite(frontLED, HIGH);
  } else {
    digitalWrite(frontLED, LOW);
  }
  if (rightDistance > rightDistanceThreshold) {
    digitalWrite(rightLED, HIGH);
  } else {
    digitalWrite(rightLED, LOW);
  }
}
