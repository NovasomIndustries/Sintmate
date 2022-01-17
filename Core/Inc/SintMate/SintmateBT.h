/*
 * SintmateBT.h
 *
 *  Created on: May 20, 2021
 *      Author: fil
 */

#ifndef INC_SINTMATEBT_H_
#define INC_SINTMATEBT_H_

#define	BT_START_FLAG		'<'
#define	BT_END_FLAG			'>'
#define	BT_TX_SIZE			64
#define	BT_RX_SIZE			16

extern	uint8_t	BT_RxBuf[16], BT_RxBufIndex , BT_TxBuf[64], BT_TxBufIndex,Bt_BytesNum,Bt_PacketReady;
extern	uint32_t	BT_Cmd,BT_Param;

extern	void BT_UART_Callback(UART_HandleTypeDef *huart);
extern	uint32_t BT_PacketParser(void);
extern	uint32_t SintMateBtProcess(void);
extern	void BT_CheckForErrors(uint32_t error_index);
extern	void Bt_SintMateLogInsert(uint32_t speed , uint32_t time , char action);
extern	void Bt_SintMateLogDump(void);

#endif /* INC_SINTMATEBT_H_ */
