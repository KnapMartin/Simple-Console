/*
 * uart_controller.cpp
 *
 *  Created on: Jan 7, 2025
 *      Author: knap-linux
 */

#include "uart_controller.h"

constexpr char rxMessageDelimiter
{ '\r' };

UartController::UartController() :
		m_rxChar
		{ }, m_huart(nullptr), m_semTx
		{ nullptr }, m_queueRx
		{ nullptr }, m_mutexTx
		{ nullptr }
		, m_printout{false}
{

}

UartController::~UartController()
{
}

void UartController::setHandleUart(UART_HandleTypeDef *huart)
{
	m_huart = huart;
}

void UartController::setHandleSemTx(osSemaphoreId_t *semTx)
{
	m_semTx = semTx;
}

void UartController::setHandleQueueRx(osMessageQueueId_t *queueRx)
{
	m_queueRx = queueRx;
}

void UartController::setHandleMutexTx(osMutexId_t *mutexTx)
{
	m_mutexTx = mutexTx;
}

bool UartController::init()
{
	if (m_huart == nullptr)
		return false;
	if (m_semTx == nullptr)
		return false;
	if (m_queueRx == nullptr)
		return false;
	if (m_mutexTx == nullptr)
		return false;

	if (HAL_UART_Receive_IT(m_huart, &m_rxChar, 1) != HAL_OK)
	{
		return false;
	}

	return true;
}

bool UartController::send(std::string data)
{
	if (osMutexAcquire(*m_mutexTx, osWaitForever) != osOK)
	{
		return false;
	}
	if (HAL_UART_Transmit_IT(m_huart, (uint8_t*) data.c_str(), data.length())
			!= HAL_OK)
	{
		osMutexRelease(*m_mutexTx);
		return false;
	}
	if (osSemaphoreAcquire(*m_semTx, osWaitForever) != osOK)
	{
		return false;
	}
	if (osMutexRelease(*m_mutexTx) != osOK)
	{
		return false;
	}

	return true;
}

//bool UartController::send(char data[], uint32_t length)
//{
//	if (osMutexAcquire(*m_mutexTx, osWaitForever) != osOK)
//	{
//		return false;
//	}
//	if (HAL_UART_Transmit_IT(m_huart, (uint8_t*) data, length) != HAL_OK)
//	{
//		return false;
//	}
//	if (osSemaphoreAcquire(*m_semTx, osWaitForever) != osOK)
//	{
//		return false;
//	}
//	if (osMutexRelease(*m_mutexTx) != osOK)
//	{
//		return false;
//	}
//
//	return true;
//}

std::string UartController::receive()
{
	char rxChar;
	std::string result;

	while (1)
	{
		if (osMessageQueueGet(*m_queueRx, &rxChar, nullptr, osWaitForever)
				!= osOK)
		{
			return "";
		}

		if (m_printout)
		{
			char buff[1];
			buff[0] = rxChar;
			this->send(buff);
		}

		result += rxChar;
		if (rxChar == rxMessageDelimiter)
		{
			break;
		}
	}

	if (m_printout)
	{
		this->send("\n");
	}

	return result;
}

bool UartController::updateInterruptRx(UART_HandleTypeDef *huart)
{
	if (huart->Instance == m_huart->Instance)
	{
		if (osMessageQueuePut(*m_queueRx, &m_rxChar, 0, 0) != osOK)
		{
			return false;
		}
		if (HAL_UART_Receive_IT(m_huart, &m_rxChar, 1) != HAL_OK)
		{
			return false;
		}
	}

	return true;
}

bool UartController::updateInterruptTx(UART_HandleTypeDef *huart)
{
	if (huart->Instance == m_huart->Instance)
	{
		if (osSemaphoreRelease(*m_semTx) != osOK)
		{
			return false;
		}
	}

	return true;
}

bool UartController::operator <<(const std::string &str)
{
	return this->send(str);
}

bool UartController::operator <<(const char *str)
{
	return this->send(std::string(str));
}

void UartController::printout(bool state)
{
	m_printout = state;
}
