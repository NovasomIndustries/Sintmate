/*
 * SintmateBT.c
 *
 *  Created on: May 20, 2021
 *      Author: fil
 */

#include "main.h"
uint8_t	BT_RxBuf[BT_RX_SIZE], BT_RxBufIndex=0 , BT_TxBuf[BT_TX_SIZE], Bt_PacketReady=0;
uint8_t	start_flag=0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
uint8_t	rxchar;
	rxchar = (uint8_t ) READ_REG(huart->Instance->RDR);
	if ( start_flag == 0 )
	{
		if ( rxchar == BT_START_FLAG)
		{
			start_flag = 1;
			SystemVar.bt_rxbuf_index=0;
		}
	}
	else
	{
		if ( rxchar == BT_END_FLAG)
		{
			BT_RxBuf[SystemVar.bt_rxbuf_index] = rxchar;
			SystemVar.bt_packet_ready = 1;
			SystemVar.bt_rxbuf_index=0;
			start_flag = 0;
			return; /* don't restart rx until packet is processed */
		}
	}
	BT_RxBuf[SystemVar.bt_rxbuf_index] = rxchar;
	SystemVar.bt_rxbuf_index++;
	if ( SystemVar.bt_rxbuf_index >= BT_RX_SIZE)
		SystemVar.bt_rxbuf_index=0;
	HAL_UART_Receive_IT(&huart7, BT_RxBuf, 1);
}


/*
 * commands
 * c = number of lines in dump
 * d = dump status, line number to dump
 * s = set speed , param follow
 * t = set time , param follow
 * g = go, no param
 * h = halt, no param
 * r = restart from stored speed, no param
 * packet format is
 * "< CMD PARAM >"
 */

uint32_t BT_PacketParser(void)
{
uint8_t pnum,startflag;
uint32_t	ret_val=0;

	pnum = sscanf((char *)BT_RxBuf,"%c %c %d",(char *)&startflag,(char *)&SystemVar.bt_cmd,(int *)&SystemVar.bt_param);
	if (( pnum == 2 ) || ( pnum == 3 ))
		ret_val=0;
	else
		ret_val=1;
	return ret_val;
}

uint32_t SintMateBtProcess(void)
/*
 * returns
 * 		0 if ok
 * 		1 if time > max time
 * 		2 if speed > max speed
 */
{
uint32_t	ret_val=0;

	switch(SystemVar.run_state)
	{
	case	RUN_STATE_RUNNING	:
		if (SystemVar.bt_cmd == 'h')
		{
			SystemVar.run_state = RUN_STATE_IDLE;
			SintMate_stop_rotations();
			DrawIdleButtons();
		}
		break;
	case	RUN_STATE_IDLE	:
		if (SystemVar.bt_cmd == 'g')
		{
			SintMate_do_rotations(((OC_PULSES_PER_ROTATION*SystemParameters.step_rpm)/60)*SystemVar.DownCounter);
			SystemVar.run_state = RUN_STATE_RUNNING;
			DrawPlayButtons();
		}
		if (SystemVar.bt_cmd == 't')
		{
			if ( SystemVar.bt_param < SystemParameters.max_running_time )
			{
				SystemVar.DownCounter = SystemVar.bt_param;
				SetCounter(SystemVar.DownCounter);
				SystemVar.Session_DownCounter = SystemVar.DownCounter;
			}
			else
				ret_val = 1;
		}
		if (SystemVar.bt_cmd == 'r')
		{
			SystemVar.DownCounter = SystemVar.Session_DownCounter ;
			SetCounter(SystemVar.DownCounter);
		}
		if (SystemVar.bt_cmd == 's')
		{
			if ( SystemVar.bt_param < MAX_STEP_SPEED_RPM )
				SystemParameters.step_rpm = SystemVar.bt_param;
			else
				ret_val = 2;
		}
		if (SystemVar.bt_cmd == 'c')
		{
			SystemVar.DownCounter = SystemVar.Session_DownCounter ;
			SetCounter(SystemVar.DownCounter);
		}

		if (SystemVar.bt_cmd == 'd')
		{
			SystemVar.bt_tx_queue_not_empty = 1;
		}

		break;
	}
	return ret_val;
}

void BT_CheckForErrors(uint32_t error_index)
{
	bzero(BT_TxBuf,BT_TX_SIZE);
	if ( error_index == 0 )
		sprintf((char *)BT_TxBuf,"< OK %c %d >\n\r",(char )SystemVar.bt_cmd,(int )SystemVar.bt_param);
	if ( error_index == 1 )
		sprintf((char *)BT_TxBuf,"< ERROR TIME ( %d ) > MAX_TIME ( %d) >\n\r",(int )SystemVar.bt_param,(int )SystemParameters.max_running_time);
	if ( error_index == 2 )
		sprintf((char *)BT_TxBuf,"< ERROR SPEED ( %d ) > MAX_SPEED ( %d) >\n\r",(int )SystemVar.bt_param,(int )MAX_STEP_SPEED_RPM);
	HAL_UART_Transmit_IT(&huart7, BT_TxBuf, strlen((char *)BT_TxBuf));
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	SystemVar.bt_tx_complete = 1;
}


void Bt_SintMateLogInsert(uint32_t speed , uint32_t time , char action)
{
	SystemLogs.counter &= (LOG_MAX_DEPTH-1);
	SystemLogs.speed[SystemLogs.counter] = speed;
	SystemLogs.time[SystemLogs.counter] = time;
	SystemLogs.action[SystemLogs.counter] = action;
	SystemLogs.counter ++;
}

void Bt_SintMateLogDump(void)
/*
 * sent : current line
 * action : R=run , H=halt
 * systemspeed , systemtime
 */
{
	if ( SystemLogs.sent !=  SystemLogs.counter )
	{
		if ( 	SystemVar.bt_tx_complete == 1 )
		{
			sprintf((char *)BT_TxBuf,"< %d %c %d %d >\n\r",(int )SystemLogs.sent ,(int )SystemLogs.action[SystemLogs.sent],(int )SystemLogs.speed[SystemLogs.sent],(int )SystemLogs.time[SystemLogs.sent]);
			SystemVar.bt_tx_complete = 0;
			HAL_UART_Transmit_IT(&huart7, BT_TxBuf, strlen((char *)BT_TxBuf));
			SystemLogs.sent++;
			SystemLogs.sent &= (LOG_MAX_DEPTH-1);
			SystemVar.bt_tx_queue_not_empty = 1;
		}
	}
	else
		SystemVar.bt_tx_queue_not_empty = 0;

}


