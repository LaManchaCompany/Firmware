void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:


      all_off();
      digitalWrite(7, HIGH);   
    delay(10);
      all_off();
      digitalWrite(6, HIGH);   
    delay(10); 
      all_off();
      digitalWrite(5, HIGH);   
    delay(10);    
      all_off();                  
      digitalWrite(4, HIGH);    
    delay(10); 


}

void all_off()
{


   digitalWrite(7, LOW);   // turn the LED on (HIGH is the voltage level)
   digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(5, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  
  }

