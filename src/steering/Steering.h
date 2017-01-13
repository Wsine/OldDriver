#ifndef _STEERING_CAN_H
#define _STEERING_CAN_H

#include "ControlSymbolCan.h"

class Steering {
    VCI_CAN_OBJ vco_receive[64];//64
    VCI_CAN_OBJ vco_receiveCar[64];
    VCI_CAN_OBJ vco_send[8];
    int nDeviceType;
    int nDeviceInd;

    int nDeviceTypeCar;
    int nDeviceIndCar;

    //int dwRel;
    int SendSteeringCommand( short steerWheelAngle, BYTE steerWheelSpd,
        BYTE vehicleSpd, BYTE engineSpd, BYTE steerWheelStatus);

    int nCANInd_1;
    VCI_INIT_CONFIG vic_1;

    int nCANInd_2;
    VCI_INIT_CONFIG vic_2;

    int nCANInd_3;
    VCI_INIT_CONFIG vic_3;

public:
    short int current_steeringangle;

    Steering();

    ~Steering();

    int StartDevice(void );
    int CloseDevice();

    int StartHuman_Driving(unsigned short int steering_angle);
    int StartSelf_Driving(short steeringangle, BYTE steerWheelSpd);
    int StopDSP(unsigned short int steering_angle);

    double ReceiveStreeingAngle();
    int ReceiveVehicleSpeed();
};  

#endif
