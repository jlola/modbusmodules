#include "Queue.h"
#include "Random.h"
#include "USART2.h"
#include "IOPin.h"
#include "RS485.h"
//#include "Timer.h"
#include "Timer3.h"
#include "slave-rtu.h"
#include "ModbusSettings.h"
#include "DeviceFunctions.h"
#include "OneWireThread.h"
#include "OneWireManager.h"
#include "string.h"
#include <stm32f0xx_tim.h>
#include "IWDG/Watchdog.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

#define RS485_SPEED 115200
char buffer[BUFFER_SIZE];

int main(void)
{

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	Watchdog::Start();

	USARTBase* usart1 = CUSART1::Instance();
	#ifdef TIMER_CONTROL
	TimerInit();
	#endif
	Timer3::Instance()->Init();
	usart1->Init(RS485_SPEED);

	OneWireThread owthread(Timer3::Instance(),OW_PORT,OW_PIN,OW_PIN_SOURCE);

	//pin led control
	IOPin pa5(GPIOA,GPIO_Pin_5,IOOutput);
	pa5.Init();

//	//pin for receive enabled driving
	IOPin pa0(GPIOA,GPIO_Pin_0,IOOutput);
	pa0.Init();
	pa0.Set(false);
	RS485 rs485(usart1, NULL,&pa0, 4*1000000/RS485_SPEED,&pa5);
	RS485SetInstance(&rs485);

	SlaveRtu slave(rs485,1);

	char pdata;
	size_t i = 0;
	//dev=0, io=100
	DeviceFunctions dev;
	//owManager=10, owDevices=15
	OneWireManager owmanager(&owthread,&slave);

	dev.Init(&slave,&owmanager);

	owmanager.StartScan();
	rs485.RecEnable(true);
    while(1)
    {
    	owmanager.Refresh();
    	dev.Process();
    	//rs485.ClearFlags();
    	if (rs485.PacketCompleted())
    	{
    		//send_command(0x05/* some interrupt ID */, m);
    		//read packet
    		while(rs485.Read(pdata))
    		{
    			if (i<BUFFER_SIZE) {
    				buffer[i++] = pdata;
    			}
    		}
    		Watchdog::ResetCounter();

    		slave.handler(buffer,i);
    		i = 0;
    		memset(buffer,0x00,sizeof(buffer));
    		rs485.RecEnable(true);
    	}
    	//int rnd = getTrueRandomNumber();
    }
}
