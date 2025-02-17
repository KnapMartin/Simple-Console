/*
 * uart_controller.h
 *
 *  Created on: Jan 7, 2025
 *      Author: knap-linux
 */

#ifndef UART_CONTROLLER_H_
#define UART_CONTROLLER_H_

#include "main.h"
#include "cmsis_os.h"
#include <string>

class UartController
{
public:
	UartController();
	virtual ~UartController();

	void setHandleUart(UART_HandleTypeDef *huart);
	void setHandleSemTx(osSemaphoreId_t *semTx);
	void setHandleQueueRx(osMessageQueueId_t *queueRx);
	void setHandleMutexTx(osMutexId_t *mutexTx);

	bool init();
	bool send(std::string data);
//	bool send(char data[], uint32_t length);
	std::string receive();

	bool updateInterruptRx(UART_HandleTypeDef *huart);
	bool updateInterruptTx(UART_HandleTypeDef *huart);

	bool operator<<(const std::string& str);
	bool operator<<(const char* str);

	void printout(bool state);

private:
	uint8_t m_rxChar;
	UART_HandleTypeDef *m_huart;
	osSemaphoreId_t *m_semTx;
	osMessageQueueId_t *m_queueRx;
	osMutexId_t *m_mutexTx;
	bool m_printout;

};

#endif /* UART_CONTROLLER_H_ */
