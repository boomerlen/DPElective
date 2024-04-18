static char buf[16];
static int up_to;

void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  up_to = 0;

  Serial.begin(115200);
}


void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.avilable != 0) {
    char incoming = Serial.read();
    if (incoming == '\n') {
      buf[up_to] = '\0';
      int val = atoi(buf);
      analogWrite(12, val);
    } else {
      buf[up_to] = incoming;
      up_to = (up_to == 15) ? 0 : up_to + 1;
    }
  }
}
