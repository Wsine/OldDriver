#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "ControlSymbolCan.h"
#include "Steering.h"

using namespace std;

Steering::Steering() {
    nDeviceType = 4;
    nDeviceInd = 0;

    nDeviceTypeCar = 4;
    nDeviceIndCar = 0;

    nCANInd_1 = 0;
    nCANInd_2 = 0;
    nCANInd_3 = 1;
}

Steering::~Steering() {
    CloseDevice();
}

int Steering::StartDevice(void) {
    int dwRelOpenDevice;
    int dwRelVCI_InitCAN;
    int dwRelVCI_InitCAN_1;
    int dwRelVCI_InitCAN_2;

    nDeviceType = 4;
    nDeviceInd = 0;

    nDeviceTypeCar = 4;
    nDeviceIndCar = 0;

    nCANInd_1 = 0;
    nCANInd_2 = 0;
    nCANInd_3 = 1;

    dwRelOpenDevice = VCI_OpenDevice(nDeviceType, nDeviceInd, 0);

    if (dwRelOpenDevice != 1) {
        cout << "VCI_OpenDevice fail! " << endl;
        return -1;
    }

    /*初始 CAN*/
    vic_1.AccCode = 0x80000008;
    vic_1.AccMask = 0xFFFFFFFF;
    vic_1.Filter = 1;  //接受所有帧
    vic_1.Timing0 = 0x00;
    vic_1.Timing1 = 0x1C;//波特率500Kbps
    vic_1.Mode = 0;     //正常模式
    dwRelVCI_InitCAN = VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd_1, &vic_1);
    if (dwRelVCI_InitCAN != 1) {
        VCI_CloseDevice(nDeviceType, nDeviceInd);
        cout << "dwRelVCI_InitCAN fail!" << endl;
        return -1;
    }

    vic_2.AccCode = 0x80000008;
    vic_2.AccMask = 0xFFFFFFFF;
    vic_2.Filter = 1;
    vic_2.Timing0 = 0x00;
    vic_2.Timing1 = 0x1C;
    vic_2.Mode = 0;
    dwRelVCI_InitCAN_1 = VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd_2, &vic_2);
    if (dwRelVCI_InitCAN_1 != 1) {
        VCI_CloseDevice(nDeviceType, nDeviceInd);
        cout << "dwRelVCI_InitCAN_1 fail!" << endl;
        return -1;
    }

    vic_3.AccCode = 0x80000008;
    vic_3.AccMask = 0xFFFFFFFF;
    vic_3.Filter = 1;
    vic_3.Timing0 = 0x00;
    vic_3.Timing1 = 0x1C;
    vic_3.Mode = 1;
    dwRelVCI_InitCAN_2 = VCI_InitCAN(nDeviceTypeCar, nDeviceIndCar, nCANInd_3, &vic_3);
    if (dwRelVCI_InitCAN_2 != 1) {
        VCI_CloseDevice(nDeviceTypeCar, nDeviceIndCar);
        cout << "dwRelVCI_InitCAN_2 fail!" << endl;
        return -1;
    }

    if (VCI_StartCAN(nDeviceType, nDeviceInd, nCANInd_1) != 1) {
        VCI_CloseDevice(nDeviceType, nDeviceInd);
        cout << "VCI_StartCAN 1 fail!!" << endl;
        return -1;
    }

    if (VCI_StartCAN(nDeviceType, nDeviceInd, nCANInd_2) != 1) {
        VCI_CloseDevice(nDeviceType, nDeviceInd);
        cout << "VCI_StartCAN 2 fail!" << endl;
        return -1;
    }

    if (VCI_StartCAN(nDeviceTypeCar, nDeviceIndCar, nCANInd_3) != 1) {
        VCI_CloseDevice(nDeviceTypeCar, nDeviceIndCar);
        cout << "VCI_StartCAN 3 fail!" << endl;
        return -1;
    }

    VCI_ClearBuffer(nDeviceType, nDeviceInd, nCANInd_1);
    VCI_ClearBuffer(nDeviceTypeCar, nDeviceIndCar, nCANInd_3);
    return 1;
}

int Steering::SendSteeringCommand(short  steerWheelAngle, BYTE steerWheelSpd,
    BYTE vehicleSpd, BYTE engineSpd, BYTE steerWheelStatus) {
    int dwRel;
    vco_send[0].ID = (UINT)(0x0000B500);
    vco_send[0].RemoteFlag = 0;
    vco_send[0].ExternFlag = 1; //扩展帧
    vco_send[0].DataLen = 7;

    //steerWheelAngle = 1024 + steerWheelAngle; //?

    steerWheelAngle = steerWheelAngle * 10;

    BYTE highangel = steerWheelAngle >> 8;

    BYTE lowangel = (steerWheelAngle << 8) >> 8;

    //short temp_angle;
    //temp_angle = ((int)highangel << 8) + (int)lowangel;
    //temp_angle = temp_angle / 10;


    BYTE b0 = vco_send[0].Data[0] = highangel;
    BYTE b1 = vco_send[0].Data[1] = lowangel;
    BYTE b2 = vco_send[0].Data[2] = steerWheelSpd;
    BYTE b3 = vco_send[0].Data[3] = vehicleSpd;

    BYTE b4 = vco_send[0].Data[4] = engineSpd;
    BYTE b5 = vco_send[0].Data[5] = steerWheelStatus;
    //BYTE temp_result = b0^b1^b2^b3^b4^b5;
    BYTE b6 = vco_send[0].Data[6] = 0x00;

    dwRel = VCI_Transmit(nDeviceType, nDeviceInd, nCANInd_1, vco_send, 1);
    return 1;
}

int Steering::WriteSteering(short steeringangle, BYTE steerWheelSpd) {
    SendSteeringCommand(steeringangle, steerWheelSpd, 0x00, 0x00, 0x20);
    return 1;
}

int Steering::CloseDevice() {
    int dwRel;
    dwRel = VCI_CloseDevice(nDeviceType, nDeviceInd);
    if (dwRel != 1) {
        // Can Close Errors
        return -1;
    }
    return 1;
}

double Steering::ReceiveSteeringAngle(void) {
    double current_steeringangle = 0;
    short temp_angle = 0;
    int dwRel = VCI_Receive(nDeviceType, nDeviceInd, nCANInd_1, vco_receive, 60, 0);

    if (dwRel > 0 && vco_receive[0].ID == (UINT)(0x0000A500)) {
        BYTE high = (BYTE)(vco_receive[0].Data[0]);
        BYTE low = (BYTE)(vco_receive[0].Data[1]);

        temp_angle = ((int)high << 8) + (int)low;
        current_steeringangle = temp_angle / 10.0;
        return current_steeringangle;
    } else if (dwRel == -1) {
        // no streeing CAN data. 
        // VCI_CloseDevice(nDeviceType, nDeviceInd);
        // VCI_OpenDevice(nDeviceType, nDeviceInd, 0);
    }
    return 1;
}
