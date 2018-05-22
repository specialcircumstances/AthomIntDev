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

// Global constants
//const int HOMEYVARBYTELIMIT = 622;

// Cloud variables we will expose
int myHomeyAPI = 1; // We use as boolean
String myHomeyClass;
String myHomeyCaps;
String myHomeyConfs;
String myHomeyActs;

AthomDevice myDevice();

// Cloud Functions we will expose
int myHomeyConf(String newSettings);
int myHomeyGet(String newSettings);
int myHomeySet(String newSettings);
int myHomeyAct(String newSettings);

// functions we don't expose
int registerHomeyVarFunc();
int initHomeyNodes(int numberOfNodes);
int setHomeyClass(int nodeID, String);



// Global Objects
JsonParserStatic<622, 20> jsonparser; // we only need one
JsonWriterStatic<622> jsonwriter;

int homeyNodes = 0; // Number of Homey Nodes/Devices we provide
                    // Useful for multiple instances of same Capability
                    // Set during setup with setHomeyNodes, read in HomeyClass
                    // Numbered 1,2,3,4 etc..
char nodeClassArray[MAX_NUMBER_NODES + 1][32]; // NOTE arrays are zero numbered


void oldsetup() {
 Serial.begin(9600);
 // Register the cloud variables
 Serial.println("Registering Homey variables and functions");
 if (registerHomeyVarFunc() != 0) {
   Serial.println("Registering Homey variables and functions failed.");
 } else {
   Serial.println("Registering Homey variables and functions succeeded.");
 }
 Serial.println("Done Setup.");
 initHomeyNodes(2);  // How many nodes are you defining
 setHomeyClass(1, "other");       // Set node 1 Homey Class
 setHomeyClass(2, "thermostat");  // Set node 2 Homey Class
}


void oldloop() {
 // Get some data
 String data = String(64);
 data = "HELLO FROM PHOTON";
 // Trigger the webhook
 Particle.publish("Homey", data, PRIVATE);
 Serial.println("Event published.");
 // Wait 60 seconds
 delay(60000);
}


int registerHomeyVarFunc() {
  int errors = 0;
  // Register Homey Variables
  if (Particle.variable("HomeyAPI", myHomeyAPI)==false)
  {
      Serial.println("Failed to register HomeyAPI.");
      errors++;
  }
  if (Particle.variable("HomeyClass", myHomeyClass)==false)
  {
      Serial.println("Failed to register HomeyClass.");
      errors++;
  }
  if (Particle.variable("HomeyCaps", myHomeyCaps)==false)
  {
      Serial.println("Failed to register HomeyCaps.");
      errors++;
  }
  if (Particle.variable("HomeyConfs", myHomeyConfs)==false)
  {
      Serial.println("Failed to register HomeyConfs.");
      errors++;
  }
  if (Particle.variable("HomeyActs", myHomeyActs)==false)
  {
      Serial.println("Failed to register HomeyActs.");
      errors++;
  }
  // register Homey Functions
  if (Particle.function("HomeyConf", myHomeyConf)==false)
  {
      Serial.println("Failed to register function HomeyConf.");
      errors++;
  }
  if (Particle.function("HomeyGet", myHomeyGet)==false)
  {
      Serial.println("Failed to register function HomeyGet.");
      errors++;
  }
  if (Particle.function("HomeySet", myHomeySet)==false)
  {
      Serial.println("Failed to register function HomeySet.");
      errors++;
  }
  if (Particle.function("HomeyAct", myHomeyAct)==false)
  {
      Serial.println("Failed to register function HomeyAct.");
      errors++;
  }
  return errors;
}


int myHomeyConf(String newSettings) {
   Serial.println("myHomeyConf Called");
   return 1;
}


int myHomeyGet(String newSettings) {
   Serial.println("myHomeyGet Called");
   return 1;
}

int myHomeySet(String newSettings) {
   Serial.println("myHomeySet Called");
   return 1;
}


int myHomeyAct(String newSettings) {
   Serial.println("myHomeyAct Called");
   return 1;
}


// Set the number of Homey Nodes we will provide
int initHomeyNodes(int numberOfNodes) {
  // Initialise, clear the array
  char mynull[32] = "null";
  for (int i = 0; i < arraySize(nodeClassArray); i++) {
    strcpy(nodeClassArray[i],mynull);
  }
  // Check within bounds and set global variable
  if (numberOfNodes > 0 && numberOfNodes <= MAX_NUMBER_NODES) {
    // Store number
    homeyNodes = numberOfNodes;
    return 0;
  } else {
    homeyNodes = 0;
    return 1;
  }
}



// Set the HomeyClass variable
// Must be a recognised, single, Homey Class (see athomdefs.h)
int setHomeyClass(int nodeID, String myClass){
  // Check nodeID within bounds
  if (!(nodeID > 0 && nodeID <= homeyNodes)) {
    return 1;
  }
  int notfound = 1;
  char myClassChar[MAX_CHARS_CLASS];
  myClass.toCharArray(myClassChar,MAX_CHARS_CLASS);
  for (int i = 0; i < arraySize(athomClasses); i++) {
    if (strncmp(athomClasses[i],myClassChar,MAX_CHARS_CLASS)==0) {
      notfound = 0;
      Serial.println("Homey Class found.");
      // Set the array
      strcpy(nodeClassArray[nodeID],myClassChar);
    }
  }
  if (notfound == 1) {
    Serial.println("Homey Class not found.");
  }
  // Write array to cloud variable as JSON
  {
    jsonwriter.init();
    JsonWriterAutoObject obj(&jsonwriter);
    for (int i = 1; i <= homeyNodes; i++) {
      jsonwriter.insertKeyValue(String(i), nodeClassArray[i]);
    }
  }
  // assign to myHomeyClass
  myHomeyClass = jsonwriter.getBuffer();
  // Clear up jsonwriter
  jsonwriter.init();
  return notfound;
}

// Clear the HomeyCaps variable
// No idea when this would be needed but add it anyway
// Must be a recognised, single, Homey Capability (see athomdefs.h)
int addHomeyCapability(String myCapability) {
  int notfound = 1;
  for (int i = 0; i < arraySize(athomCapabilities); i++) {
    if (athomCapabilities[i] == myCapability) {
      myHomeyCaps = myCapability;
      notfound = 0;
      Serial.println("Homey Class found.");
    }
  }
  if (notfound == 1) {
    Serial.println("Homey Class not found.");
  }
  return notfound;
}
