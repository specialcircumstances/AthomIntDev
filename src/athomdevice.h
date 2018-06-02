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
#include "Arduino.h"      // Required for JSON lib
#include "ArduinoJson.h"
#include "athomdefs.h"

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

 class AthomBaseObject
 {
   // Base object supporting list operations
   public:
     AthomBaseObject();
     AthomBaseObject(const String myName);  // Constructor
     // Get and set name
     virtual int setName(const String myCap);
     virtual String getName();
     //
     // List methods
     virtual int setParent(AthomBaseObject* myParent);
     virtual AthomBaseObject* getPrev();
     virtual int setPrev(AthomBaseObject* prevObject);
     virtual AthomBaseObject* getNext();
     virtual int setNext(AthomBaseObject* nextObject);

   protected:
     char _myName[MAX_CHARS_NAME];
     AthomBaseObject* _prevObject;
     AthomBaseObject* _nextObject;
     AthomBaseObject* _myParent;

 };


 class AthomGetSetObject: public AthomBaseObject {
    // Extends AthomBaseObject to support Get Set and Callbacks
  public:
    AthomGetSetObject();
    AthomGetSetObject(const String myName); // Constructor override
    int setSetCallback( int (*yourFunc)(int) ); // callback func for set
    int setSetCallback( float (*yourFunc)(float) ); // callback func for set
    int setSetCallback( bool (*yourFunc)(bool) ); // callback func for set
    //
    int setGetCallback( int (*yourFunc)() ); // callback func for get
    int setGetCallback( float (*yourFunc)() ); // callback func for get
    int setGetCallback( bool (*yourFunc)() ); // callback func for get
    //
    int doSet(const int myValue);
    float doSet(const float myValue);
    bool doSet(const bool myValue);
    int doGet(const int myValue);
    int doGetInt();
    float doGet(const float myValue);
    float doGetFloat();
    bool doGet(const bool myValue);
    bool doGetBool();
    bool isGetable();
    bool isSetable();
    bool isInt();
    bool isFloat();
    bool isBool();
    String getType(); // String representation of type

  protected:
    bool _isSetable;     // true when callback attached
    bool _isGetable;     // true when callback attached
    bool _isInt;
    bool _isFloat;
    bool _isBool;
    char _setCallbackType;
    int (*_setCallbacki)(int);   // set callback function
    float (*_setCallbackf)(float);   // set callback function
    bool (*_setCallbackb)(bool);   // set callback function
    char _getCallbackType;
    int (*_getCallbacki)();   // get callback function
    float (*_getCallbackf)();   // get callback function
    bool (*_getCallbackb)();   // get callback function
 };


 class AthomCapability : public AthomGetSetObject
 {
    public:
      AthomCapability();
      AthomCapability(const String myCap); // Constructor override
 };


 class AthomConfigItem : public AthomGetSetObject
 {
    public:
      AthomConfigItem();
      AthomConfigItem(const String myCap); // Constructor override
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



 class AthomNode : public AthomBaseObject
 {
   public:
     AthomNode(const String myClass); // Constructor

     // Class
     int setClass(const String myClass);     // Set Homey Class of Device
     String getClass();                // Retrieve Class of Device

     // capabilities
     int addCapability(const String myCapability); // Add a Capability to the Node
     AthomCapability* getCapability(const int capNumber);
     //String getCapabilities(const int nodeId);
     int countCapabilities();
     int findCapabilityByName(const String myCapability); // Check if capability exists

     // actions
     int addAction(String myAction,  int (*yourFunc)(int)); // Add an Action to the Node
     String getActions();

     // triggers
     // TODO:

   private:
     int _capabilityCount;
     AthomCapability* _firstCapability; // PTR to first capability
     AthomAction* _firstAction;         // PTR to first action

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
      AthomCapability* getCapability(const int nodeNumber, const int capNumber);
      String getCapabilityName(const int nodeNumber, const int capNumber);
      int countCapabilities(const int nodeId);
      int findCapabilityByName(const int nodeId, const String myCapability);
      template <class FuncType> // we allow int, float and bool
      void setCapabilityGetCallback(const int nodeId, const String myCapability, FuncType (*yourFunc)() );
      template <class FuncType>
      void setCapabilitySetCallback(const int nodeId, const String myCapability, FuncType (*yourFunc)(FuncType) );

      // configuration items
      int addConfigItem(const String myConfigItem); // Add a Capability to the Node
      AthomConfigItem* getConfigItem(const int configItemNum);
      //String getCapabilities(const int nodeId);
      int countConfigItems();
      int findConfigItemByName(const String myConfigItem); // Check if capability exists
      template <class FuncType>
      void setConfigItemGetCallback(const String myConfigItem, FuncType (*yourFunc)() );
      template <class FuncType>
      void setConfigItemSetCallback(const String myConfigItem, FuncType (*yourFunc)(FuncType) );

    private:
      char _myName[MAX_CHARS_NAME];
      AthomNode* _firstNode;              // PTR to first node
      AthomConfigItem* _firstConfigItem;  // PTR to first config item
      int _nodeCount;
      int _configItemCount;
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

      unsigned long _lastReport; // Last report based on millis()
      void _sendReport(const int nodeId, const String myCap, const int value);
      void _sendReport(const int nodeId, const String myCap, const float value);
      void _sendReport(const int nodeId, const String myCap, const bool value);
      void _sendReport(const int nodeId, const String myCap, const String value);


  };


 #endif
