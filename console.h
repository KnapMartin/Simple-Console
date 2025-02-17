/*
 * console_controller.h
 *
 *  Created on: Feb 14, 2025
 *      Author: knap-linux
 */

#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

#include <string>
#include <functional>
#include <array>
#include <tuple>

#define CONS_VERSION "0.2"

#define CONS_DEBUG 1

#define CONS_COLS 80
#define CONS_ROWS 24

constexpr size_t CONS_MAX_COMMANDS{10};

class Console
{
public:
//	using CommandFunction = std::function<void(const std::string&)>;
	using CommandFuncion = void(*)(const std::string&);
	using SendFunction = std::function<void(const std::string&)>;
	using RecieveFuncion = std::function<std::string()>;

	Console(SendFunction send, RecieveFuncion recieve);
	virtual ~Console();

	void registerCommand(const std::string &command, CommandFuncion function, const uint32_t matchLength = 0);
	void splash();
	void run();
	void printRegistered();

private:
	void setCursor(const uint32_t x, const uint32_t y);
	void clear();
	void verticalLine();

	SendFunction m_send;
	RecieveFuncion m_recieve;
	std::array< std::tuple<std::string, CommandFuncion, uint32_t>, CONS_MAX_COMMANDS > m_commands;
	uint32_t m_commandCount;
};


#endif /* INC_CONSOLE_H_ */
