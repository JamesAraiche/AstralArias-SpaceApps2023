#include <Arduino.h>
#include <M5Atom.h>
#include <painlessMesh.h>
#include <Arduino_JSON.h>

#define   MESH_SSID       "CoolSystem"
#define   MESH_PASSWORD   "secretCode"
#define   MESH_PORT       5555

//-----------Helpful Structs-------------------//
typedef struct message_struct{
  String thisNodeID;
  String dist0;
  String dist1;
  String dist2;
}message_struct;

//------------Important variables/objects/structs-------------------//
String tagID = "0"; //Note: will evntually be changed to a general Node ID
//int mode = 1;      // Eventually make into a mode selection w/ button, [1=anchor, 0=tag]
String data = "!"; // to hold the distance data
message_struct distanceData;

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;
bool calc_delay = false;
SimpleList<uint32_t> nodes;

//-----------------Proto-Functions------------------------//
String distanceMsg();

void sendMessage(); 
void receivedCallback(uint32_t from, String & msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback(); 
void nodeTimeAdjustedCallback(int32_t offset); 
void delayReceivedCallback(uint32_t from, int32_t delay);

Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, &sendMessage ); // start with a one second interval

//--------------------The actual M5 stuff--------------------//
void setup()
{
  M5.begin(true, false, true);
  Serial2.begin(115200, SERIAL_8N1, 19, 22); // 8 bits no parity 1 stop, TX is G19, RX is G22
  M5.dis.fillpix(0xBBFFFF);
  //The UWB stuff
  Serial2.write("AT+anchor_tag=0,0\r\n"); // Set mode and ID
  delay(50);
  Serial2.write("AT+interval=5\r\n"); // Set the calculation precision. irrelevant in anchor mode
  delay(50);
  Serial2.write("AT+switchdis=1\r\n"); // Measure distance
  distanceData.thisNodeID = tagID;
  //The mesh stuff
  mesh.setDebugMsgTypes(ERROR | DEBUG);
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  M5.dis.fillpix(0x000000);

  while(Serial2.find("\r\nOK\r\n")){
      M5.dis.fillpix(0x800080);
  }
}

void loop()
{
  if(Serial2.available()){
    data = Serial2.readString();
    M5.dis.drawpix(12, 0x800080);

    while(data.length()<24){ //Ensures that string is correct size. $=no additional distance data [To be delt with in PC code]
      data+="a$$$$$$m";
    }
    Serial.print(data);
    distanceData.dist0=data.substring(data.indexOf('a'),data.indexOf('m'));
    data.remove(0,data.indexOf('m')+1);
    distanceData.dist1=data.substring(data.indexOf('a'),data.indexOf('m'));
    data.remove(0,data.indexOf('m')+1);
    distanceData.dist2=data.substring(data.indexOf('a'),data.indexOf('m'));
    data.remove(0,data.indexOf('m')+1);

    mesh.update(); //does all the mesh stuff
  }
  else{ //if serial1 isnt working, turn screen Red
    M5.dis.drawpix(12, 0xFF0000);
  }
}

//------------The Functions---------------------//
String distanceMsg(){ //Package the distance data into a nice JSON for the message
    JSONVar distanceJSON;
    distanceJSON["nodeID"] = distanceData.thisNodeID;
    distanceJSON["dist0"] = distanceData.dist0;
    distanceJSON["dist1"] = distanceData.dist1;
    distanceJSON["dist2"] = distanceData.dist2; //Note: before entering this function, concat a bunc of $ or something to replace missing distance points
    return JSON.stringify(distanceJSON);
} //Another note: distance data will not turn into coordinates until it meets the PC program

void sendMessage() {
  String msg = distanceMsg();
  mesh.sendSingle(3663488093, msg);
  Serial.print(msg.c_str());
}

void receivedCallback(uint32_t from, String & msg) {
  //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  //Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  //Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}

void changedConnectionCallback() {
  //Serial.printf("Changed connections\n");
  nodes = mesh.getNodeList();
  //Serial.printf("Num nodes: %d\n", nodes.size());
  //Serial.printf("Connection list:");

  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) {
    //Serial.printf(" %u", *node);
    node++;
  }
  //Serial.println();
  calc_delay = true;
}

void nodeTimeAdjustedCallback(int32_t offset) {
  //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  //Serial.printf("Delay to node %u is %d us\n", from, delay);
}



//-----------------Important functions--------------------//
void receivedCallback(uint32_t from, String & msg) {
  //Serial.printf("%s\n", msg.c_str());
  JSONVar recDistance = JSON.parse(msg.c_str());
  serData = JSON.stringify(recDistance["nodeID"])+JSON.stringify(recDistance["dist0"])+JSON.stringify(recDistance["dist1"])+JSON.stringify(recDistance["dist2"])+"\n";
  Serial.print(serData);
}

void newConnectionCallback(uint32_t nodeId) {
  //Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  //Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}

void changedConnectionCallback() { //This will be called when Root recieves Serial message from python requesting info
  //Serial.printf("Changed connections\n");
  nodes = mesh.getNodeList();
  //Serial.printf("Num nodes: %d\n", nodes.size());
  //Serial.printf("Connection list:");

  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) {
    //Serial.printf(" %u", *node);
    node++;
  }
  //Serial.println();
  calc_delay = true;
}

void nodeTimeAdjustedCallback(int32_t offset) {
  //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  //Serial.printf("Delay to node %u is %d us\n", from, delay);
}

