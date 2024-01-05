const byte analogPin = 0;   // Connect AN output of LSA08 to analog pin 0
const byte junctionPulse = 4;   // Connect JPULSE of LSA08 to pin 4
unsigned int junctionCount = 0;   // Variable to store junction count value
void setup() {
  pinMode(junctionPulse,INPUT);
  Serial.begin(9600);

}

void loop() {


  float readVal = analogRead(analogPin);    // Read value from analog pin

  // Convert voltage level into line position value
int  positionVal = ((float)readVal/921)*70;
  Serial.println(positionVal );


}
