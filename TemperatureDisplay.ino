#define CLK 8 // to MM5468 pin 21
#define dataBit  9 // to MM5468 pin 22
#define load 10 // to MM5468
#include "DHT.h"
#define DHTPIN 11
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// 7-segment display code array - first one is zero, etc.
byte segCode[] = {
  0b00111111, 0b00000110, 0b01011011,
  0b01001111, 0b01100110, 0b01101101,
  0b01111101, 0b00000111, 0b01111111,
  0b01100111, 0b00000000, 0b00111001, 0b01110110
};


void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(dataBit, OUTPUT);
  digitalWrite(CLK, 0);
  digitalWrite(dataBit, 0);
  digitalWrite(load, 1);
  dht.begin();
  Serial.begin(9600);
  Serial.println("DHTxx test!");

}

// the loop routine runs over and over again forever:
void loop() {


  //    ssrWriteMSB(segCode[3]); //1
  //    ssrWriteMSB(segCode[7]); //1
  //    ssrWriteMSB(segCode[1]); //1
  //    ssrWriteMSB(segCode[2]); //1
  //    zeroWrite(1);
  //    delay(5000);
  //    zeroWrite(33);
  //    for (int x = 0; x < 10000; x++)
  //    {
  //     writeInt(x);
  //     zeroWrite(1);
  //     delay(25);
  //    }
  //
  //
  //    //pulseLoad();
  //    delay(5000);
  float t = dht.readTemperature();
  int a = int(t*10);
  
  writeTemp(a);
  
  Serial.println(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  delay(1000);
  float h = dht.readHumidity();
    a = int(h*10);
  Serial.print(h);
  writeHumidity(a);
  delay(3000);

  
} // end loop


void pulseCLK()   {
  //delay(20);
  digitalWrite(CLK, 1);
  //delay(20);
  digitalWrite(CLK, 0);
}

void pulseLoad()
{
  digitalWrite(load, 1);
  digitalWrite(load, 0);
}


// MSB out first!
void ssrWriteMSB(byte value)   {
  for (int x = 0; x < 8; x++)   {
    byte temp = value & 0x80;
    if (temp == 0x80)   digitalWrite(dataBit, 1); // data bit HIGH
    else digitalWrite(dataBit, 0); // data bit LOW
    pulseCLK();
    value = value << 0x01; // shift left
  }
}
void  zeroWrite(byte num1)   {
  digitalWrite(dataBit, 0); // data bit LOW
  for (int x = 0; x < num1; x++) pulseCLK();
}
void  oneWrite(byte num1)   {
  digitalWrite(dataBit, 1); // data bit LOW
  for (int x = 0; x < num1; x++) pulseCLK();
}
void writeTemp(int x)
{
  ssrWriteMSB(segCode[11]);
  Serial.print("c");
  //Serial.print(x);
  int a = int(x) % 10;
  ssrWriteMSB(segCode[a]);
  Serial.print(a);
  a = (int(x / 10)) % 10;
  if (a != 0) {
    byte b = segCode[a]|0x80;
    ssrWriteMSB(b);
  }
  Serial.print(a);
  a = int(x/100)%10;
  Serial.print(a);
  ssrWriteMSB(segCode[a]);
  zeroWrite(1);
}
void writeHumidity(int x)
{
  ssrWriteMSB(segCode[12]);
  Serial.print("c");
  //Serial.print(x);
  int a = int(x) % 10;
  ssrWriteMSB(segCode[a]);
  Serial.print(a);
  a = (int(x / 10)) % 10;
  if (a != 0) {
    byte b = segCode[a]|0x80;
    ssrWriteMSB(b);
  }
  Serial.print(a);
  a = int(x/100)%10;
  Serial.print(a);
  ssrWriteMSB(segCode[a]);
  zeroWrite(1);
}
void writeInt(int x)
{
  if (x > 9999)
  {
    zeroWrite(32);
  }
  else
  {
    int a = int(x) % 10;
    ssrWriteMSB(segCode[a]);

    a = (int(x / 10)) % 10;
    if (a != 0) {
      ssrWriteMSB(segCode[a]);
    }
    else
    {
      if (x < 100)
      {
        ssrWriteMSB(segCode[10]);
      }
      else
      {
        ssrWriteMSB(segCode[0]);
      }

    }

    a = (int(x / 100) % 10);
    if (a != 0) {
      ssrWriteMSB(segCode[a]);
    }
    else
    {
      if (x < 1000)
      {
        ssrWriteMSB(segCode[10]);
      }
      else
      {
        ssrWriteMSB(segCode[0]);
      }

    }
    a = (int(x / 1000));
    if (a != 0) {
      ssrWriteMSB(segCode[a]);
    }
    else
    {
      if (x < 10000)
      {
        ssrWriteMSB(segCode[10]);
      }
      else
      {
        ssrWriteMSB(segCode[0]);
      }

    }

  }

}



