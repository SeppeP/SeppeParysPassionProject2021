#define btn1 3
#define btn2 4
#define SW 8
#define joy_x A1
#define joy_y A2

int button1Poll = 0;
int button2Poll = 0;

int joy_map = 10;

void setup() {
  Serial.begin(19200);

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  
  pinMode(SW, INPUT_PULLUP);
  pinMode(joy_x, INPUT);
  pinMode(joy_y, INPUT);

}

void loop() {
  //button1Poll = digitalRead(btn1);

    float joy_rx = analogRead(joy_x);
    float joy_ry = analogRead(joy_y);

    joy_rx = map(joy_rx, 1, 1024, -joy_map, joy_map+1);
    joy_ry = map(joy_ry, 1, 1024, joy_map, -joy_map-1);

    Serial.print(joy_rx);
    Serial.print(",");
    Serial.print(joy_ry);
    Serial.print(",");
    Serial.print(!digitalRead(SW));
    Serial.print(",");
    Serial.print(digitalRead(btn1));
    Serial.print(",");
    Serial.println(digitalRead(btn2));
    delay(16);
}
