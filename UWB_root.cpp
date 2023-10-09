#include <Arduino.h>
#include <M5Stack.h>
#include <painlessMesh.h>
#include <Arduino_JSON.h>

#define   MESH_SSID       "CoolSystem"
#define   MESH_PASSWORD   "secretCode"
#define   MESH_PORT       5555

String serData;

//---------------------Proto-functions--------------------//
//void sendMessage(); 
void receivedCallback(uint32_t from, String & msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback(); 
void nodeTimeAdjustedCallback(int32_t offset); 
void delayReceivedCallback(uint32_t from, int32_t delay);

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;
bool calc_delay = false;
SimpleList<uint32_t> nodes;

//-------------------The actual M5 code--------------------//
void setup() {
  M5.begin();
  Serial.begin(115200);
  mesh.setDebugMsgTypes(ERROR | DEBUG);  // set before init() so that you can see error messages
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);
}

void loop() {
  mesh.update();
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

