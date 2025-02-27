long zero;
#define IR 17
#define LED 5
void setup() {
  pinMode(IR, INPUT);
  zero = 0;
  for(int i = 0; i < 100; i++){
    zero+=analogRead(IR);
  }
  zero/=100;
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(IR));
  digitalWrite(LED, map(analogRead(IR) - zero, 0, 1023, 0, 255));
}
