#ifndef _ACTION_DRIVER_
#define _ACTION_DRIVER_


#include "can.h"

#ifdef __cplusplus  //为C提供接口
extern "C"
{
#endif

typedef struct
{
  uint8_t command_specifier_not_valid;

}SDO_ERROR;

#define SDO_LOAD_1 0x4f
#define SDO_LOAD_2 0x4b
#define SDO_LOAD_3 0x47 
#define SDO_LOAD_4 0x43
#define BOOTUP     0x00
#define COMMAND_SPECIFIER_NOT_VALID 0x0504
#define POSITION_ACTUAL_VALUE 0x6064// Actual position as taken from position sensor,in user units
#define ACTUAL_POSITION_INTERNAL_UNIT 0x6063//Actual position taken from position sensor,in increments
#define VELOCITY_SENSOR_ACTUAL_VALUE 0x6069
#define CURRENT_ACTUAL_VALUE 0x6078
#define SDOTX 0x600
#define SDORX 0x580
#define DATA_LEN 8
#define ENABLE  1
#define DISABLE 0
#define RELATIVE_POSITION_MODE 0
#define ABSOLUATE_POSITION_MODE 1
#define CURRENT 1
#define VELOCITY 2
#define RELATIVE_POSITION 3
#define ABSOLUATE_POSITION 4

void Action_Set_Heartbeat_Tim(uint8_t drivernum,uint16_t heartbeat_time);
void Action_Set_Operational_Mode(uint8_t drivernum);
void Action_Enable_Driver(uint8_t driverNum,int8_t status);
void Action_Config_Vel(uint8_t driverNum,uint32_t acc,uint32_t dec);
void Action_Set_Velocity(uint8_t driverNum,int32_t vel) ;
void Actin_Config_Pos(uint8_t driverNum,int32_t vel,uint32_t acc,uint32_t dec);
void Action_Set_Position(uint8_t driverNum,uint8_t mode,int32_t  pos);
void Action_Offset_Home(uint8_t driverNum,int32_t position);
void Action_Get_Motorstatus(uint8_t driverNum,uint16_t status);
uint8_t Action_Check_Error(CanRxMsg* RxMessage,uint8_t drive_num);
uint16_t Action_Get_Motor_Feedback(CanRxMsg* RxMessage,uint8_t drivernum);
void Actin_Config_Torque(uint8_t driverNum,int16_t max_torque);
void Action_Set_Torque( uint8_t driverNum,int16_t torque );

void Set_Speed_Value(int tmp);
void Set_AbsolutePosition_Value(int tmp);
void Set_RelativePosition_Value(int tmp);
void Set_Current_Value(int tmp);

int32_t Get_Speed_Value(void);
int32_t Get_AbsolutePosition_Value(void);
int32_t Get_RelativePosition_Value(void);
int32_t Get_Current_Value(void);


#ifdef __cplusplus //为C提供接口
}
#endif	

#endif

