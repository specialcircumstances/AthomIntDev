/*
 * Project AthomIntDev
 * Description:
 * Author: SpecialCircumstances
 * Date:
 */
//#include "JsonParserGeneratorRK.h"
//#include "AthomIntDev.h"
//#include "athomdefs.h"
#include "athomdevice.h"

// We'll need one of these.

// Create an Athom Device
AthomDevice myDevice;

void debug(const String message) {
  Serial.print(message);
  uint32_t freemem = System.freeMemory();
  Serial.print("\t\t\t Free Mem: ");
  Serial.println(freemem);
}

void debugint(const int message) {
  String num = String(message);
  debug(String("INT: " + num));
}

void setup() {
 Serial.begin(9600);
 while(!Serial.isConnected()) Particle.process();
 debug("Hello. Please wait 2 secs.");
 delay(2000); // 3 sec delay
 // Create AthomDevice
 debug("Checking we have an AthomDevice");
 debug("Device name is: " + myDevice.getName());
 debug("Setting name to TESTY");
 myDevice.setName("TESTY");
 debug("Device name is: " + myDevice.getName());
 int nodeInt = 0;
 debug("Adding node, bad class");
 debugint(myDevice.addNode("bad_class"));
 debug("How many nodes do we have?");
 debugint(myDevice.countNodes());
 debug("Adding node, good class other");
 debugint(myDevice.addNode("other"));
 debug("How many nodes do we have?");
 debugint(myDevice.countNodes());
 debug("Adding node, good class thermostat");
 debugint(myDevice.addNode("thermostat"));
 debug("How many nodes do we have?");
 debugint(myDevice.countNodes());
 debug("Adding node, good class kettle");
 debugint(myDevice.addNode("kettle"));
 debug("How many nodes do we have?");
 debugint(myDevice.countNodes());

 debug("Checking class of node 1. Should be other");
 AthomNode* myNode = myDevice.getNode(1);
 debug(myNode->getClass());
 debug("Checking class of node 2. Should be thermostat");
 myNode = myDevice.getNode(2);
 debug(myNode->getClass());
 debug("Checking class of node 3. Should be kettle");
 myNode = myDevice.getNode(3);
 debug(myNode->getClass());
 debug("Checking class of node 4. Should be nullptr");
 myNode = myDevice.getNode(4);
 if (myNode == nullptr) {
   debug("nullptr returned OK.");
 }


}


void loop() {
 // Get some data
 // Trigger the webhook
 // Wait 60 seconds
 delay(60000);
}
