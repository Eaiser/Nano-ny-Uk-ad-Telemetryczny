#include <Wire.h>
#include <SPI.h>                                       
#include <SD.h> 
byte sendData[85];
int x=0;
int counter=0;
byte data[85];
byte noData[85] = {0};
byte gatewayMac[8] = {0x00,0x13,0xA2,0x00,0x41,0x7C,0x48,0x1E};
int frameType = 99;
bool frame = false;
bool request =false;
bool moduleFlag = false;
File plik;

void setup() {
  Wire.begin(8);                
  Wire.onReceive(receiveEvent); 
  Serial1.begin(9600);
  Serial.begin(9600);           
  
  if (!SD.begin(17))                                       
  {
     Serial.println("Nie wykryto karty(ERR)");           
     return;                                              
  }
   
  Serial.println("Karta Wykryta (3/3))");
   if (SD.exists("test2.txt"))                               
  {
    
   Serial.println("Plik o podanej nazwie istnieje !");  
   
  } 
  
  else                                                    
  {
    
   plik = SD.open("test2.txt", FILE_WRITE);                
   Serial.println("Utworzono plik o nazwie 123.csv");
  
  }
}

void loop() {



   plik = SD.open("test2.txt", FILE_WRITE);
   if (plik){        
    boolean flag = false; 
   for(int i=0;i<22;i++){
      char value = char(data[i]);                 
      if(isspace(value) || isdigit(value) || char(value)=='-' || char(value)=='.'){
        plik.print(value);
        flag = true;
      } 
    }
      if(flag == true){  
      plik.println();
      flag = false;
    }
    plik.close();


    }
    else {
   
  }
  

   readFrame();
   
   
}

void receiveEvent(int howMany) {
  int i=0;
  while (1 < Wire.available()) { 
    
   data[i] = Wire.read();
   if((char)data[0] == '3' && (char)data[2] == '0' && (char)data[3] == '5') {
    moduleFlag = true; 
   }
     if(i>4){
     sendData[x] = data[i];
     x++;
   }
     i++;
    }

 data[i+1] = Wire.read();
 
 counter ++;
 
 if(counter == 4){
  x = 0;
  counter = 0;
  if(request == true || moduleFlag){
    Serial.println();
    generateFrame(3,gatewayMac,sendData);
   }
  }

}

void readFrame(){
  if(Serial1.available()>101){
    if(request == true){
      request = false;
    }else{
              
               if(Serial1.read() == 0x7E){
              
              for(int i=2;i<102;i++){
                      if(i==15){
                           
                            frameType=(Serial1.read());
                            Serial.print(frameType);
                            if(frameType == 6){
                              request = true;
                             }else if(frameType == 2){
                                generateFrame(1,gatewayMac,noData);
                             }
                      
                      }
                      else{
                        byte discardByte = Serial1.read();
                      }
                 }
                   
               }
    }
  
 }

}
void generateFrame(int frameType, byte destinationMac[], byte data[85]){


      byte checksum = 0x00;
      Serial1.write(0x7E);
      Serial1.write((byte)0x0);
      Serial1.write(0x62);
      Serial1.write((byte)0x00);
      Serial1.write((byte)0x00);
      
      for(int i=0;i<8;i++){
        Serial1.write(destinationMac[i]);
        checksum += destinationMac[i];
      }
   
     
       Serial1.write((byte)0x00);
        switch(frameType){
        case 1:
        Serial1.write((byte)0x01);
        checksum += 0x01;
         for(int i=0;i<86;i++){
        Serial1.write((byte)0x00);
        }
        break;
             
        Serial1.write((byte)0x02);
        for(int i=0;i<86;i++){
        Serial1.write((byte)0x00);
        }
        checksum += 0x02;
        break;
        case 3:
        Serial1.write((byte)0x03);
        for(int i=0;i<86;i++){
         if(i<68){
           Serial1.write(data[i]);
           checksum += data[i];
         }else{
          Serial1.write(0x00);
         }
       
        }
        checksum += 0x03;

        
       break;
     }     
}
