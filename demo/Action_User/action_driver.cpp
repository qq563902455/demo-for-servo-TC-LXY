#include "action_driver.h"
SDO_ERROR sdo_error;

/************************************************
函数名称：Action_SendRemote
函数功能：发送远程帧实验 
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
************************************************/
void Action_SendRemote(uint8_t drivernum,uint16_t pdo)
{
  uint8_t transmit_mailbox;	                  //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId= 0x180 + pdo - 0x1800 + drivernum;					 // standard identifier=0
	TxMessage.ExtId= 0;					 // extended identifier=StdId
	TxMessage.IDE  = CAN_Id_Standard ;			     // type of identifier for the message is Standard
	TxMessage.RTR  = CAN_RTR_Remote;			       // the type of frame for the message that will be transmitted
	TxMessage.DLC  = 0;

		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
}

/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
************************************************/
void Action_SetRPdo(uint8_t drivernum,uint16_t pdo,uint8_t enable)
{
	//对于RPDO ，需设置COB-ID，映射
	//对于TPDO ，需设置COB-ID，映射，通讯方式，事件计时器
	//发送相同COB-ID的远程帧后，生产者即可按PDO参数返回数据
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x23001401,0x00000000,		// Index-1400(RPDO1)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+drivernum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+drivernum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;

	data[0][0] = data[0][0] |((pdo-0x1400)<<16) ;
	switch(enable)
	{
		case ENABLE:
      data[0][1] = ((( pdo - 0x1400 + 2 )<< 8 ) | 0x00000000 ) + drivernum ;			
			break;
		case DISABLE:
			data[0][1] = ((( pdo - 0x1400 + 2 )<< 8 ) | 0x80000000 ) + drivernum ;
			break;
	}								  
	data[0][1] = (((data[0][1]>>24)&0xff))+(((data[0][1]>>16)&0xff)<<8)+ (((data[0][1]>>8)&0xff)<<16)+((data[0][1]&0xff)<<24);		
						 
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
}
/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
************************************************/
void Action_SetTPdo(uint8_t drivernum,uint16_t pdo,uint8_t enable)
{
	//对于RPDO ，需设置COB-ID，映射
	//对于TPDO ，需设置COB-ID，映射，通讯方式，事件计时器
	//发送相同COB-ID的远程帧后，生产者即可按PDO参数返回数据
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x23001801,0x00000000,		// Index-1800(TPDO1)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+drivernum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+drivernum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;

	data[0][0] = data[0][0] |((pdo-0x1800)<<16) ;
	switch(enable)
	{
		case ENABLE:
			data[0][1] = ((( pdo - 0x1800 + 1 )<< 8 ) | 0x00000080 ) + drivernum ;			
			break;
		case DISABLE:
			data[0][1] = ((( pdo - 0x1800 + 1 )<< 8 ) | 0x80000080 ) + drivernum ;
			break;
	}		
   
	data[0][1] = (((data[0][1]>>24)&0xff))+(((data[0][1]>>16)&0xff)<<8)+ (((data[0][1]>>8)&0xff)<<16)+((data[0][1]&0xff)<<24);							 
						 
    	
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
}

/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
      当TPDO的传输类型为0 时，接收到同步帧，则发送该PDO（非同步）
      当TPDO的传输参数为1~240时，接收到相应个数的同步帧，则发送该PDO
      当TPDO的传输参数为254时， 制造商自定义事件（ n*500us 为周期发送该PDO参数 ）
************************************************/
void Action_SetTPdoComType(uint8_t drivernum,uint16_t pdo,uint8_t type)
{
	//对于RPDO ，需设置COB-ID，映射
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x2f001802,0x00000000,		// Index-1800(TPDO1)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+drivernum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+drivernum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;

	data[0][0] = data[0][0] |((pdo-0x1800)<<16) ;
  data[0][1] = type; 
	data[0][1] = (((data[0][1]>>24)&0xff))+(((data[0][1]>>16)&0xff)<<8)+ (((data[0][1]>>8)&0xff)<<16)+((data[0][1]&0xff)<<24);							 
						 
    	
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
}


/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
      当RPDO的传输参数为0~240时， 接收到一个同步帧后，将数据更新到应用
      当RPDO的传输参数为254、255时，接收到数据即更新到应用
************************************************/
void Action_SetRPdoComType(uint8_t drivernum,uint16_t pdo,uint8_t type)
{
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x2f001402,0x00000000,		// Index-1400(RPDO1)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+drivernum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+drivernum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;

	data[0][0] = data[0][0] |((pdo-0x1400)<<16) ;
  data[0][1] = type; 
	data[0][1] = (((data[0][1]>>24)&0xff))+(((data[0][1]>>16)&0xff)<<8)+ (((data[0][1]>>8)&0xff)<<16)+((data[0][1]&0xff)<<24);							 
						 
    	
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
}


/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
      禁止时间，仅为TPDP性质
      TPDO发送间隔不得小于该时间，单位100us
************************************************/
void Action_SetTPdoBanTime(uint8_t drivernum,uint16_t pdo,uint8_t time)
{
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x2f001803,0x00000000,		// Index-1800(TPDO1)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+drivernum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+drivernum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;

	data[0][0] = data[0][0] |((pdo-0x1800)<<16) ;
  data[0][1] = time ; 
	data[0][1] = (((data[0][1]>>24)&0xff))+(((data[0][1]>>16)&0xff)<<8)+ (((data[0][1]>>8)&0xff)<<16)+((data[0][1]&0xff)<<24);							 
						 
    	
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
}

/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
      事件计数器，仅为TPDP性质
************************************************/
void Action_SetTPdoEventCnt(uint8_t drivernum,uint16_t pdo,uint8_t event_cnt)
{
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x2f001805,0x00000000,		// Index-1800(TPDO1)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+drivernum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+drivernum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;

	data[0][0] = data[0][0] |((pdo-0x1800)<<16) ;
  data[0][1] = event_cnt ; 
	data[0][1] = (((data[0][1]>>24)&0xff))+(((data[0][1]>>16)&0xff)<<8)+ (((data[0][1]>>8)&0xff)<<16)+((data[0][1]&0xff)<<24);							 
						 
    	
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
}


/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
      事件计数器，仅为TPDP性质
************************************************/
void Action_SetTPdoMap(uint8_t drivernum,uint16_t pdo)
{
	 uint8_t i=0; 
	 uint32_t data[3][2]={    				 
						   0x2f001a00,0x00000002,		// 这里设置tpdo1对应两个映射（有效个数为1~8）
						   0x23001a01,0x60780010,
	             0x23001a02,0x60690020,  
	             };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+drivernum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+drivernum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;

	data[0][0] = data[0][0] |((pdo-0x1800)<<16) ;
	data[1][0] = data[1][0] |((pdo-0x1800)<<16) ;						 
	data[2][0] = data[2][0] |((pdo-0x1800)<<16) ;		
							 
	data[0][1] = (((data[0][1]>>24)&0xff))+(((data[0][1]>>16)&0xff)<<8)+ (((data[0][1]>>8)&0xff)<<16)+((data[0][1]&0xff)<<24);							 
	data[1][1] = (((data[1][1]>>24)&0xff))+(((data[1][1]>>16)&0xff)<<8)+ (((data[1][1]>>8)&0xff)<<16)+((data[1][1]&0xff)<<24);		
	data[2][1] = (((data[2][1]>>24)&0xff))+(((data[2][1]>>16)&0xff)<<8)+ (((data[2][1]>>8)&0xff)<<16)+((data[2][1]&0xff)<<24);						 
    	
	for(i = 0;i<3;i++)
	{
		TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
		TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
		TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
		TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
							 
		TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
		TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
		TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
		TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
				
		transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
		while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
	}
}

/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
      事件计数器，仅为TPDP性质
************************************************/
void Action_SetRPdoMap(uint8_t drivernum,uint16_t pdo)
{
	 uint8_t i=0; 
	 uint32_t data[3][2]={    				 
						   0x2f001600,0x00000002,		// 这里设置pdo1对应两个映射（有效个数为1~8）
						   0x23001601,0x60830020,
	             0x23001602,0x60840020,  
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+drivernum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+drivernum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;

	data[0][0] = data[0][0] |((pdo-0x1400)<<16) ;
	data[1][0] = data[1][0] |((pdo-0x1400)<<16) ;						 
	data[2][0] = data[2][0] |((pdo-0x1400)<<16) ;						 
						 
	data[0][1] = (((data[0][1]>>24)&0xff))+(((data[0][1]>>16)&0xff)<<8)+ (((data[0][1]>>8)&0xff)<<16)+((data[0][1]&0xff)<<24);							 
	data[1][1] = (((data[1][1]>>24)&0xff))+(((data[1][1]>>16)&0xff)<<8)+ (((data[1][1]>>8)&0xff)<<16)+((data[1][1]&0xff)<<24);		
	data[2][1] = (((data[2][1]>>24)&0xff))+(((data[2][1]>>16)&0xff)<<8)+ (((data[2][1]>>8)&0xff)<<16)+((data[2][1]&0xff)<<24);						 
    	
	for(i = 0;i<3;i++)
	{
		TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
		TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
		TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
		TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
							 
		TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
		TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
		TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
		TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
				
		transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
		while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
	}						 
}

/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
      以PDO方式发送速度模式下的加速度和减速度
************************************************/
void Action_PDOVelConfig(uint8_t drivernum,uint16_t pdo,uint32_t acc,uint32_t dec)
{
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x00000000,0x00000000,	 
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=0x200 + pdo - RPDO1 + drivernum;					 // standard identifier=0
	TxMessage.ExtId=0;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;

	data[0][0] = acc;
	data[0][1] = dec;						 
						 
	data[0][1] = (((data[0][1]>>24)&0xff))+(((data[0][1]>>16)&0xff)<<8)+ (((data[0][1]>>8)&0xff)<<16)+((data[0][1]&0xff)<<24);							 
	data[0][0] = (((data[0][0]>>24)&0xff))+(((data[0][0]>>16)&0xff)<<8)+ (((data[0][0]>>8)&0xff)<<16)+((data[0][0]&0xff)<<24);		
    	

		TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
		TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
		TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
		TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
							 
		TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
		TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
		TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
		TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
				
		transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
		while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
							 
}
/************************************************
函数名称：Action_SetPdo
函数功能: 配置PDO
入口参数：drivernum-电机节点号.
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
      以PDO方式发送速度模式下的加速度和减速度
************************************************/
uint8_t Action_PDOReData(CanRxMsg* RxMessage,uint8_t drivernum,uint16_t pdo)
{
	    static int32_t Speed_Value=0;
	    static int32_t AbsolutePosition_Value=0;
	    static int32_t RelativePosition_Value=0;	
     	static int32_t Current_Value=0;		

			if((RxMessage->StdId - drivernum ) == (((pdo + 1 -TPDO1) << 8 )| 0x80) )
			{	
	
								 Current_Value        = RxMessage->Data[0] + (RxMessage->Data[1] << 8 );
								 Set_Current_Value(Current_Value);	
								 Speed_Value					= RxMessage->Data[2] + (RxMessage->Data[3] << 8 ) + (RxMessage->Data[4] <<16 ) + (RxMessage->Data[5] << 24 );	
    	}
	    else 
				return 0;	
     return 1;
							 
}

/************************************************
函数名称：Action_Set_Heartbeat_Tim
函数功能：设置心跳包时间
入口参数：drivernum-电机节点号.
           heratbeat_time-心跳包时间，单位ms，不大于65535
出口参数：无
备注：此项设置只能在驱动器状态为 pre-operational 时设置。
************************************************/
void Action_Set_Heartbeat_Tim(uint8_t drivernum,uint16_t heartbeat_time)
{
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x2b171000,0x64000000,		     //设置心跳包时间
						 };
  uint8_t transmit_mailbox;	                  //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId= SDOTX + drivernum;					 // standard identifier=0
	TxMessage.ExtId= SDOTX + drivernum;					 // extended identifier=StdId
	TxMessage.IDE  = CAN_Id_Standard ;			     // type of identifier for the message is Standard
	TxMessage.RTR  = CAN_RTR_Data  ;			       // the type of frame for the message that will be transmitted
	TxMessage.DLC  = DATA_LEN;

  data[0][1]=(((heartbeat_time>>24)&0xff))+(((heartbeat_time>>16)&0xff)<<8)+ (((heartbeat_time>>8)&0xff)<<16)+((heartbeat_time&0xff)<<24);	//低位在前							 
	
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
//	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us


}
/************************************************
函数名称：Action_Set_Operational_Mode
函数功能：切换驱动器工作状态为 operational
入口参数：drivernum-电机节点号
出口参数：无
备注：驱动器状态为operational时，才能接受位置、速度等命令。
************************************************/
void Action_Set_Operational_Mode(uint8_t drivernum)
{
	 uint8_t i=0; 
	 uint32_t data[1][2]={   		 
						   0x01000000,0x00000000,	 // 转换驱动器模式为工作模式
						 };
  uint8_t transmit_mailbox;	           //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=0x000;					     // standard identifier=0
	TxMessage.ExtId=0x000;					     // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			 // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			   // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;
  
	 data[0][0]=0x01000000+((drivernum<<16)&0xffffffff); 
    	
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us	

}
/************************************************
函数名称：Action_Enable_Driver
函数功能：使能电机，使能后电机处于抱死状态
入口参数：drivernum-电机节点号
           status   -ENABLE使能电机，
                     DISABLE不使能。
出口参数：无
备注：无
************************************************/
void Action_Enable_Driver(uint8_t driverNum,int8_t status)
{
	 uint8_t i=0; 
	 uint32_t data[1][2]={   		 
		           0x2b406000,0x0f000000         
						 };
  uint8_t transmit_mailbox;	                //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+driverNum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+driverNum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;
    	
  if(status == 1)
	{
	  data[0][1] = 0x0f000000;
	}else if(status == 0)
  {
	  data[0][1] = 0x0d000000;
	}	
						 
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us	
}
/************************************************
函数名称：Actin_Config_Torque
函数功能：位置模式初始化
入口参数：drivernum-电机节点号
           max_torque-最大力矩
出口参数：无
备注:CANOpen协议中传输数据是低位在前
************************************************/
void Actin_Config_Torque(uint8_t driverNum,int16_t max_torque)   	
{    
	 uint8_t i=0; 
	 uint32_t data[2][2]={    				 
						   0x2f606000,0x04000000,		// Index-6060(Mode of operation)
		           0x2b726000,0x00000000,   // Index-6072(Max Torque)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+driverNum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+driverNum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;
  data[1][1]=(((max_torque>>24)&0xff))+(((max_torque>>16)&0xff)<<8)+ (((max_torque>>8)&0xff)<<16)+((max_torque&0xff)<<24);	
						 
	for(i=0;i<2;i++)
	{	      	
			TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
			TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
			TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
			TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
								 
			TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
			TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
			TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
			TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
				
			transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
			while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
	}
}
/************************************************
函数名称：Action_Set_Torque
函数功能：给定力矩命令
入口参数：drivernum-电机节点号
           torque- 目标力矩
                 - 如果产生所需力矩的电流未2A，额定电流是3.2A，则torque = 2000mA * 1000 /3200mA =625
                 - 代表额定电流的 62.5%
出口参数：无
备注:CANOpen协议中传输数据是低位在前
************************************************/
void Action_Set_Torque( uint8_t driverNum,int16_t torque )                 
{      
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x2b716000,0x00000000,		// Index-6071(Target torque)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+driverNum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+driverNum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;
						 
  data[0][1]=(((torque>>24)&0xff))+(((torque>>16)&0xff)<<8)+ (((torque>>8)&0xff)<<16)+((torque&0xff)<<24);	
						 					 
	for(i=0;i<1;i++)
	{	     	
			TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
			TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
			TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
			TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
								 
			TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
			TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
			TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
			TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
						
			transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
			while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
	}
}
/************************************************
函数名称：Action_Config_Vel
函数功能：速度模式初始化
入口参数：drivernum-电机节点号
           acc-加速度，单位脉冲/s2
           dec-减速度，单位脉冲/s2
出口参数：无
备注:CANOpen协议中传输数据是低位在前
************************************************/
void Action_Config_Vel(uint8_t driverNum,uint32_t acc,uint32_t dec)            
{

	 uint8_t i=0; 
	 uint32_t data[3][2]={    				 
							0x2f606000,0x03000000,    //  Index-6060(Mode of operation) data-03(velocity mode)
							0x23836000,0x00000000,		//  Index-6083(Profile acceleration)
              0x23846000,0x00000000     //  Index-6084(Profile deceleration)
						 };
	 

  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+driverNum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+driverNum;					 // extended identifier=StdId
				
  		//	TxMessage.RTR = CAN_RTR_Remote;			  
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;


   data[1][1]=(((acc>>24)&0xff))+(((acc>>16)&0xff)<<8)+ (((acc>>8)&0xff)<<16)+((acc&0xff)<<24);		
   data[2][1]=(((dec>>24)&0xff))+(((dec>>16)&0xff)<<8)+ (((dec>>8)&0xff)<<16)+((dec&0xff)<<24);								 
		 
							 
	for(i=0;i<3;i++)
	{	    
 	
			TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
			TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
			TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
			TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
								 
			TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
			TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
			TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
			TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
				
			transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
			while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
	}
		
}
/************************************************
函数名称：Action_Set_Velocity
函数功能：给定速度命令
入口参数：drivernum-电机节点号
           vel-速度，单位脉冲/s
出口参数：无
备注:CANOpen协议中传输数据是低位在前
************************************************/
void Action_Set_Velocity( uint8_t driverNum,int32_t vel )                 
{      
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x23ff6000,0x00000000,		     // Index-60ff(Target velocity)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+driverNum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+driverNum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;


  data[0][1]=(((vel>>24)&0xff))+(((vel>>16)&0xff)<<8)+ (((vel>>8)&0xff)<<16)+((vel&0xff)<<24);								 
	for(i=0;i<1;i++)
	{	    
			TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
			TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
			TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
			TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
								 
			TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
			TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
			TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
			TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
				
			transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
			while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
	}
}
/************************************************
函数名称：Actin_Config_Pos
函数功能：位置模式初始化
入口参数：drivernum-电机节点号
           vel-速度，单位脉冲/s
           acc-加速度，单位脉冲/s2
           dec-减速度，单位脉冲/s2
出口参数：无
备注:CANOpen协议中传输数据是低位在前
************************************************/
void Actin_Config_Pos(uint8_t driverNum,int32_t vel,uint32_t acc,uint32_t dec)   	
{    
	 uint8_t i=0; 
	 uint32_t data[5][2]={    				 
						   0x2f606000,0x01000000,		// Index-6060(Mode of operation)
		           0x2b866000,0x00000000,   // Index-6086(Motion proflie type) data-0(trapezoidal profile mode)
		           0x23816000,0x00000000,   // Index-6081(Proflie velocity)
		           0x23836000,0x00000000,   // Index-6083(Profile acceleration)
		           0x23846000,0x00000000,   // Index-6084(Profile deceleration)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+driverNum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+driverNum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;
  data[2][1]=(((vel>>24)&0xff))+(((vel>>16)&0xff)<<8)+ (((vel>>8)&0xff)<<16)+((vel&0xff)<<24);	
  data[3][1]=(((acc>>24)&0xff))+(((acc>>16)&0xff)<<8)+ (((acc>>8)&0xff)<<16)+((acc&0xff)<<24);		
  data[4][1]=(((dec>>24)&0xff))+(((dec>>16)&0xff)<<8)+ (((dec>>8)&0xff)<<16)+((dec&0xff)<<24);								 
	for(i=0;i<5;i++)
	{	      	
			TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
			TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
			TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
			TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
								 
			TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
			TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
			TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
			TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
				
			transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
			while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
	}
}
/************************************************
函数名称：Action_Set_Position
函数功能：给定位置命令
入口参数：drivernum-电机节点号
           mode-ABSOLUATE_POSITION_MODE,绝对位置模式；
                RELATIVE_POSITION_MODE,相对位置模式
           pos-目标位置
出口参数：无
备注:CANOpen协议中传输数据是低位在前
************************************************/
void Action_Set_Position( uint8_t driverNum,uint8_t mode,int32_t pos )                 
{      
	 uint8_t i=0; 
	 uint32_t data[2][2]={    				 
						   0x237a6000,0x00000000,		// Index-607a(Target position)
               0x2b406000,0x00000000,   // Index-6040(Control word) data-3f(absoluate position mode)
		                                    //                          data-7f(relative  position mode)
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+driverNum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+driverNum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;
						 
  data[0][1]=(((pos>>24)&0xff))+(((pos>>16)&0xff)<<8)+ (((pos>>8)&0xff)<<16)+((pos&0xff)<<24);	
						 
  if(mode == ABSOLUATE_POSITION_MODE)
	{
     data[1][1]= 0x3f000000; 
	}else if(mode == RELATIVE_POSITION_MODE)
	{
     data[1][1]= 0x7f000000;	 	
	}

					 
	for(i=0;i<3;i++)
	{	     	
			TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
			TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
			TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
			TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
								 
			TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
			TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
			TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
			TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
				
			transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
			while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
	}
}
/************************************************
函数名称：Action_Offset_Home
函数功能：零点设置，用于绝对位置模式中,可设置当前点为零点
入口参数：drivernum-电机节点号
           position-零点位置,设置为0
出口参数：无
备注:CANOpen协议中传输数据是低位在前
************************************************/
void Action_Offset_Home(uint8_t driverNum,int32_t position)
{
	 uint8_t i=0; 
	 uint32_t data[3][2]={    				 
						   0x2f606000,0x06000000,		 //索引6060(MODE OF OPERATION)至HOME模式
		           0x237c6000,0x00000000,    //索引607c(HOMING OFFSET)
		           0x2f896000,0x00000000     //索引6089(HOMING METHOD) 0为Home is current position
						 };
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+driverNum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+driverNum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;
						 
  data[1][1]=(((position>>24)&0xff))+(((position>>16)&0xff)<<8)+ (((position>>8)&0xff)<<16)+((position&0xff)<<24);		
						 
	for(i=0;i<3;i++)
	{	    	
			TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
			TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
			TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
			TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
								 
			TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
			TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
			TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
			TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
				
			transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
			while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
	}
   	
}
/************************************************
函数名称：Action_Get_Motorstatus
函数功能：向驱动器发送获取电流信息命令
入口参数：drivernum-电机节点号
           status   -can be CURRENT
                            VELOCITY
                            ABSOLUATE_POSITION
                            RELATIVE_POSITION
出口参数：无
备注:CANOpen协议中传输数据是低位在前，对传输的数据做了处理
************************************************/
void Action_Get_Motorstatus(uint8_t driverNum,uint16_t status)
{
	 uint8_t i=0; 
	 uint32_t data[1][2]={    				 
						   0x0000000,0x000000,		       // Index-6078(Current actual value)
						 };                              // Index-6064(Position actual value-increments)
	                                           // Index-6063(Position actual value)
						                                 // Index-6069(Velocity snesor actual value)
  uint8_t transmit_mailbox;	                 //transmit_mailbox: the number of the mailbox that is used for transmission.
	CanTxMsg TxMessage;
	TxMessage.StdId=SDOTX+driverNum;					 // standard identifier=0
	TxMessage.ExtId=SDOTX+driverNum;					 // extended identifier=StdId
	TxMessage.IDE=CAN_Id_Standard ;			       // type of identifier for the message is Standard
	TxMessage.RTR=CAN_RTR_Data  ;			         // the type of frame for the message that will be transmitted
	TxMessage.DLC=DATA_LEN;
						 
	switch(status)
	{
		case CURRENT:
			data[0][0]=0x40786000;
			break;
		case VELOCITY:
			data[0][0]=0x40696000;			
			break;
		case RELATIVE_POSITION:
			data[0][0]=0x40636000;
			break;
		case ABSOLUATE_POSITION:
			data[0][0]=0x40646000;
			break;
	}		
	
	TxMessage.Data[0] = (*(unsigned long*)&data[i][0]>>24)&0xff;
	TxMessage.Data[1] = (*(unsigned long*)&data[i][0]>>16)&0xff;
	TxMessage.Data[2] = (*(unsigned long*)&data[i][0]>>8)&0xff;
	TxMessage.Data[3] =  *(unsigned long*)&data[i][0]&0xff;
						 
	TxMessage.Data[4] = (*(unsigned long*)&data[i][1]>>24)&0xff;
	TxMessage.Data[5] = (*(unsigned long*)&data[i][1]>>16)&0xff;
	TxMessage.Data[6] = (*(unsigned long*)&data[i][1]>>8)&0xff;
	TxMessage.Data[7] = *(unsigned long*)&data[i][1]&0xff;
		
	transmit_mailbox= CAN_Transmit(CAN1, &TxMessage);         //1.4us	
	while((CAN_TransmitStatus(CAN1, transmit_mailbox)!= CAN_TxStatus_Ok));//等待238us
}

/************************************************
函数名称：Action_Check_Error
函数功能：接收驱动器返回的错误信息
入口参数：RxMessage-CAN接收的一帧数据
           drivernum-电机节点号
出口参数：1-有错误
           0-没有错误
备注:CANOpen协议中传输数据是低位在前
************************************************/
uint8_t Action_Check_Error(CanRxMsg* RxMessage,uint8_t drive_num){
	    static uint16_t Index;
	    static uint8_t  Subindex,SDO; 	
	
			SDO      = RxMessage->Data[3] & 0xff;
			Index    =(RxMessage->Data[1]<<8 &0xffff)  + (RxMessage->Data[2] &0xffff);
		  Subindex = RxMessage->Data[0] & 0Xff;    	
	    Subindex = Subindex;
			if((RxMessage->StdId - drive_num ) == 0x580 )
			{			
				switch(SDO)
				{
					case SDO_LOAD_1:
							 switch(Index)
							 {
								 case COMMAND_SPECIFIER_NOT_VALID:
										 sdo_error.command_specifier_not_valid=1; //此命令无效
								     return 1;
							 }		
						break;
					case SDO_LOAD_4:
						break;				
				}
    	}
	    else return 0;	
			return 1;
}


/************************************************
函数名称：Action_Get_Motor_Feedback
函数功能：从驱动器接收各项状态量（速度、电流、位置）
入口参数：RxMessage-CAN接收的一帧数据，drivernum-电机节点号
出口参数：无
备注:CANOpen协议中传输数据是低位在前，对传输的数据做了处理
************************************************/
uint16_t Action_Get_Motor_Feedback(CanRxMsg* RxMessage,uint8_t drive_num ){
	    static int32_t Speed_Value=0;
	    static int32_t AbsolutePosition_Value=0;
	    static int32_t RelativePosition_Value=0;	
     	static int32_t Current_Value=0;	
	    static uint16_t Index;
	    static uint8_t  Subindex,SDO; 	
	
			SDO      = RxMessage->Data[0] & 0xff;
			Index    =(RxMessage->Data[2]<<8 &0xffff)  + (RxMessage->Data[1] &0xffff);
		  Subindex = RxMessage->Data[3] & 0Xff;  
      Subindex = Subindex;	
			if((RxMessage->StdId - drive_num ) == 0x580 )
			{	
	
				switch(SDO)
				{
					case SDO_LOAD_4:
						switch(Index)
						{
							case POSITION_ACTUAL_VALUE://绝对
								 AbsolutePosition_Value = RxMessage->Data[4] + (RxMessage->Data[5] << 8 ) + (RxMessage->Data[6] <<16 ) + (RxMessage->Data[7] << 24 );
							   Set_AbsolutePosition_Value(AbsolutePosition_Value); 								
								 return POSITION_ACTUAL_VALUE;
							case ACTUAL_POSITION_INTERNAL_UNIT://相对位置
								 RelativePosition_Value = RxMessage->Data[4] + (RxMessage->Data[5] << 8 ) + (RxMessage->Data[6] <<16 ) + (RxMessage->Data[7] << 24 );
								 Set_RelativePosition_Value(RelativePosition_Value);  
								 return ACTUAL_POSITION_INTERNAL_UNIT;					
							case VELOCITY_SENSOR_ACTUAL_VALUE://速度
								 Speed_Value           = RxMessage->Data[4] + (RxMessage->Data[5] << 8 ) + (RxMessage->Data[6] <<16 ) + (RxMessage->Data[7] << 24 );
									Set_Speed_Value(Speed_Value);	
									return 	VELOCITY_SENSOR_ACTUAL_VALUE;							
							case CURRENT_ACTUAL_VALUE://电流
								 Current_Value        = RxMessage->Data[4] + (RxMessage->Data[5] << 8 ) + (RxMessage->Data[6] <<16 ) + (RxMessage->Data[7] << 24 );
								 Set_Current_Value(Current_Value);	
								 return 	CURRENT_ACTUAL_VALUE;							
						}
						break;				
				}
    	}
	    else 
				return 0;	
     return 1;
}
int Speed_Value=0;
void Set_Speed_Value(int tmp){
  Speed_Value=tmp;
}
int32_t Get_Speed_Value(void){
  return Speed_Value;
}
int AbsolutePosition_Value=0;
void Set_AbsolutePosition_Value(int tmp){
   AbsolutePosition_Value=tmp;
}
int32_t Get_AbsolutePosition_Value(void){
  return AbsolutePosition_Value;
}
int RelativePosition_Value=0;
void Set_RelativePosition_Value(int tmp){
   RelativePosition_Value=tmp;
}
int32_t Get_RelativePosition_Value(void){
  return RelativePosition_Value;
}

int Current_Value=0;
void Set_Current_Value(int tmp){
   Current_Value=tmp;
}
int32_t Get_Current_Value(void){
  return Current_Value;
}
