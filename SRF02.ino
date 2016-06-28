//First experiments in using the SRF02 with SERIAL comms
//Allows distance measurements to be sent via Serial to PC

#define CMD_REAL_RANGE_TX_cm      84
#define CMD_REAL_RANGE_NO_TX_cm   81
#define CMD_GET_RANGE             94

int sendflag = 1;
String readString;
int oldTime = 0;
String serialData = "";
boolean done = false;
int index = 0;

int sensorAddr[] = {3,0,4,1,5,2,6};     //Array used to store the addresses of the SRF02 sensors in the order in which they must be read
//sizeof give the amount of bytes used in the array and not the actual elements. When using int every element is 2 bytes long
//  therefore the value must be divided by the siezeof(int) to get the total amount of elements in the array
const int numSensors = sizeof(sensorAddr)/ sizeof(int);    
int sensorDist[numSensors];

void setup() 
{
  Serial.begin(115200);        
  Serial3.begin(9600);  //Comms port for SRF modules        
  Serial.println("US Interface Started");
  delay(1000);
}

void loop()
{   
  //Lees seriedata tot 'n EOL karakter gekry word
  if (Serial.available() >0)
  {
    char c = Serial.read();
    
    switch(c)
    {
      case '<':      //new command line starts
      index = 0;
      break;
      
      case '\r':
        done = true;        
        break;
        
      default:
        serialData += c;        //Add serial data input char to string
        break;
    }
  }   

  if (done)
  {
    done = false;
    char testChar = serialData.charAt(0);

    switch(testChar)
    {
      case 's':
      {
        for (int n = 0; n <= numSensors-1; n++)
        { 
          sensorDist[n] = getRange(sensorAddr[n]);  
          Serial.print(sensorAddr[n]);
          Serial.print(':');
          Serial.print(sensorDist[n]);  
          Serial.print('\t');    
          delay(5);
        }
        Serial.println();
        break;
      }
    }
    serialData = "";
  }

  
  //Ping distance to nearest object for each of the sensors
  for (int n = 0; n <= numSensors-1; n++)
  { 
    ping(sensorAddr[n]);
    delay(10);
  }
}

//Ping a sensors without sending the distance data back
void ping(int _sensorAddr)
{
   Serial3.write(_sensorAddr);
   Serial3.write(CMD_REAL_RANGE_NO_TX_cm);
}

int getRange(int _sensorAddr)
{
  Serial3.write(_sensorAddr);
  Serial3.write(CMD_GET_RANGE);

  while(Serial3.available() < 2);     //Wait until 2 bytes received
  int rxData = Serial3.read()<<8;
  rxData |= Serial3.read();
  return rxData;
}
