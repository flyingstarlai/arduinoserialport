
enum TASKS {
  PING,
  TURN_LED_ON,
  TURN_LED_OFF,
  OPEN_GATE,
  CLOSE_GATE,  
};


boolean debug = true;

const byte numChars = 10;
char receivedChars[numChars];

boolean newData = false;


void setup() 
{

     pinMode(8, OUTPUT); 

     Serial.begin(9600);
     sendStartMessage();
}

void loop() 
{
     if (Serial.available() > 0)     {  recvWithStartEndMarkers(); }
     if (newData) { parseData(); }
}     


void sendStartMessage()
{
     
     Serial.println(" "); 
     Serial.println("Arduino Gate Automation"); 
     Serial.println("TSU Maker"); 
     Serial.println(" ");
     
     if (debug) { Serial.println("Debug is on"); }
           else { Serial.println("Debug is off"); }
    Serial.println(" "); 
}



  
void parseData()
{  
        newData = false;    
        if (debug) { Serial.println( receivedChars ); }    


        // PING
        if (strcmp(receivedChars, "PING")  == 0)
        {
            Serial.println("PING");
            
            digitalWrite(8, HIGH); 
            delay(100);
            digitalWrite(8, LOW); 
            delay(100);
            digitalWrite(8, HIGH); 
            delay(100);
            digitalWrite(8, LOW); 
            
            sendFeedBack(PING);
        }

        if (strcmp(receivedChars, "START")  == 0)
        {
            sendStartMessage();
        }  

        if (strcmp(receivedChars, "LEDON")  == 0)
        {
          digitalWrite(8,HIGH); 
          sendFeedBack(TURN_LED_ON);
        }

        if (strcmp(receivedChars, "LEDOFF")  == 0)
        {
          digitalWrite(8,LOW); 
         sendFeedBack(TURN_LED_OFF);

        }
        
}






void recvWithStartEndMarkers() 
{

     static boolean recvInProgress = false;
     static byte ndx = 0;
     char startMarker = '<';
     char endMarker = '>';

     char rc;

     if (Serial.available() > 0) 
     {
          rc = Serial.read();

          if (recvInProgress == true) 
          {
               if (rc != endMarker) 
               {
                    receivedChars[ndx] = rc;
                    ndx++;
                    if (ndx >= numChars) { ndx = numChars - 1; }
               }
               else 
               {
                     receivedChars[ndx] = '\0'; // terminate the string
                     recvInProgress = false;
                     ndx = 0;
                     newData = true;
               }
          }

          else if (rc == startMarker) { recvInProgress = true; }
     }

}




void sendFeedBack(int task)
{
  switch(task) {
     case PING:
       Serial.println(" ");Serial.print("Feedback from: "); Serial.println("PING");
       break;
    case TURN_LED_ON:
       Serial.println(" ");Serial.print("Feedback from: "); Serial.println("TURN LED ON");
       break;
    case TURN_LED_OFF:
       Serial.println(" ");Serial.print("Feedback from: "); Serial.println("TURN LED OFF");
       break;
       
  }
 
}





