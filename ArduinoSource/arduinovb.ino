
enum TASKS_FEEDBACK {
  PING,
  OPEN_GATE,
  CLOSE_GATE, 
};


//IR Consts:
const int irReceiver = 2;                // 紅外線接收器
const int irLed  = 3;                    // 紅外線發射器
const unsigned int frequency = 38000;    // 發射頻率(單位: Hz)


boolean debug = true;
const byte numChars = 10;
char receivedChars[numChars];
boolean newData = false;
boolean readSensor = false;


void setup() 
{

    pinMode(8, OUTPUT); //LED
    pinMode(irReceiver, INPUT);           // 把 irReceiver 接腳設置為 INPUT
    pinMode(irLed, OUTPUT);               // 把 irLed 接腳設置為 INPUT
    Serial.begin(9600);
    sendStartMessage();
}

void loop() 
{
   if (Serial.available() > 0) { recvWithStartEndMarkers(); }
   if (newData) { parseData(); }
   if(readSensor){  readIRSensor();}
}     

void readIRSensor() {
  tone(irLed, frequency); 
  delay(5);     
  int ir_status = digitalRead(irReceiver);   // 讀取 irReceiver 的狀態
  if (ir_status == 0) {
     Serial.println("IR0");
     noTone(irLed);  
  } else
    Serial.println("IR1");      
  delay(5);  
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
            
            if(debug) sendFeedback(PING);
        }

        if (strcmp(receivedChars, "START")  == 0)
        {
            sendStartMessage();
        }  

        if (strcmp(receivedChars, "OPENGATE")  == 0)
        {
          readSensor = true;
            if(debug) sendFeedback(OPEN_GATE);
            digitalWrite(8,HIGH); 
        } 

        if (strcmp(receivedChars, "CLOSEGATE")  == 0)
        {
          readSensor = false;
            if(debug) sendFeedback(CLOSE_GATE);
            digitalWrite(8,LOW);
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




void sendFeedback(int task)
{
  switch(task) {
     case PING:
       Serial.println(" ");Serial.print("Feedback from: "); Serial.println("PING");
       break;
    case OPEN_GATE:
      Serial.println(" ");Serial.print("Feedback from: "); Serial.println("OPEN GATE");
      break;
    case CLOSE_GATE:
      Serial.println(" ");Serial.print("Feedback from: "); Serial.println("CLOSE GATE");
      break;  
  }
 
}





