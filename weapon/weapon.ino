   #define rubberCord A0
   #define laserPin 8
   
   bool stretched = false;
   int oldValue = 0;
   void setup(void) { 
     Serial.begin(9600);
     pinMode(rubberCord, INPUT);
     pinMode(laserPin, OUTPUT);
     digitalWrite(laserPin, LOW);
   } 
   void loop(void) { 
     int value; 
     value = analogRead(rubberCord);
     Serial.println(value);
     if(oldValue - value < -25 && !stretched){
      Serial.println("gespannen");
      digitalWrite(laserPin, HIGH);
      stretched = true;
     }
     
     if(oldValue - value > 25 && stretched){
      Serial.println("geschoten");
      stretched = false;
      digitalWrite(laserPin, LOW);
     }
     oldValue = value;
     delay(1000); 
   }
