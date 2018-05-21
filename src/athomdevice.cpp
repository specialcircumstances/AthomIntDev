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

// Helper Functions

 bool athomIsClass(String myClass) {
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


 // Class Definitions

 AthomCapability::AthomCapability() {  // Class Constructor

 }

 AthomAction::AthomAction() {          // Class Constructor

 }

 AthomTrigger::AthomTrigger() {       // Class Constructor

 }

// ***************************************************

 AthomNode::AthomNode(const String myClass){              // Class Constructor
  // Init private vars
   _myName = "Undefined";
   //_myClass =
   _firstCapability = nullptr;
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
   return _nextNode;
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


// **********************************************


 AthomDevice::AthomDevice() {
   // init private variables
   _myName = "UNDEFINED";
   _firstNode = nullptr;
   _nodeCount = 0;
   // Init private Cloud variables
   _myHomeyAPI = 0;   //false
   _myHomeyClass = "";
   _myHomeyCaps = "";
   _myHomeyConfs = "";
   _myHomeyActs = "";
   _myHomeySend = "";
 }            // Class Constructor

 int AthomDevice::setName(const String myName) {
   _myName = myName;
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
    //
  }
  return nodeCount;
}

int AthomDevice::countNodes() {
  return _nodeCount;
}

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
