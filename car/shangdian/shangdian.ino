int speedPin_M1 = 5;     //M1 Speed Control
int speedPin_M2 = 6;     //M2 Speed Control
int directionPin_M1 = 4;     //M1 Direction Control
int directionPin_M2 = 7;     //M1 Direction Control

void setup(){ 
  Serial.begin(9600);
                              
}

void loop(){
int a;
  //Serial.flush();
  while(Serial.available()==0)
  {
    
    
   }
   Serial.flush();
  while(Serial.available()!=0)
  {
    
    Serial.println("fuck");
    a=Serial.read() -'0';
    
    if (a==1)//前进
    {
     
 carAdvance(100,100);
    delay(2000);
   
      }
    if(a==2)//后退
    {
       carBack(100,100);
    delay(2000);
    
      }
    if(a==3)//左拐
    {
       carTurnLeft(70,150);
    delay(2000);
   
      }
      if(a==4)//右拐
      {
          carTurnRight(150,70);
    delay(2000);
    
        }
       if(a==5)//后退
      {
          carBack(100,100);
    delay(2000);
    
        }
      
    if(a==0)
    {carStop();
      }
 
  }
  
}

void carStop(){                 //  Motor Stop
  digitalWrite(speedPin_M2,0); 
  digitalWrite(directionPin_M1,LOW);    
  digitalWrite(speedPin_M1,0);   
  digitalWrite(directionPin_M2,LOW);    
}   

void carTurnLeft(int leftSpeed,int rightSpeed){         //Turn Left
  analogWrite (speedPin_M2,leftSpeed);              //PWM Speed Control
  digitalWrite(directionPin_M1,LOW);    
  analogWrite (speedPin_M1,rightSpeed);    
  digitalWrite(directionPin_M2,LOW);
} 

void carTurnRight(int leftSpeed,int rightSpeed){       //Turn Right
  analogWrite (speedPin_M2,leftSpeed);
  digitalWrite(directionPin_M1,LOW);   
  analogWrite (speedPin_M1,rightSpeed);    
  digitalWrite(directionPin_M2,LOW);
}

void carBack(int leftSpeed,int rightSpeed){             //Move backward
  analogWrite (speedPin_M2,leftSpeed);
  digitalWrite(directionPin_M1,HIGH);    
  analogWrite (speedPin_M1,rightSpeed);    
  digitalWrite(directionPin_M2,HIGH);
}
void carAdvance(int leftSpeed,int rightSpeed){           //Move forward
  analogWrite (speedPin_M2,leftSpeed);
  digitalWrite(directionPin_M1,LOW);    
  analogWrite (speedPin_M1,rightSpeed);    
  digitalWrite(directionPin_M2,LOW);
}
