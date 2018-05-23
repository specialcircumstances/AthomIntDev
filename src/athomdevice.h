/*
 * Project AthomIntDev
 * Description: Oject definitions for Athom Homey devices on Particle
 * Author: SpecialCircumstances
 * Date:
 */
#ifndef athomdevice_h
#define athomdevice_h

// #include "Arduino.h"
#include "application.h"    // Need String at least
#include "athomdefs.h"
#include "JsonParserGeneratorRK.h"

// Bearing in mind that Homey has the following types:
// Number, which is Decimal to two places.
// boolean
// ENUM
// So, we will use an internal "int" and shift the decimal point.
// e.g. 1 is stored as 100

// Forward Declarations

class AthomNode;
class AthomDevice;

// Class definitions

 class AthomCapability
 {
   public:
     AthomCapability(const String myCap); // Constructor
     // Get and set capability name
     int setCapability(const String myCap);
     String getCapability();
     //
     void setCallback( int (*yourFunc)(int) ); // callback func for set
     void getCallback( int (*yourFunc)(int) ); // callback func for get
     //
     int doSet(const int myValue);
     int doGet(const int myValue);

     // To support multiple capabilities in an efficient manner
     int setNode(AthomNode* myNode);
     AthomCapability* getPrev();
     int setPrev(AthomCapability* prevCapability);
     AthomCapability* getNext();
     int setNext(AthomCapability* nextCapability);

   private:
     char _myCapability[MAX_CHARS_CLASS];
     bool _isSetable;     // true when callback attached
     bool _isGetable;     // true when callback attached
     int _lastSet;        // value of last set (from Homey)
     int _lastGet;        // value of last get (from callback)
     int (*_setCallback) (int);   // set callback function
     int (*_getCallback) (int);   // get callback function
     AthomCapability* _prevCapability;
     AthomCapability* _nextCapability;
     AthomNode* _myNode;  // Which node is this attached to
 };


 class AthomAction        // Something we can do in response to a Flow
 {
   public:
     AthomAction(const String actionName, int (*yourFunc)(int)); // Constructor
     // Get and set capability name
     int setName(const String actionName);
     String getName();
     //
     void setCallback( int (*yourFunc)(int) ); // callback func for action
     //
     int doAction(const int myValue);       // trigger function from Homey

     // To support multiple actions in an efficient manner
     int setNode(AthomNode* myNode);
     AthomAction* getPrev();
     int setPrev(AthomAction* prevAction);
     AthomAction* getNext();
     int setNext(AthomAction* nextAction);

   private:
     String _actionName;        // Name of the action
     bool _isUsable;     // true when callback attached
     int (*_actionCallback) (int);   // action callback function
     AthomAction* _prevAction;
     AthomAction* _nextAction;
     AthomNode* _myNode;  // Which node is this attached to
 };


 class AthomTrigger        // An event we want to offer to trigger Flows
 {
   public:
     AthomTrigger();  //Constructor
     // Get and set Trigger name
     int setId(String triggerId);
     String getId();
     int setTitle(String triggerTitle);
     String getTitle();

    // TODO: Tokens. And generally thinking more about triggers

     // Do it
     int trigger();       //Send the trigger to Homey

     // To support multiple triggers in an efficient manner
     AthomTrigger* getPrev();
     int setPrev(AthomTrigger* prevTrigger);
     AthomTrigger* getNext();
     int setNext(AthomTrigger* nextTrigger);

   private:
     String _name;        // Name of the trigger
     AthomTrigger* _prevTrigger;
     AthomTrigger* _nextTrigger;
 };



 class AthomNode
 {
   public:
     AthomNode(const String myClass); // Constructor
     int setName(const String myName); // Set Name of Node
     String getName(); // Retrieve Name of Node

     // Class
     int setClass(const String myClass);     // Set Homey Class of Device
     String getClass();                // Retrieve Class of Device

     // capabilities
     int addCapability(const String myCapability); // Add a Capability to the Node
     AthomCapability* getCapability(const int capNumber);
     //String getCapabilities(const int nodeId);
     int countCapabilities();
     int findCapability(const String myCapability); // Check if capability exists

     // actions
     int addAction(String myAction,  int (*yourFunc)(int)); // Add an Action to the Node
     String getActions();

     // triggers
     // TODO:

     // To support multiple nodes in an efficient manner
     int setDevice(AthomDevice* myDevice);
     AthomNode* getPrev();
     int setPrev(AthomNode* prevNode);
     AthomNode* getNext();
     int setNext(AthomNode* nextNode);

     //void dot();
     //void dash();
   private:
     String _myName;
     char _myClass[MAX_CHARS_CLASS];
     int _capabilityCount;
     AthomCapability* _firstCapability; // PTR to first capability
     AthomAction* _firstAction;         // PTR to first action
     AthomNode* _prevNode;              //PTR to previous Node
     AthomNode* _nextNode;              //PTR to next Node
     AthomDevice* _myDevice;            // PTR to device
 };


  class AthomDevice
  {
    public:
      AthomDevice();
      int setName(const String myName);       // Set Name of Device
      String getName();                 // Retrieve Name of Device

      // nodes
      int addNode(const String nodeClass);        // Add a Node to the Device
      //int deleteNode(const int nodeId);     // Delete a Node from the Device
      //String listNodes();                 // Get a list of Node TODO:PTR to array
      int countNodes();                  // Returns the number of nodes
      AthomNode* getNode(const int nodeNumber);
      String getClass(const int nodeId); // get the class of node x

      // capabilities
      int addCapability(const int nodeId, const String myCapability);
      String getCapability(const int nodeNumber, const int capNumber);
      int countCapabilities(const int nodeId);
      int findCapability(const int nodeId, const String myCapability);

    private:
      char _myName[MAX_CHARS_NAME];
      AthomNode* _firstNode;              // PTR to first node
      int _nodeCount;
      int _updateHomeyClass();
      int _updateHomeyCaps();
      int _updateHomeyConfs();
      int _updateHomeyActs();
      int _updateHomeySend();

      // Cloud variables we will expose
      int _myHomeyAPI;                     // We use as boolean
      String _myHomeyClass;
      String _myHomeyCaps;
      String _myHomeyConfs;
      String _myHomeyActs;
      String _myHomeySend;       // A reponse channel ?????

      // Cloud Functions we will expose
      int _myHomeyConf(String message);
      int _myHomeyGet(String message);
      int _myHomeySet(String message);
      int _myHomeyAct(String message);
      int _myHomeyRecv(String message); // A query channel ?????

  };


 #endif
