/*
 * console_controller.h
 *
 *  Created on: Feb 14, 2025
 *      Author: knap-linux
 */

#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#include <unordered_map>
#include <string>
#include <functional>

#define CONS_VERSION "0.1"

#define CONS_DEBUG 0

#define CONS_COLS 80
#define CONS_ROWS 24

class Console
{
public:
//	using CommandFunction = std::function<void(const std::string&)>;
	using CommandFuncion = void(*)(const std::string&);
	using SendFunction = std::function<void(const std::string&)>;
	using RecieveFuncion = std::function<std::string()>;

	Console(SendFunction send, RecieveFuncion recieve);
	virtual ~Console();

	void registerCommand(const std::string &command, CommandFuncion function);
	void splash();
	void run();

private:
	void setCursor(const uint32_t x, const uint32_t y);
	void clear();
	void verticalLine();

	SendFunction m_send;
	RecieveFuncion m_recieve;
	std::unordered_map<std::string, CommandFuncion> m_commands;
};


#endif /* INC_CONSOLE_H_ */
