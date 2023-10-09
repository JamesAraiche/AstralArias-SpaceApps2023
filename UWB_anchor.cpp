#include <Arduino.h>
#include <M5Atom.h>
#include <painlessMesh.h>
#include <Arduino_JSON.h>

#define   MESH_SSID       "CoolSystem"
#define   MESH_PASSWORD   "secretCode"
#define   MESH_PORT       5555

//-----------Helpful Structs-------------------//
typedef struct message_struct{
  int thisNodeID;
  String dist0;
  String dist1;
  String dist2;
}message_struct;

//------------Important variables/objects/structs-------------------//
//int ID = 0;        // Eventually make it an ID selection
//int mode = 1;      // Eventually make into a mode selection w/ button, [1=anchor, 0=tag]
String data = "?"; // to hold the distance data
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

//----------------The Actual M5 Code------------------------//
void setup()
{
  M5.begin(true, false, true);
  Serial2.begin(115200, SERIAL_8N1, 19, 22); // 8 bits no parity 1 stop, TX is G19, RX is G22
  M5.dis.fillpix(0xFFFF00);
  //The UWB stuff
  Serial2.write("AT+anchor_tag=1,3\r\n"); // Set mode and ID
  data = Serial2.readString();
  //Serial.print(data);
  //The mesh stuff
  mesh.setDebugMsgTypes(ERROR | DEBUG);
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);

  M5.dis.fillpix(0x00FF00);
}

void loop()
{
  mesh.update();
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
  mesh.sendBroadcast(msg);
  //Serial.printf("Sending message: %s\n", msg.c_str());
}

void receivedCallback(uint32_t from, String & msg) {
  //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  //Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
 // Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
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