void setup()
{
  Serial.begin(9600);
  float a=100,b=100;
  Serial.println(tan(a/b));
}
void loop()
{
   float a=100,b=100;
    Serial.print("a=100;b=100:  ");
  float x=atan(1)*180/PI;
  Serial.println(x);
  
  delay(500);
  a=1;
  b=100;
     Serial.print("a=1;b=100:  ");
      x=atan(a/b)*180/PI;
   Serial.println(x);
    delay(500);
   a=100;
   b=1;
      Serial.print("a=100;b=1:  ");
       x=atan(a/b)*180/PI;
    Serial.println(x);
     delay(500);
  
}
