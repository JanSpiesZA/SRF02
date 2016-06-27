

#define CMD_REAL_RANGE_TX_cm      84
#define CMD_REAL_RANGE_NO_TX_cm   81

int indata = 0;
int adr = 1;
int sendflag = 1;
String readString;

int numSensors = 0;
int sensorAddr[] = {0,1};     //Array used to store the addresses of the SRF02 sensors in the order in which they must be read

void setup() {
        Serial.begin(9600);        
        Serial3.begin(9600);  //SRF modules
        
        Serial.println("Started");
        //sizeof give the amount of bytes used in the array and not the actual elements. When using int every element is 2 bytes long
        //  therefore the value must be divided by the siezeof(int) to get the total amount of elements in the array
        numSensors = sizeof(sensorAddr)/ sizeof(int);    
        Serial.println(numSensors);        
        delay(1000);
}

void loop()
{ 
  if (sendflag == 1)
  {        
    for (int n = 0; n <= numSensors-1; n++)
    { 
      Serial.print("Sensor adr: ");
      Serial.println(sensorAddr[n]);
      Serial3.write(sensorAddr[n]);
      Serial3.write(CMD_REAL_RANGE_TX_cm);
      Serial.println("Send");

//      while (2 >= Serial3.available())
//      {
//       indata = Serial3.read()<<8;
//       indata |= Serial3.read();
//       Serial.println(indata); 
//      }
    }
    sendflag = 0;
  }
  delay(500);        
}

//Ping a sensors without sending the distance data back
void ping(int _sensorAddr)
{
   Serial.print("PING Sensor with adr: #");
   Serial.println(_sensorAddr);
   Serial3.write(_sensorAddr);
   Serial3.write(CMD_REAL_RANGE_NO_TX_cm);
}

int getRange(int _sensorAddr)
{
  int rxData = Serial3.read()<<8;
  rxData |= Serial3.read();
  return rxData;
}

void serialEvent3()
{
  if (Serial3.available() >= 2)
  {
    indata = Serial3.read()<<8;
    indata |= Serial3.read();
    Serial.println(indata);
    sendflag = 1;
    delay(1000);    
  }    
}
