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
 debug("via Device");
 debug(myDevice.getClass(1));
 debug("Checking class of node 2. Should be thermostat");
 myNode = myDevice.getNode(2);
 debug(myNode->getClass());
 debug("via Device");
 debug(myDevice.getClass(2));
 debug("Checking class of node 3. Should be kettle");
 myNode = myDevice.getNode(3);
 debug(myNode->getClass());
 debug("via Device");
 debug(myDevice.getClass(3));
 debug("Checking class of node 4. Should be nullptr");
 myNode = myDevice.getNode(4);
 if (myNode == nullptr) {
   debug("nullptr returned OK.");
 }
 debug("via Device - should be not found");
 debug(myDevice.getClass(4));
 debug("---------------------------------------------");
 debug("CAPABILITIES");
 debug("Adding capability, bad node, good class");
 debugint(myDevice.addCapability(7,"onoff"));
 debug("Adding capability, good node, bad class");
 debugint(myDevice.addCapability(3,"rubbish"));
 debug("Adding capability, good node, good class");
 debugint(myDevice.addCapability(3,"onoff"));
 debug("Adding another capability, good node, good class");
 debugint(myDevice.addCapability(3,"dim"));
 debug("Adding another capability, good node, good class");
 debugint(myDevice.addCapability(3,"light_hue"));
 debug("Adding same capability, different node, good class");
 debugint(myDevice.addCapability(1,"light_hue"));
 debug("Adding same capability, different node, good class");
 debugint(myDevice.addCapability(2,"thermostat_mode"));
 debug("Checking first class of nodes 1-3.");
 debug(myDevice.getCapability(1,1));
 debug(myDevice.getCapability(2,1));
 debug(myDevice.getCapability(3,1));
 debug("Checking first class of non existant node.");
 debug(myDevice.getCapability(4,1));
 debug("Checking non existant class of first node.");
 debug(myDevice.getCapability(1,56));
 debug("Checking non existant class of last node.");
 debug(myDevice.getCapability(3,56));
 debug("Checking non existant class of non existant node.");
 debug(myDevice.getCapability(34,56));
 
}


void loop() {
 // Get some data
 // Trigger the webhook
 // Wait 60 seconds
 debug("Mem check");
 delay(60000);
}
