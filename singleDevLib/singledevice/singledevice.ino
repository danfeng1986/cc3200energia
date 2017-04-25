

#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <QiaqiaMqttSingleDevice.h>

//char AP_NAME[] = "qiaqaicloud_818";
//char AP_PASSWORD[] = "qiaqiaGo";

//char AP_NAME[50] = "JTXXX_";  //AP可变热点名时使用
//char AP_PASSWORD[] = "jttec888";

unsigned long SENDConfigStart = 0;
unsigned int SENDFirstTimeState = 1;
QiaqiaMQTTSD qiaqia;

void readCallback(int offsetindex, char *devack) {
  Serial.println("i am here 222");
  char *p;
  char one[] = {"3.7"};
  char two[] = {"23"};
  char three[] = {"1"};
  switch(offsetindex){
    case 1:
      p = one;
     break;
    case 2:
      p = two;
      break;
    case 3:
      p = three;
      break;
    default:
      break;
  }
  qiaqia.sdAck(devack, p);
}

void writeCallback(int offsetindex, float value) { 
    Serial.println("enter write callback!!!!!!!!!!");
    Serial.println(offsetindex);
    Serial.println(value);

}
/*
void timeCallback(int busport, int devaddr, char *datetime, char* devack){

  Serial.println(busport);
  Serial.println(devaddr);
  
  Serial.println("enter time callback");
  Serial.println(datetime);

  //用户填写子设备校时程序--写
  //根据budport和devaddr编写与子设备的通讯函数


  //用户填写子设备校时程序--读
  //根据budport和devaddr编写与子设备的通讯函数
  //qiaqia.gwAck(devack, "2016-12-21 14:51:23");
}

//通过devindex可以映射出子设备的busport和devaddr，
void restartCallback(int busport, int devaddr){
  Serial.println(busport);
  Serial.println(devaddr);

  if((busport == 0)&&(devaddr == 0)){
    Serial.println("enter gateway restart callback");
    delay(1000);
    PRCMSOCReset();  //Performs a software reset of a SOC
  }
  else {
    Serial.println("enter subdevice restart callback");
  }  //用户填写子设备复位程序，向子设备传输一个值，在子设备上映射出复位程序
}
*/


void setup()
{
  Serial.begin(115200);

//  Serial.print("Attempting to connect to Network named:");
//  Serial.print(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
//    delay(300);
//  }
/*
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  // We are connected and have an IP address.
  // Print the WiFi status.
  printWifiStatus();
*/
  //qiaqia.gwRTCInit();
  // Device initialization with gwindex(WiFi module), apikey and ipcloud
  //qiaqia.gwInit("5861ce35c8e0b10d40abd3f3", "57fb3f58d8207e1934bb2e88", "101.201.78.22");
  qiaqia.sdInit();
  //Activate read callback function
  qiaqia.setReadCallback(readCallback);
  //Activate write callback function
  qiaqia.setWriteCallback(writeCallback);
  //Activate time callback function
  //qiaqia.setGwAndDevTimeCallback(timeCallback);
  //Activate restart callback function
  //qiaqia.setGwAndDevRestartCallback(restartCallback);

  
}

void loop()
{
    //Connect to the Qiaqia Cloud platform, make sure this function in the loop()
    
    qiaqia.sdRun();
    //Serial.println("developer is testing!!");
    delay(50);

    
         if(SENDFirstTimeState == 1){
              SENDConfigStart = millis();
              SENDFirstTimeState = 0;
         }   
         if((millis() - SENDConfigStart) < 0){
              Serial.println("millis() runoff");  //按下时正赶上溢出
              SENDFirstTimeState = 1;  //下次重新标记按下时刻
         }
         if((millis() - SENDConfigStart) > 3000){ 
              qiaqia.sdValueChange(1, "1.6");
              qiaqia.sdValueChange(2, "2.6");
              qiaqia.sdValueChange(3, "3.6");

              SENDFirstTimeState = 1;
         }
         
          
}





