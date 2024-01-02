#ifndef ISENDER_HPP
#define ISENDER_HPP

#include <cstdint>
enum MessageID
{
  VOID_ID = -1,
  NODE_ID_LEFTBLINKER,         // Accepts on or off
  NODE_ID_RIGHTBLINKER,        // Accepts on or off
  NODE_ID_HIGHBEAM,            // Accepts on or off
  NODE_ID_LOWBEAM,             // Accepts on or off
  NODE_ID_TAILLIGHT,           // Accepts on or off
  NODE_ID_BRAKELIGHT,          // Accepts on or off
  NODE_ID_REVERSELIGHT,        // Accepts on or off
  NODE_ID_ACCELERATE,          // Accepts speed in % from 0 to 100
  NODE_ID_DECELERATE,          // Accepts speed in % from 0 to 100
  NODE_ID_STEERLEFT,           // Accepts angle in degrees from 0 to 360
  NODE_ID_STEERRIGHT,          // Accepts angle in degrees from 0 to 360
  NODE_ID_SPEED,               // Accepts request
  NODE_ID_RPM,                 // Accepts request
  NODE_ID_TEMPERATURE,         // Accepts request
  NODE_ID_PRESSURE,            // Accepts request
  NODE_ID_DASHBOARD,           // Sends requests and accepts data
  NODE_ID_CRUISECONTROL,       // Accepts on or off
  NODE_ID_CRUISECONTROL_SPEED, // Accepts speed in % from 0 to 100
};

// Commands for nodes over CAN
enum CANCommand
{
  COMMAND_OFF,
  COMMAND_ON,
  COMMAND_REQUEST,
};

class ISender
{
public:
    virtual bool SendMessage(uint8_t msgId, float value) = 0;
    virtual bool SendMessage(uint8_t msgId, int value) = 0;
};

#endif