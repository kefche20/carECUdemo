#ifndef LSMANAGER_HPP
#define LSMANAGER_HPP

#include "Led.hpp"
#include "CanDataProcession.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class LSManager : public ICanListener
{
private:
    // light system component
    BlinkingLed leftInd;  // left indicator
    BlinkingLed rightInd; // right indicator
    TweakingLed highBm;   // high beam

    // rtos
    SemaphoreHandle_t mutex;
    QueueHandle_t rcdCmdQueue; // recieved message queue

public:
    LSManager(uint8_t leftIndPin, uint8_t rightIndPin, uint8_t highBm);

    bool RecieveMessage(CanData newData) override
    {
        return xQueueSend(rcdCmdQueue, (void *)&newData, portMAX_DELAY);
    }

private:
    static void ReadingCommandTask(void *parameter)
    {
        LSManager *lsManager = static_cast<LSManager *>(parameter);
        CanData rcdData;

        while (1)
        {
            if (xQueueReceive(lsManager->rcdCmdQueue, (void *)&rcdData, portMAX_DELAY))
            {
                switch (rcdData.msgId)
                {
                case NODE_ID_INDICATOR:
                    HandleIndicatorCommand(rcdData.command[0], lsManager->leftInd, lsManager->rightInd);
                    break;
                case NODE_ID_HIGHBEAM:
                    HandleHighHBeamCommand(rcdData.command[0], lsManager->highBm);
                    break;
                }
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }

    static bool HandleIndicatorCommand(byte command, BlinkingLed &leftInd, BlinkingLed &rightInd)
    {
        //checking if command is out of range 
        if (command < LEFT_INDICATOR || command > BOTH_OFF)
        {
            return false;
        }

        LighSystemCommand cmd = static_cast<LighSystemCommand>(command);
        
        //turn on and off indiccators according to the command
        switch (cmd)
        {
        case LEFT_INDICATOR:
            leftInd.SetBlinking(RealTime::ON);
            rightInd.SetBlinking(RealTime::OFF);

            break;
        case RIGHT_INDICATOR:
            leftInd.SetBlinking(RealTime::OFF);
            rightInd.SetBlinking(RealTime::ON);

            break;
        case BOTH_OFF:
            leftInd.SetBlinking(RealTime::OFF);
            rightInd.SetBlinking(RealTime::OFF);
            break;
        }

        return true;
    }

    static bool
    HandleHighHBeamCommand(byte command, TweakingLed &highBm)
    {
        if (command < INCREASE || command > DECREASE)
        {
            return false;
        }

        // add tweaking cmd to regulate the brightness of the high beam
        Tendency tendency = static_cast<Tendency>(command);
        return highBm.AddTweakingCommand(tendency);
    }
};

#endif