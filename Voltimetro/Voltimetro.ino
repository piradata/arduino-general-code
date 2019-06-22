#define redundancia 1024.0

#define VT0 0.0
#define VT1 1.5
#define VT2 3.6
#define VT3 4.8
#define VT4 6.0
#define VT5 7.2

float V0, V1, V2, V3, V4, V5;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(10, INPUT_PULLUP);

  Serial.println("coloque " + String(VT0) + " Volts");
  while (digitalRead(10) == 1);
  while (digitalRead(10) == 1);
  V0 = VoltRead();
  while (digitalRead(10) == 0);
  while (digitalRead(10) == 0);
  Serial.println("coloque " + String(VT1) + " Volts");
  while (digitalRead(10) == 1);
  while (digitalRead(10) == 1);
  V1 = VoltRead();
  while (digitalRead(10) == 0);
  while (digitalRead(10) == 0);
  Serial.println("coloque " + String(VT2) + " Volts");
  while (digitalRead(10) == 1);
  while (digitalRead(10) == 1);
  V2 = VoltRead();
  while (digitalRead(10) == 0);
  while (digitalRead(10) == 0);
  Serial.println("coloque " + String(VT3) + " Volts");
  while (digitalRead(10) == 1);
  while (digitalRead(10) == 1);
  V3 = VoltRead();
  while (digitalRead(10) == 0);
  while (digitalRead(10) == 0);
  Serial.println("coloque " + String(VT4) + " Volts");
  while (digitalRead(10) == 1);
  while (digitalRead(10) == 1);
  V4 = VoltRead();
  while (digitalRead(10) == 0);
  while (digitalRead(10) == 0);
  Serial.println("coloque " + String(VT5) + " Volts");
  while (digitalRead(10) == 1);
  while (digitalRead(10) == 1);
  V5 = VoltRead();

}

void loop() {

  long int sensorValue = VoltRead();

  if (sensorValue >= V0 and sensorValue <= V1) {
    Serial.println(   ((sensorValue - V0) * (VT1 - VT0)) / (V1 - V0) + VT0, 4    );
  }
  else if (sensorValue > V1 and sensorValue <= V2) {
    Serial.println(   ((sensorValue - V1) * (VT2 - VT1)) / (V2 - V1) + VT1, 4    );
  }
  else if (sensorValue > V2 and sensorValue <= V3) {
    Serial.println(   ((sensorValue - V2) * (VT3 - VT2)) / (V3 - V2) + VT2, 4    );
  }
  else if (sensorValue > V3 and sensorValue <= V4) {
    Serial.println(   ((sensorValue - V3) * (VT4 - VT3)) / (V4 - V3) + VT3, 4    );
  }
  else if (sensorValue > V4 and sensorValue <= V5) {
    Serial.println(   ((sensorValue - V4) * (VT5 - VT4)) / (V5 - V4) + VT4, 4    );
  }
  else if (sensorValue > V5) {
    Serial.println(   ((sensorValue - V5) * (VT5 - VT4)) / (V5 - V4) + VT5, 4    );
  }

  //Serial.println((sensorValue/1023.0)*30.0,5);
  //Serial.println(digitalRead(10));

  delay(10);
}

long int VoltRead(void) {
  long int Value = 0;
  int _n = 0;
  while (_n < redundancia) {
    Value += analogRead(A2);
    _n++;
  }
  Value /= redundancia;
  return Value;
}
