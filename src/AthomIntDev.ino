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

SerialLogHandler logHandler;

// Create an Athom Device
AthomDevice myDevice;

int myTestInt = 0;
float myTestFloat = 0.0;
bool myTestBool = false;

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

int TestCallBackInt() {
  debug("Test Callback Int Called OK - returning " + String(myTestInt));
  return myTestInt;
}

float TestCallBackFloat() {
  debug("Test Callback Float Called OK - returning " + String(myTestFloat));
  return myTestFloat;
}

 bool TestCallBackBool() {
  debug("Test Callback Bool Called OK - returning " + String(myTestBool));
  return myTestBool;
}

int TestSetCallBackInt(int myVal) {
  debug("Int Test Set Callback Called OK - got: " + String(myVal));
  myTestInt = myVal;
  return myTestInt;
}

float TestSetCallBackFloat(float myVal) {
  debug("Float Test Set Callback Called OK - got: " + String(myVal));
  myTestFloat = myVal;
  return myTestFloat;
}

bool TestSetCallBackBool(bool myVal) {
  debug("Bool Test Set Callback Called OK - returning same bool" + String(myVal));
  myTestBool = myVal;
  return myTestBool;
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
 debug("Adding same capability, same node, same class");
 debugint(myDevice.addCapability(2,"thermostat_mode"));
 debug("Checking first class of nodes 1-3.");
 debug(myDevice.getCapabilityName(1,1));
 debug(myDevice.getCapabilityName(2,1));
 debug(myDevice.getCapabilityName(3,1));
 debug("Checking first class of non existant node.");
 debug(myDevice.getCapabilityName(4,1));
 debug("Checking non existant class of first node.");
 debug(myDevice.getCapabilityName(1,56));
 debug("Checking non existant class of last node.");
 debug(myDevice.getCapabilityName(3,56));
 debug("Checking non existant class of non existant node.");
 debug(myDevice.getCapabilityName(34,56));
 debug("Find capability by node and name. 3. dim");
 debugint(myDevice.findCapabilityByName(3,"dim"));
 debug("Int Callbacks");
 debug("Set Callback Method for Node 2, thermostat_mode, Get");
 myDevice.setCapabilityGetCallback(2,"thermostat_mode",&TestCallBackInt);
 debug("Set Callback Method for Node 2, thermostat_mode, Set");
 myDevice.setCapabilitySetCallback(2,"thermostat_mode",&TestSetCallBackInt);
 debug("Float Callbacks");
 debug("Set Callback Method for Node 3, Dim, Get");
 myDevice.setCapabilityGetCallback(3,"dim",&TestCallBackFloat);
 debug("Set Callback Method for Node 3, Dim, Set");
 myDevice.setCapabilitySetCallback(3,"dim",&TestSetCallBackFloat);
 debug("Bool callbacks");
 debug("Set Callback Method for Node 3, onoff, Get");
 myDevice.setCapabilityGetCallback(3,"onoff",&TestCallBackBool);
 debug("Set Callback Method for Node 3, onoff, Set");
 myDevice.setCapabilitySetCallback(3,"onoff",&TestSetCallBackBool);

 debug("-----------------------------------------");
 debug("Test add Config Item: polling_int");
 debugint(myDevice.addConfigItem("polling_int"));
 debug("How many config items do we have?");
 debugint(myDevice.countConfigItems());
 debug("Test add DUPLICATE Config Item: polling_int");
 debugint(myDevice.addConfigItem("polling_int"));
 debug("How many config items do we have?");
 debugint(myDevice.countConfigItems());
 debug("Get ConfigItem by Number (1)");
 debug(myDevice.getConfigItem(1)->getName());
 debug("Get ConfigItem by Name polling_int should be 1");
 debugint(myDevice.findConfigItemByName("polling_int"));
 debug("MULTIPLES");
 debug("Test add Config Item: adjust01 and adjust02");
 debugint(myDevice.addConfigItem("adjust01"));
 debugint(myDevice.addConfigItem("adjust02"));
 debug("How many config items do we have?");
 debugint(myDevice.countConfigItems());
 debug("Test add DUPLICATE Config Item: adjust01");
 debugint(myDevice.addConfigItem("adjust01"));
 debug("Test add DUPLICATE Config Item: adjust02");
 debugint(myDevice.addConfigItem("adjust02"));
 debug("How many config items do we have?");
 debugint(myDevice.countConfigItems());
 debug("Get ConfigItem by Number (3)");
 debug(myDevice.getConfigItem(3)->getName());
 debug("Get ConfigItem by Name adjust02 should be 3");
 debugint(myDevice.findConfigItemByName("adjust02"));
 // Set GET callbacks
 debug("Setting GET callbacks int, float and bool for those three");
 myDevice.setConfigItemGetCallback("polling_int",&TestCallBackInt);
 myDevice.setConfigItemGetCallback("adjust01",&TestCallBackFloat);
 myDevice.setConfigItemGetCallback("adjust02",&TestCallBackBool);
 debug("Setting SET callbacks int, float and bool for those three");
 myDevice.setConfigItemSetCallback("polling_int",&TestSetCallBackInt);
 myDevice.setConfigItemSetCallback("adjust01",&TestSetCallBackFloat);
 myDevice.setConfigItemSetCallback("adjust02",&TestSetCallBackBool);

}


void loop() {
 // Get some data
 // Trigger the webhook
 // Wait 60 seconds
 debug("Mem check");
 myDevice.sendReport();
 delay(60000);
}
