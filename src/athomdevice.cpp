/*
 * Project AthomIntDev
 * Description: Oject definitions for Athom Homey devices on Particle
 * Author: SpecialCircumstances
 * Date:
 * Some notes
 * We use String type outwardly, but char arrays
 * internally.
 */

 #include "athomdevice.h"

// Library Global Objects
// We will often need these so lets reuse these ones throughout
// JSON lib included in athomdevice.h
JsonParserStatic<622, 20> jsonparser;
JsonWriterStatic<622> jsonwriter;

// Helper Functions
  static void debug(const String message) {
    if (!ATHOMDEFS_DEBUG) {return;};
    Serial.print(message);
    uint32_t freemem = System.freeMemory();
    Serial.print("\t\t\t Free Mem: ");
    Serial.println(freemem);
  }

  static void debugint(const int message) {
    if (!ATHOMDEFS_DEBUG) {return;};
    String num = String(message);
    debug(String("INT: " + num));
  }

   static bool athomIsClass(String myClass) {
     // Check supplied String matches a valid Homey Class
     // convert to
     char myClassChar[MAX_CHARS_CLASS];
     myClass.toCharArray(myClassChar,MAX_CHARS_CLASS);
     // athomClasses provided by athomdefs.h
     for (int i = 0; i < arraySize(athomClasses); i++) {
       if (strncmp(athomClasses[i],myClassChar,MAX_CHARS_CLASS)==0) {
         return true;
       }
     }
     return false;
   }


   static bool athomIsCapability(String myCap) {
     // Check supplied String matches a valid Homey Class
     // convert to
     char myCapChar[MAX_CHARS_CAPABILITY];
     myCap.toCharArray(myCapChar,MAX_CHARS_CAPABILITY);
     // athomClasses provided by athomdefs.h
     for (int i = 0; i < arraySize(athomCapabilities); i++) {
       if (strncmp(athomCapabilities[i],myCapChar,MAX_CHARS_CAPABILITY)==0) {
         return true;
       }
     }
     return false;
   }


 // Class Definitions

 // ******************************************************

 AthomCapability::AthomCapability(const String myCap) {  // Class Constructor
   _isSetable = false;
   _isGetable = false;
   _lastSet = 0;
   _lastGet = 0;
   _setCallback = nullptr;
   _getCallback = nullptr;
   _prevCapability = nullptr;
   _nextCapability = nullptr;
   _myNode = nullptr;
   setName(myCap);
 }

 int AthomCapability::setName(const String myCap) {
   // Sometimes this might be validated
   // Sometimes not...
   if (athomIsCapability(myCap)) {
     // Store to private variable
     myCap.toCharArray(_myName,MAX_CHARS_CAPABILITY);
     return 0;
   }
   return -1;
 }

 String AthomCapability::getName() {
   // convert to String and return
   String rStr = _myName;
   return rStr;
 }


 AthomCapability* AthomCapability::getNext() {
   return _nextCapability;
 }

 AthomCapability* AthomCapability::getPrev() {
   return _prevCapability;
 }

 int AthomCapability::setNext(AthomCapability* nextCap) {
   // TODO checking type
   _nextCapability = nextCap;
   return 0;
 }

 int AthomCapability::setPrev(AthomCapability* prevCap) {
   // TODO checking type
   _prevCapability = prevCap;
   return 0;
 }

 int AthomCapability::setNode(AthomNode* myNode) {
   // TODO checking type
   _myNode = myNode;
   return 0;
 }

 void AthomCapability::setSetCallback( int (*yourFunc)(int) ) {
   _setCallback = yourFunc;
   _isSetable = true;
 }

 int AthomCapability::doSet(const int myValue) {
   if (_isSetable) {
     return _setCallback(myValue);
   } else {
     return -1;
   }
 }

 bool AthomCapability::isGetable() {
   return _isGetable;
 }

 void AthomCapability::setGetCallback( int (*yourFunc)(int) ) {
   _getCallback = yourFunc;
   _isGetable = true;
 }

 int AthomCapability::doGet(const int myValue) {
   if (_isGetable) {
     return _getCallback(myValue);
   } else {
     return -1;
   }
 }

 bool AthomCapability::isSetable() {
   return _isSetable;
 }



 // ******************************************************

 AthomAction::AthomAction(const String myName, int (*yourFunc)(int)) {
   // Pass a name and a function to call
   _isUsable = false;
   setName(myName);
   setCallback(yourFunc);
 }


 int AthomAction::setName(const String actionName) {
   // TODO: Check name is unique
   _actionName = actionName;
   return 0;
 }


 String AthomAction::getName() {
   // Is currently a String
   return _actionName;
 }


 void AthomAction::setCallback( int (*yourFunc)(int) ) {
   _actionCallback = yourFunc;
 }

 int AthomAction::doAction(const int myValue) {
   return _actionCallback(myValue);
 }

// ***************************************************

 AthomTrigger::AthomTrigger() {       // Class Constructor

 }

// ***************************************************

 AthomNode::AthomNode(const String myClass){              // Class Constructor
  // Init private vars
   _myName = "Undefined";
   //_myClass =
   _firstCapability = nullptr;
   _capabilityCount = 0;
   _firstAction = nullptr;
   _prevNode = nullptr;
   _nextNode = nullptr;
   _myDevice = nullptr;
   setClass(myClass);
 }

 int AthomNode::setClass(const String nodeClass) {
   // Sometimes this might be validated
   // Sometimes not...
   if (athomIsClass(nodeClass)) {
     // Store to private variable
     nodeClass.toCharArray(_myClass,MAX_CHARS_CLASS);
     return 0;
   }
   return -1;
 }

 String AthomNode::getClass() {
   // convert to String and return
   String rStr = _myClass;
   return rStr;
 }

 AthomNode* AthomNode::getNext() {
   return _nextNode;
 }

 AthomNode* AthomNode::getPrev() {
   return _prevNode;
 }

 int AthomNode::setNext(AthomNode* nextNode) {
   // TODO checking type
   _nextNode = nextNode;
   return 0;
 }

 int AthomNode::setPrev(AthomNode* prevNode) {
   // TODO checking type
   _prevNode = prevNode;
   return 0;
 }

 int AthomNode::setDevice(AthomDevice* myDevice) {
   // TODO checking type
   _myDevice = myDevice;
   return 0;
 }

 int AthomNode::addCapability(const String myCap){
   int capabilityCount = 0;
   if (findCapabilityByName(myCap)!=0) {
     return -1; // duplicate or does not exist
   } // duplicate

   AthomCapability* newCap = new AthomCapability(myCap);
   // TODO: check for malloc fail??
   if (_firstCapability==nullptr) {
     _firstCapability=newCap;
     capabilityCount=1;
   } else {
     AthomCapability* next = _firstCapability;
     AthomCapability* last = _firstCapability;
     while (next!=nullptr) {
       last = next;
       next = last->getNext();
       capabilityCount++;
     }
     newCap->setPrev(last);
     last->setNext(newCap);
     capabilityCount++;
   }
   // Tell the capability who it's parent is
   newCap->setNode(this);
   // Store count of nodes.
   _capabilityCount = capabilityCount;
   // Update Cloud variable
   // parent - _updateHomeyCaps();

   return capabilityCount;
 }


 AthomCapability* AthomNode::getCapability(const int capNumber) {
   // Returns a pointer to the requested node
   // Check bounds
   if ((0 < capNumber) && (capNumber <= _capabilityCount)) {
     // Should note at this time we don't support
     // dynamic changes to capability list. No delete.
     // MESH might change that. TODO
     if (capNumber==1) {
       return _firstCapability;
     } else {
       int myCount = 1;
       AthomCapability* next = _firstCapability;
       AthomCapability* last = _firstCapability;
       while (next!=nullptr) {
         if (myCount==capNumber) {return next;}
         myCount++;
         last = next;
         next = last->getNext();
      }
    }
   }
   return nullptr;
 }

 // How many capabilities do we have
 int AthomNode::countCapabilities() {
   return _capabilityCount;
 }


 // Check if a Capability exists, return 0 for no, or capaility ID
 int AthomNode::findCapabilityByName(const String myCap) {
   int capabilityCount = 0;
   if (athomIsCapability(myCap)) {
     if (_firstCapability == nullptr) {
       // No capabilities
       return 0;
     } else {
       // Step through capabilities
       AthomCapability* next = _firstCapability;
       AthomCapability* last = _firstCapability;
       while (next!=nullptr) {
         capabilityCount++;
         //debug(next->getName());
         if (next->getName().compareTo(myCap)==0) {
           return capabilityCount;
         }
         last = next;
         next = last->getNext();
       }
       return 0; // Not found
     }
   } else {
     return -1; // No such capability
   }
 }


// **********************************************


 AthomDevice::AthomDevice() {
   // init private variables
   strncpy ( _myName, "UNDEFINED", MAX_CHARS_NAME );
   _firstNode = nullptr;
   _nodeCount = 0;
   // Init private Cloud variables
   _myHomeyAPI = 0;   //false
   _myHomeyClass = "";
   _myHomeyCaps = "";
   _myHomeyConfs = "";
   _myHomeyActs = "";
   _myHomeySend = "";
   _lastReport = millis();
   int errors = 0;  // TODO do something with this
   if (Particle.variable("HomeyAPI", _myHomeyAPI)==false)
   {
       debug("Failed to register HomeyAPI.");
       errors++;
   }
   if (Particle.variable("HomeyClass", _myHomeyClass)==false)
   {
       debug("Failed to register HomeyClass.");
       errors++;
   }
   if (Particle.variable("HomeyCaps", _myHomeyCaps)==false)
   {
       debug("Failed to register HomeyCaps.");
       errors++;
   }
   if (Particle.variable("HomeyConfs", _myHomeyConfs)==false)
   {
       debug("Failed to register HomeyConfs.");
       errors++;
   }
   if (Particle.variable("HomeyActs", _myHomeyActs)==false)
   {
       debug("Failed to register HomeyActs.");
       errors++;
   }
   // Init Particle Functions
   if (Particle.function("HomeyConf", &AthomDevice::_myHomeyConf, this)==false)
   {
       Serial.println("Failed to register function HomeyConf.");
       errors++;
   }
   if (Particle.function("HomeyGet", &AthomDevice::_myHomeyGet, this)==false)
   {
       Serial.println("Failed to register function HomeyGet.");
       errors++;
   }
   if (Particle.function("HomeySet", &AthomDevice::_myHomeySet, this)==false)
   {
       Serial.println("Failed to register function HomeySet.");
       errors++;
   }
   if (Particle.function("HomeyAct", &AthomDevice::_myHomeyAct, this)==false)
   {
       Serial.println("Failed to register function HomeyAct.");
       errors++;
   }
 }            // Class Constructor


 int AthomDevice::setName(const String myName) {
   myName.toCharArray(_myName,MAX_CHARS_NAME);
 }


 String AthomDevice::getName() {
   return _myName;
 }


int AthomDevice::addNode(const String nodeClass) {
  // Takes a String which much be a match for a Homey Class
  // Returns node ID, or zero if fails.
  int nodeCount = 0;
  if (athomIsClass(nodeClass)) {
    // create an oject in Heap, and get PTR
    AthomNode* newNode = new AthomNode(nodeClass);
    // TODO: check for malloc fail??
    if (_firstNode==nullptr) {
      _firstNode=newNode;
      nodeCount=1;
    } else {
      AthomNode* next = _firstNode;
      AthomNode* last = _firstNode;
      while (next!=nullptr) {
        last = next;
        next = last->getNext();
        nodeCount++;
      }
      newNode->setPrev(last);
      last->setNext(newNode);
      nodeCount++;
    }
    // Tell the node who it's parent is
    newNode->setDevice(this);
    // Store count of nodes.
    _nodeCount = nodeCount;
    // Update Cloud variable
    _updateHomeyClass();
  }
  return nodeCount;
}

// How many nodes do we have
int AthomDevice::countNodes() {
  return _nodeCount;
}


// Give me a pointer to a particular node
// Node level, not friendly, use device level methods
AthomNode* AthomDevice::getNode(const int nodeNumber) {
    // Returns a pointer to the requested node
    // Check bounds
    if ((0 < nodeNumber) && (nodeNumber <= _nodeCount)) {
      // Should note at this time we don't support
      // dynamic changes to node list. No delete.
      // MESH might change that. TODO
      if (nodeNumber==1) {
        return _firstNode;
      } else {
        int myCount = 1;
        AthomNode* next = _firstNode;
        AthomNode* last = _firstNode;
        while (next!=nullptr) {
          if (myCount==nodeNumber) {return next;}
          myCount++;
          last = next;
          next = last->getNext();
       }
     }
    }
    return nullptr;
}

// Get me the class of a particular nodeID
// Device level method. Better to use that trying to access
// the node directly via a pointer
String AthomDevice::getClass(const int nodeId) {
  AthomNode* myNode = getNode(nodeId);
  if (myNode != nullptr) {
    return myNode->getClass();
  } else {
    return "node_not_found";
  }
}

// Add a capability to a node
// Returns resulting count of capabilities for that node
int AthomDevice::addCapability(const int nodeId, const String myCapability) {
  AthomNode* myNode = getNode(nodeId);
  if (myNode != nullptr) {
    int result =  myNode->addCapability(myCapability);
    _updateHomeyCaps();
    return result;
  } else {
    return -1;
  }
}

AthomCapability* AthomDevice::getCapability(const int nodeNumber, const int capNumber) {
  AthomNode* myNode = getNode(nodeNumber);
  if (myNode != nullptr) {
    AthomCapability* myCap = myNode->getCapability(capNumber);
    if (myCap != nullptr) {
      return myCap;
    } else {
      return nullptr;
    }
  } else {
    return nullptr;
  }
}


String AthomDevice::getCapabilityName(const int nodeNumber, const int capNumber) {
  AthomCapability* myCap = getCapability(nodeNumber, capNumber);
  if (myCap != nullptr) {
    return myCap->getName();
  } else {
    return "capability_not_found";
  }
}


int AthomDevice::countCapabilities(const int nodeNumber) {
  AthomNode* myNode = getNode(nodeNumber);
  if (myNode != nullptr) {
      return myNode->countCapabilities();
  } else {
    return 0;
  }
}


int AthomDevice::findCapabilityByName(const int nodeNumber, const String myCap) {
  // returns the capability id of a particular capability
  //  0 is capability not found
  // -1 is bad capability
  // -2 is node not found
  AthomNode* myNode = getNode(nodeNumber);
  if (myNode != nullptr) {
      return myNode->findCapabilityByName(myCap);
  } else {
    //debug("Node not found" + String(nodeNumber));
    return -2;
  }
}

void AthomDevice::setCapabilityGetCallback(const int nodeId, const String myCapability, int (*yourFunc)(int) ) {
  // TODO Add return values
  int myCapId = findCapabilityByName(nodeId, myCapability);
  if (myCapId > 0) {
    AthomCapability* myCap = getCapability(nodeId, myCapId);
    myCap->setGetCallback(yourFunc);
  }
}

void AthomDevice::setCapabilitySetCallback(const int nodeId, const String myCapability, int (*yourFunc)(int) ) {
  // TODO Add return values
  int myCapId = findCapabilityByName(nodeId, myCapability);
  if (myCapId > 0) {
    AthomCapability* myCap = getCapability(nodeId, myCapId);
    myCap->setSetCallback(yourFunc);
  }
}



int AthomDevice::_updateHomeyClass() {
  // This to update Cloud Vars
  // Need to create a String containing JSON
  // Write array to cloud variable as JSON
  // jsonwriter is scoped to support it's autobject
  jsonwriter.init();   // CLear buffer
  {
    JsonWriterAutoObject obj(&jsonwriter);
    for (int i = 1; i <= _nodeCount; i++) {

      jsonwriter.insertKeyValue(String(i), getClass(i));
    }
  }
  // assign to myHomeyClass
  _myHomeyClass = jsonwriter.getBuffer();
}


int AthomDevice::_updateHomeyCaps() {
  // This to update Cloud Vars
  // Need to create a String containing JSON
  // Write array to cloud variable as JSON
  // jsonwriter is scoped to support it's autobject
  jsonwriter.init();   // CLear buffer
  {
    JsonWriterAutoObject obj(&jsonwriter);
    for (int i = 1; i <= _nodeCount; i++) {
      //String nodeCaps = "";
      jsonwriter.insertKeyArray(String(i));
      int numCaps = countCapabilities(i);
      for (int c = 1; c <= numCaps; c++) {
        //nodeCaps+= getCapability(i,c);
        //if (c < numCaps) {nodeCaps+= ",";}
        jsonwriter.insertArrayValue(getCapabilityName(i,c));
      }
      jsonwriter.finishObjectOrArray();
    }
  }
  // assign to myHomeyCaps
  _myHomeyCaps= jsonwriter.getBuffer();
}


int AthomDevice::_myHomeyConf(const String message) {
   Serial.println("myHomeyConf Called");
   return 1;
}


int AthomDevice::_myHomeyGet(const String message) {
  // This function decodes the received message
  // message is limited to 63 characters
  // The request will be in JSON format, with node and capability
  // identified. Note that the GET doesn't return the data,
  // rather it triggers a report to be sent.
  //
  //    {
  //      "n": "nodeid",
  //      "c": "capability_name"
  //    }
  //0         1         2         3         4         5         6
  // 123456789012345678901234567890123456789012345678901234567890123
  // {"n": "nodeid","c": "capability_name"}
  debug("myHomeyGet Called");
  int nodeId = 0;
  String myCapability = "";
  // We need to
  // Decode JSON to node and capability
  jsonparser.clear();
  jsonparser.addString(message);

  if (jsonparser.parse()) {
    // Looks valid (we received all parts)
    jsonparser.getOuterValueByKey("n", myCapability); //reuse String
    nodeId = myCapability.toInt();
    jsonparser.getOuterValueByKey("c", myCapability);
  } else {
    // bad data
    debug("WARNING: Bad Data");
    return -1;
  }
  // Check node and capability exist (return -1 if not)
  int capId = findCapabilityByName(nodeId, myCapability);
  if (capId < 1) {
    debug("WARNING: Capability Not Found: " + myCapability);
    return capId;
  }
  // Get the capability
  AthomCapability* myCap = getCapability(nodeId,capId);
  // Check the capability isGetable
  if (!myCap->isGetable()) {
    debug("WARNING: Capability Not Gettable");
    return -1;
  }
  // Call the function and check it's result (pass that back)
  int result = myCap->doGet(1);
  debugint(result);
  // TODO: we need to wrap in a timeout, if poss
  // Trigger a report.
  _sendReport(nodeId, myCapability, result);
  return 1;
}

int AthomDevice::_myHomeySet(const String message) {
   Serial.println("myHomeySet Called");
   return 1;
}


int AthomDevice::_myHomeyAct(const String message) {
   Serial.println("myHomeyAct Called");
   return 1;
}


int AthomDevice::_myHomeyRecv(const String message) {
   Serial.println("myHomeySend Called");
   return 1;
}

void AthomDevice::_sendReport(const int nodeId, const String myCap, const int value) {
  // OK now, to be clear, this is kind of blocking
  // Particle Cloud rate limits events to 1 Hz
  // So, to keep things under control, we wait
  // here, until at least a second since the last report
  // Build the data set first
  // As private function, input is assumed good
  // TODO is this a good place to shift decimals?
  JsonWriterStatic<250> reportJw;
  //debug("REPORT capability is: " + myCap);
  //debug("REPORT node is: " + String(nodeId));
  //debug("REPORT value is: " + String(value));
  //jsonwriter.init();   // CLear buffer
  //reportJw.startArray();  // neater
  reportJw.startObject();
  reportJw.insertKeyObject(String(nodeId));
  reportJw.insertKeyValue(myCap, String(value));
  reportJw.finishObjectOrArray();
  reportJw.finishObjectOrArray();
  //reportJw.finishObjectOrArray();
  // assign to myHomeyCaps
  //debug("Checking frequency...");
  while ( millis()-_lastReport < 1000) {
    delay(100); // lower load than process() due to while
    //debug("WAIT!");
  }
  //debug("Done waiting.");
  //String data = String(reportJw.getBuffer());
  //debug(reportJw.getBuffer());
  Particle.publish("Homey", reportJw.getBuffer(), PRIVATE);
  _lastReport = millis();
}
