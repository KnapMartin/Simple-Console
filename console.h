/*
 * console_controller.h
 *
 *  Created on: Feb 14, 2025
 *      Author: knap-linux
 */

#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#include "uart_controller.h"
#include <unordered_map>
#include <string>

#define CONS_DEBUG 0

#define CONS_COLS 80
#define CONS_ROWS 24

class Console
{
public:
	using CommandFuncion = void(*)(const std::string&);

	Console(UartController &uart);
	virtual ~Console();

	void registerCommand(const std::string &command, CommandFuncion function);
	void splash();
	void run();

private:
	void setCursor(const uint32_t x, const uint32_t y);
	void clear();
	void verticalLine();

	UartController &m_uart;
	std::unordered_map<std::string, CommandFuncion> m_commands;
};


#endif /* INC_CONSOLE_H_ */
