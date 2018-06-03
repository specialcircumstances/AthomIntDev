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
//JsonParserStatic<622, 20> jsonparser;
//JsonWriterStatic<622> jsonwriter;

// Does not appear to clear/init properly

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

   static bool athomConfigIsValid(String myConf) {
     if (myConf.length() < MAX_CHARS_NAME ) {
       return true;
     } else {
       return false;
     }
   }

 // Class Definitions

 // ******************************************************

 AthomBaseObject::AthomBaseObject() {
   _prevObject = nullptr;
   _nextObject = nullptr;
   _myParent = nullptr;
   setName("UNDEFINED");
 }


 AthomBaseObject::AthomBaseObject(const String myCap) {
   _prevObject = nullptr;
   _nextObject = nullptr;
   _myParent = nullptr;
   setName(myCap);
 }


 int AthomBaseObject::setName(const String myName) {
   myName.toCharArray(_myName,MAX_CHARS_NAME);
   return 0;
 }

 String AthomBaseObject::getName() {
   // convert to String and return
   String rStr = _myName;
   return rStr;
 }


 AthomBaseObject* AthomBaseObject::getPrev() {
   return _prevObject;
 }


 AthomBaseObject* AthomBaseObject::getNext() {
   return _nextObject;
 }


 int AthomBaseObject::setNext(AthomBaseObject* nextObject) {
   // TODO checking type
   _nextObject = nextObject;
   return 0;
 }


 int AthomBaseObject::setPrev(AthomBaseObject* prevObject) {
   // TODO checking type
   _prevObject = prevObject;
   return 0;
 }


 int AthomBaseObject::setParent(AthomBaseObject* myParent) {
   _myParent = myParent;
   return 0;
 }


 // ******************************************************
 AthomGetSetObject::AthomGetSetObject()
  : AthomBaseObject()
   {
     _isSetable = false;
     _isGetable = false;
     _isInt = false;
     _isFloat = false;
     _isBool = false;
     _setCallbacki = nullptr;
     _setCallbackf = nullptr;
     _setCallbackb = nullptr;
     _getCallbacki = nullptr;
     _getCallbackf = nullptr;
     _getCallbackb = nullptr;
     //: AthomBaseObject(myName);
   }


 AthomGetSetObject::AthomGetSetObject(const String myName)
  : AthomBaseObject(myName)
 {
   _isSetable = false;
   _isGetable = false;
   _isInt = false;
   _isFloat = false;
   _isBool = false;
   _setCallbacki = nullptr;
   _setCallbackf = nullptr;
   _setCallbackb = nullptr;
   _getCallbacki = nullptr;
   _getCallbackf = nullptr;
   _getCallbackb = nullptr;
   //: AthomBaseObject(myName);
 }


 int AthomGetSetObject::setSetCallback( int (*yourFunc)(int) ) {
   if (yourFunc==nullptr || (_isGetable && (_isFloat || _isBool))) {
     // Get and set type must match
     return -1;
   }
   _setCallbacki = yourFunc;
   _setCallbackf = nullptr;
   _setCallbackb = nullptr;
   _isInt = true;
   _isFloat = false;
   _isBool = false;
   _isSetable = true;
   return 0;
 }


 int AthomGetSetObject::setSetCallback( float (*yourFunc)(float) ) {
   if (yourFunc==nullptr || (_isGetable && (_isInt || _isBool))) {
     // Get and set type must match
     return -1;
   }
   _setCallbackf = yourFunc;
   _setCallbacki = nullptr;
   _setCallbackb = nullptr;
   _isInt = false;
   _isFloat = true;
   _isBool = false;
   _isSetable = true;
   return 0;
 }


 int AthomGetSetObject::setSetCallback( bool (*yourFunc)(bool) ) {
   if (yourFunc==nullptr || (_isGetable && (_isFloat || _isInt))) {
     // Get and set type must match
     return -1;
   }
   _setCallbackb = yourFunc;
   _setCallbackf = nullptr;
   _setCallbacki = nullptr;
   _isInt = false;
   _isFloat = false;
   _isBool = true;
   _isSetable = true;
   return 0;
 }


 int AthomGetSetObject::doSet(const int myValue) {
   if (_isInt && _isSetable && _setCallbacki!=nullptr) {
     return _setCallbacki(myValue);
   } else {
     return -1;
   }
 }


 float AthomGetSetObject::doSet(const float myValue) {
   if (_isFloat && _isSetable && _setCallbackf!=nullptr) {
     return _setCallbackf(myValue);
   } else {
     return -1;
   }
 }


 bool AthomGetSetObject::doSet(const bool myValue) {
   if (_isBool && _isSetable && _setCallbackb!=nullptr) {
     return _setCallbackb(myValue);
   } else {
     return false;
   }
 }


 bool AthomGetSetObject::isSetable() {
   return _isSetable;
 }


 int AthomGetSetObject::setGetCallback( int (*yourFunc)() ) {
   if ( yourFunc==nullptr || (_isSetable && (_isFloat || _isBool))) {
     // Get and set type must match
     return -1;
   }
   _getCallbacki = yourFunc;
   _getCallbackf = nullptr;
   _getCallbackb = nullptr;
   _isInt = true;
   _isFloat = false;
   _isBool = false;
   _isGetable = true;
   return 0;
 }


 int AthomGetSetObject::setGetCallback( float (*yourFunc)() ) {
   if ( yourFunc==nullptr || (_isSetable && (_isInt || _isBool))) {
     // Get and set type must match
     return -1;
   }
   _getCallbackf = yourFunc;
   _getCallbacki = nullptr;
   _getCallbackb = nullptr;
   _isInt = false;
   _isFloat = true;
   _isBool = false;
   _isGetable = true;
   return 0;
 }


 int AthomGetSetObject::setGetCallback( bool (*yourFunc)() ) {
   if ( yourFunc==nullptr || (_isSetable && (_isInt || _isFloat))) {
     // Get and set type must match
     return -1;
   }
   _getCallbackb = yourFunc;
   _getCallbacki = nullptr;
   _getCallbackf = nullptr;
   _isInt = false;
   _isFloat = false;
   _isBool = true;
   _isGetable = true;
   return 0;
 }


 int AthomGetSetObject::doGet(const int myValue) {
   // Value is ignored for Get, just overloading
   return doGetInt();
 }


 int AthomGetSetObject::doGetInt() {
    // Value is ignored for Get
    if (_isGetable && _isInt && _getCallbacki!=nullptr) {
      return _getCallbacki();
    } else {
      return -1;
    }
 }


 float AthomGetSetObject::doGet(const float myValue) {
    // Value is ignored for Get, just overloading
    return doGetFloat();
 }


 float AthomGetSetObject::doGetFloat() {
   if (_isGetable && _isFloat && _getCallbackf!=nullptr) {
     return _getCallbackf();
   } else {
     return -1.0;
   }
 }


 bool AthomGetSetObject::doGet(const bool myValue) {
   // Value is ignored for Get, just overloading
   return doGetBool();
 }


 bool AthomGetSetObject::doGetBool() {
   if (_isGetable && _isBool && _getCallbackb!=nullptr) {
     bool result = _getCallbackb();
     return result;
   } else {
     return NULL;
   }
 }


 bool AthomGetSetObject::isGetable() {
   return _isGetable;
 }


 bool AthomGetSetObject::isInt() {
   return _isInt;
 }


 bool AthomGetSetObject::isFloat() {
   return _isFloat;
 }


 bool AthomGetSetObject::isBool() {
   return _isBool;
 }


 String AthomGetSetObject::getType() {
   if (_isBool) {return "bool";}
   if (_isInt) {return "int";}
   if (_isFloat) {return "float";}
   return "null";
 }

 // ******************************************************


 AthomCapability::AthomCapability()
 {  // Class Constructor
  // Check capability is valid
  // Can't throw exceptions so mark bad object
  String validatedCap = "UNKNOWN";
  setName(validatedCap);
 }


 AthomCapability::AthomCapability(const String myCap)
  : AthomGetSetObject(myCap)
  {  // Class Constructor
   // Check capability is valid
   // Can't throw exceptions so mark bad object
   String validatedCap = myCap;
   if (!athomIsCapability(validatedCap)) {
     validatedCap = "UNKNOWN";
   }
   setName(validatedCap);
 }

 // ******************************************************


 AthomConfigItem::AthomConfigItem()
 {  // Class Constructor
  // Check capability is valid
  // Can't throw exceptions so mark bad object
  String validatedCap = "UNKNOWN";
  setName(validatedCap);
  _minInt = 0;
  _minFloat = 0.0;
  _maxInt = 65535;
  _maxFloat = 1.0;
 }


 AthomConfigItem::AthomConfigItem(const String myName)
  : AthomGetSetObject(myName)
  {  // Class Constructor
   // Check capability is valid
   // Can't throw exceptions so mark bad object
   String validatedName = myName;
   if (!athomConfigIsValid(validatedName)) {
     validatedName = "UNKNOWN";
   }
   setName(validatedName);
   _minInt = 0;
   _minFloat = 0.0;
   _maxInt = 65535;
   _maxFloat = 1.0;
 }

 // Additional GETS

 int AthomConfigItem::getMinInt() {
   return _minInt;
 }


 int AthomConfigItem::getMaxInt() {
   return _maxInt;
 }


 float AthomConfigItem::getMinFloat() {
   return _minFloat;
 }


 float AthomConfigItem::getMaxFloat() {
   return _maxFloat;
 }


 bool AthomConfigItem::getMinBool() {
   return false;
 }


 bool AthomConfigItem::getMaxBool() {
   return true;
 }


 String AthomConfigItem::getDesc() {
   return _desc;
 }

 // Additional Sets

 void AthomConfigItem::setMin(const int minValue) {
   if (_isInt) {
     _minInt = minValue;
   } else if (_isFloat) {
     _minFloat = float(minValue);
   }
 }


 void AthomConfigItem::setMin(const float minValue) {
   if (_isFloat) {
     _minFloat = minValue;
   } else if (_isInt) {
     _minInt = int(minValue);
   }
 }


 void AthomConfigItem::setMin(const bool minValue) {
   // Ignore the nonsense
 }


 void AthomConfigItem::setMax(const int maxValue) {
   if (_isInt) {
     _maxInt = maxValue;
   } else if (_isFloat) {
     _maxFloat = float(maxValue);
   }
 }


 void AthomConfigItem::setMax(const float maxValue) {
   if (_isFloat) {
     _maxFloat = maxValue;
   } else if (_isInt) {
     _maxInt = int(maxValue);
   }
 }


 void AthomConfigItem::setMax(const bool maxValue) {
   // Ignore the nonsense
 }


 void AthomConfigItem::setDesc(const String myDesc) {
   // TODO Prob should add some truncation/validation here
   _desc = myDesc;
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

 AthomNode::AthomNode(const String myClass)
  : AthomBaseObject("UNDEFINED")
 {              // Class Constructor
   _firstCapability = nullptr;
   _capabilityCount = 0;
   _firstAction = nullptr;
   String validatedClass = myClass;
   if (!athomIsClass(validatedClass)) {
     validatedClass = "UNKNOWN";
   }
   setName(validatedClass);
 }


 int AthomNode::setClass(const String nodeClass) {
   // Sometimes this might be validated
   // Sometimes not...
   if (athomIsClass(nodeClass)) {
     // Store to private variable
     setName(nodeClass);
     return 0;
   }
   return -1;
 }


 String AthomNode::getClass() {
   // potatoe potatoe
   return getName();
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
     AthomBaseObject* next = _firstCapability;
     AthomBaseObject* last = _firstCapability;
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
   newCap->setParent(this);
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
       AthomBaseObject* next = _firstCapability;
       while (next!=nullptr) {
         if (myCount==capNumber) {
           return static_cast<AthomCapability*>(next);
         }
         myCount++;
         next = next->getNext();
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
       AthomBaseObject* next = _firstCapability;
       while (next!=nullptr) {
         capabilityCount++;
         //debug(next->getName());
         if (static_cast<AthomCapability*>(next)->getName().compareTo(myCap)==0) {
           return capabilityCount;
         }
         next = next->getNext();
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
      AthomBaseObject* next = _firstNode;
      AthomBaseObject* last = _firstNode;
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
    //newNode->setParent(this);
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
        AthomBaseObject* next = _firstNode;
        while (next!=nullptr) {
          if (myCount==nodeNumber) {return static_cast<AthomNode*>(next);}
          myCount++;
          next = next->getNext();
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


// Templated overloads for bool, int and float
template <class FuncType>
void AthomDevice::setCapabilityGetCallback(const int nodeId, const String myCapability, FuncType (*yourFunc)() ) {
  // TODO Add return values
  int myCapId = findCapabilityByName(nodeId, myCapability);
  if (myCapId > 0) {
    getCapability(nodeId, myCapId)->setGetCallback(yourFunc);
  }
}
template void AthomDevice::setCapabilityGetCallback<int>(const int nodeId, const String myCapability, int (*yourFunc)() );
template void AthomDevice::setCapabilityGetCallback<float>(const int nodeId, const String myCapability, float (*yourFunc)() );
template void AthomDevice::setCapabilityGetCallback<bool>(const int nodeId, const String myCapability, bool (*yourFunc)() );


// Templated overloads for bool, int and float
template <class FuncType>
void AthomDevice::setCapabilitySetCallback(const int nodeId, const String myCapability, FuncType (*yourFunc)(FuncType) ) {
  // TODO Add return values
  int myCapId = findCapabilityByName(nodeId, myCapability);
  if (myCapId > 0) {
    getCapability(nodeId, myCapId)->setSetCallback(yourFunc);
  }
}
template void AthomDevice::setCapabilitySetCallback<int>(const int nodeId, const String myCapability, int (*yourFunc)(int) );
template void AthomDevice::setCapabilitySetCallback<float>(const int nodeId, const String myCapability, float (*yourFunc)(float) );
template void AthomDevice::setCapabilitySetCallback<bool>(const int nodeId, const String myCapability, bool (*yourFunc)(bool) );


AthomConfigItem* AthomDevice::getConfigItem(const int configItemNum) {
    // Returns a pointer to the requested node
    // Check bounds
    if ((0 < configItemNum) && (configItemNum <= _configItemCount)) {
      // Should note at this time we don't support
      // dynamic changes to node list. No delete.
      // MESH might change that. TODO
      if (configItemNum==1) {
        return _firstConfigItem;
      } else {
        int myCount = 1;
        AthomBaseObject* next = _firstConfigItem;
        while (next!=nullptr) {
          if (myCount==configItemNum) {
            return static_cast<AthomConfigItem*>(next);
          }
          myCount++;
          next = next->getNext();
       }
     }
    }
    return nullptr;
}


// Check if a Config Item exists, return 0 for no, -1 for invalid, or capaility ID
int AthomDevice::findConfigItemByName(const String myConf) {
  int configItemCount = 0;
  if (athomConfigIsValid(myConf)) {
    if (_firstConfigItem == nullptr) {
      // No capabilities
      return 0; // Config Item Not Found
    } else {
      // Step through capabilities
      AthomBaseObject* next = _firstConfigItem;
      while (next!=nullptr) {
        configItemCount++;
        if (static_cast<AthomConfigItem*>(next)->getName().compareTo(myConf)==0) {
          return configItemCount;
        }
        next = next->getNext();
      }
      return 0; // Config Item Not Found
    }
  } else {
    return -1; // Config Item is invalid
  }
}


int AthomDevice::addConfigItem(const String myName){
  int configItemCount = 0;
  if (findConfigItemByName(myName)!=0) {
    return -1; // duplicate or name not valid
  }
  AthomConfigItem* newConf = new AthomConfigItem(myName);
  // TODO: check for malloc fail??
  if (_firstConfigItem==nullptr) {
    _firstConfigItem=newConf;
    configItemCount=1;
  } else {
    AthomBaseObject* next = _firstConfigItem;
    AthomBaseObject* last = _firstConfigItem;
    while (next!=nullptr) {
      last = next;
      next = last->getNext();
      configItemCount++;
    }
    newConf->setPrev(last);
    last->setNext(newConf);
    configItemCount++;
  }
  // Tell the capability who it's parent is
  //newConf->setParent(this);
  // Store count of nodes.
  _configItemCount = configItemCount;
  // Update Cloud variable
  _updateHomeyConfs();
  return configItemCount;
}


// Templated overloads for bool, int and float
template <class FuncType>
void AthomDevice::setConfigItemGetCallback(const String myConfigItem, FuncType (*yourFunc)() ) {
  // TODO Add return values
  int myConfId = findConfigItemByName(myConfigItem);
  if (myConfId > 0) {
    getConfigItem(myConfId)->setGetCallback(yourFunc);
  }
}
template void AthomDevice::setConfigItemGetCallback<int>(const String myConfigItem, int (*yourFunc)() );
template void AthomDevice::setConfigItemGetCallback<float>(const String myConfigItem, float (*yourFunc)() );
template void AthomDevice::setConfigItemGetCallback<bool>(const String myConfigItem, bool (*yourFunc)() );


// Templated overloads for bool, int and float
template <class FuncType>
void AthomDevice::setConfigItemSetCallback(const String myConfigItem, FuncType (*yourFunc)(FuncType) ) {
  // TODO Add return values
  int myConfId = findConfigItemByName(myConfigItem);
  if (myConfId > 0) {
    getConfigItem(myConfId)->setSetCallback(yourFunc);
  }
}
template void AthomDevice::setConfigItemSetCallback<int>(const String myConfigItem, int (*yourFunc)(int) );
template void AthomDevice::setConfigItemSetCallback<float>(const String myConfigItem, float (*yourFunc)(float) );
template void AthomDevice::setConfigItemSetCallback<bool>(const String myConfigItem, bool (*yourFunc)(bool) );


int AthomDevice::countConfigItems() {
    return _configItemCount;
}


int AthomDevice::_updateHomeyClass() {
  // This to update Cloud Vars
  // Need to create a String containing JSON
  // Write array to cloud variable as JSON
  DynamicJsonBuffer jsonbuffer(622);
  JsonObject& root = jsonbuffer.createObject();
  for (int i = 1; i <= _nodeCount; i++) {
    root[String(i)] = getClass(i);
  }
  // assign to myHomeyClass
  _myHomeyClass = "";
  root.printTo(_myHomeyClass);
}


int AthomDevice::_updateHomeyCaps() {
  // This to update Cloud Vars
  // Need to create a String containing JSON
  // Write array to cloud variable as JSON
  DynamicJsonBuffer jsonbuffer(622);
  JsonObject& root = jsonbuffer.createObject();
  for (int i = 1; i <= _nodeCount; i++) {
    JsonArray& node = root.createNestedArray(String(i));
    int numCaps = countCapabilities(i);
    for (int c = 1; c <= numCaps; c++) {
      node.add(getCapabilityName(i,c));
    }
  }
  // assign to myHomeyCaps
  _myHomeyCaps = "";
  root.printTo(_myHomeyCaps);
}


int AthomDevice::_updateHomeyConfs() {
  // This to update Cloud Vars
  // Need to create a String containing JSON
  // Write array to cloud variable as JSON
  DynamicJsonBuffer jsonbuffer(622);
  JsonObject& root = jsonbuffer.createObject();
  for (int i = 1; i <= _configItemCount; i++) {
    JsonObject& conf = root.createNestedObject(String(i));
    AthomConfigItem* myConfigItem = getConfigItem(i);
    conf["n"] = myConfigItem->getName();
    conf["t"] = myConfigItem->getType();
  }
  // assign to myHomeyConfs
  _myHomeyConfs = "";
  root.printTo(_myHomeyConfs);
}


int AthomDevice::_myHomeyConf(const String message) {
   Serial.println("myHomeyConf Called");
   return 1;
}


int AthomDevice::_configItemGet(const String myConfigItem, const String myParam) {
  // Only called from _myHomeyGet
  // Branch to deal with get of Config items
  // If myParam is empty return value of param
  // If myParam is not recognised return error
  // valid myParams are
  //  "min"
  //  "max"
  //  "label"  - i.e. configItem Name
  //  "desc"
  //  "type"
  //  "value"
  // Also, will grab "special" info. as required through reserved configItem names
  // Check node and capability exist (return -1 if not)
  int confId = findConfigItemByName(myConfigItem);
  if (confId < 1) {
    debug("WARNING: Config Item Not Found: " + myConfigItem);
    return confId;
  }
  // Get the config Item
  AthomConfigItem* myItem = getConfigItem(confId);
  // Get MIN
  if (myParam=="min") {
    // Get minimum value
    if (myItem->isInt()) {
      _sendReport(0,myConfigItem,myParam,myItem->getMinInt());
      return 1;
    }
    if (myItem->isFloat()) {
      _sendReport(0,myConfigItem,myParam,myItem->getMinFloat());
      return 1;
    }
    if (myItem->isBool()) {
      _sendReport(0,myConfigItem,myParam,myItem->getMinBool());
      return 1;
    }
  }
  // Get MAX
  if (myParam=="max") {
    // Get minimum value
    if (myItem->isInt()) {
      _sendReport(0,myConfigItem,myParam,myItem->getMaxInt());
      return 1;
    }
    if (myItem->isFloat()) {
      _sendReport(0,myConfigItem,myParam,myItem->getMaxFloat());
      return 1;
    }
    if (myItem->isBool()) {
      _sendReport(0,myConfigItem,myParam,myItem->getMaxBool());
      return 1;
    }
  }
  // Get Label
  if (myParam=="label") {
    _sendReport(0,myConfigItem,myParam,myItem->getName());
    return 1;
  }
  // Get Desc
  if (myParam=="desc") {
    _sendReport(0,myConfigItem,myParam,myItem->getDesc());
    return 1;
  }
  // Get Type
  if (myParam=="type") {
    _sendReport(0,myConfigItem,myParam,myItem->getType());
    return 1;
  }
  // Get Value
  if (myParam=="value" || myParam=="") {
    if (myItem->isInt()) {
      _sendReport(0,myConfigItem,myParam,myItem->doGetInt());
      return 1;
    }
    if (myItem->isFloat()) {
      _sendReport(0,myConfigItem,myParam,myItem->doGetFloat());
      return 1;
    }
    if (myItem->isBool()) {
      _sendReport(0,myConfigItem,myParam,myItem->doGetBool());
      return 1;
    }
  }
  debug("_configItemGet Called but nothing recognised");
  return -1;
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
  DynamicJsonBuffer jsonbuffer(256);
  JsonObject& root = jsonbuffer.parseObject(message);
  if (root.success()) {
    // Looks valid (we received all parts)
    myCapability = root.get<String>("n");
    nodeId = myCapability.toInt();
    myCapability = root.get<String>("c");
    if (nodeId==0) {
      // Config Item GET branches
      // myCapability is the Config Setting, which param?
      String myParam = root.get<String>("p");
      return _configItemGet(myCapability, myParam);
    }
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
    return -3;
  }
  // Call the function
  // See what sort of type we will return and act accordingly
  if (myCap->isInt()) {
    int result = myCap->doGetInt();
    _sendReport(nodeId, myCapability, result);
    return 1;
  } else if (myCap->isFloat()) {
    float result = myCap->doGetFloat();
    _sendReport(nodeId, myCapability, result);
    return 2;
  } else if (myCap->isBool()) {
    bool result = myCap->doGetBool();
    _sendReport(nodeId, myCapability, result);
    return 3;
  }
  // TODO: we need to wrap in a timeout, if poss
  return -4;
}

int AthomDevice::_myHomeySet(const String message) {
  // Set a value via user provided function.
  // User function must accept and return same type of variable
  // int, float or bool.
  // Return value should be the actual value that was set.
  // This allows for limits.
  // Set will also (as per GET) trigger a report of the resulting value
  debug("myHomeySet Called");
  int nodeId = 0;
  String myCapability = "";
  String myValueStr = "";
  // We need to
  // Decode JSON to node and capability
  DynamicJsonBuffer jsonbuffer(256);
  JsonObject& root = jsonbuffer.parseObject(message);
  //
  if (root.success()) {
   // Looks valid (we received all parts)
    nodeId = root.get<int>("n");
    myCapability = root.get<String>("c");
    myValueStr =  root.get<String>("v");
  } else {
   // bad data
   debug("WARNING: Bad Data");
   return -1;
  }
  // TODO more validation of input
  // Get Capability and check type for conversion
  // Check node and capability exist (return -1 if not)
  int capId = findCapabilityByName(nodeId, myCapability);
  if (capId < 1) {
    debug("WARNING: Capability Not Found: " + myCapability);
    return capId;
  }
  // Get the capability
  AthomCapability* myCap = getCapability(nodeId,capId);
  // Check the capability isGetable
  if (!myCap->isSetable()) {
    debug("WARNING: Capability Not Settable");
    return -3;
  }
  // Call the callback function
  // See what sort of type we will return and act accordingly
  if (myCap->isInt()) {
    int result = myCap->doSet(int(myValueStr.toInt()));
    _sendReport(nodeId, myCapability, result);
    return 1;
  } else if (myCap->isFloat()) {
    float result = myCap->doSet(myValueStr.toFloat());
    _sendReport(nodeId, myCapability, result);
    return 2;
  } else if (myCap->isBool()) {
    bool myBool = false;
    if (myValueStr == "true") {
      myBool = true;
    }
    bool result = myCap->doSet(myBool);
    _sendReport(nodeId, myCapability, result);
    return 3;
  }
  // TODO: we need to wrap in a timeout, if poss
  return -4;
}


int AthomDevice::_myHomeyAct(const String message) {
   Serial.println("myHomeyAct Called");
   return 1;
}


int AthomDevice::_myHomeyRecv(const String message) {
   Serial.println("myHomeyRecv Called");
   return 1;
}


void AthomDevice::_sendReport(const int nodeId, const String myCap, const int value) {
  // Integer wrapper
  String strValue = String(value);
  _sendReport(nodeId, myCap, strValue);
}


void AthomDevice::_sendReport(const int nodeId, const String myConf, const String myParam, const int value) {
  // Integer wrapper with Config Param
  String strValue = String(value);
  DynamicJsonBuffer jsonbuffer(256);
  JsonObject& root = jsonbuffer.createObject();
  root[myParam] = strValue;
  strValue = "";
  root.printTo(strValue);;
  _sendReport(nodeId, myConf, strValue);
}


void AthomDevice::_sendReport(const int nodeId, const String myCap, const float value) {
  // Float wrapper
  String strValue = String(value, 2); // e.g. a 2 place decimal number
  _sendReport(nodeId, myCap, strValue);
}


void AthomDevice::_sendReport(const int nodeId, const String myConf, const String myParam, const float value) {
  // Float wrapper with Config Param
  String strValue = String(value, 2); // e.g. a 2 place decimal number
  DynamicJsonBuffer jsonbuffer(256);
  JsonObject& root = jsonbuffer.createObject();
  root[myParam] = strValue;
  strValue = "";
  root.printTo(strValue);
  _sendReport(nodeId, myConf, strValue);
}


void AthomDevice::_sendReport(const int nodeId, const String myCap, const bool value) {
  // Boolean wrapper
  String strValue;
  if (value) {
    strValue = "true";
  } else {
    strValue = "false";
  }
  _sendReport(nodeId, myCap, strValue);
}


void AthomDevice::_sendReport(const int nodeId, const String myConf, const String myParam, const bool value) {
  // Boolean wrapper with Config Param
  String strValue;
  if (value) {
    strValue = "true";
  } else {
    strValue = "false";
  }
  DynamicJsonBuffer jsonbuffer(256);
  JsonObject& root = jsonbuffer.createObject();
  root[myParam] = strValue;
  strValue = "";
  root.printTo(strValue);
  _sendReport(nodeId, myConf, strValue);
}


void AthomDevice::_sendReport(const int nodeId, const String myConf, const String myParam, const String value) {
  // String wrapper with Config Param
  DynamicJsonBuffer jsonbuffer(256);
  JsonObject& root = jsonbuffer.createObject();
  root[myParam] = value;
  String strValue = "";
  root.printTo(strValue);
  _sendReport(nodeId, myConf, strValue);
}


void AthomDevice::_sendReport(const int nodeId, const String myCap, const String value) {
  // OK now, to be clear, this is kind of blocking
  // Particle Cloud rate limits events to 1 Hz
  // So, to keep things under control, we wait
  // here, until at least a second since the last report
  // Build the data set first
  // As private function, input is assumed good
  DynamicJsonBuffer jsonbuffer(256);
  JsonObject& root = jsonbuffer.createObject();
  //
  JsonObject& node = root.createNestedObject(String(nodeId));
  node[myCap] = value;
  // assign to myHomeyCaps
  //debug("Checking frequency...");
  while ( millis()-_lastReport < 1000) {
    delay(100); // lower load than process() due to while
  }
  String data = "";
  root.printTo(data);
  debug(data);
  Particle.publish("Homey", data, PRIVATE);
  _lastReport = millis();
}
