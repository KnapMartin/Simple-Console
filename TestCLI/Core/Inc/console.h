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

#define CONS_VERSION "0.3"

constexpr uint32_t CONS_COLS{80};
constexpr uint32_t CONS_ROWS{24};
constexpr uint32_t CONS_MAX_COMMANDS{10};

class Console
{
public:
	using CommandFunction = std::function<void(const std::string&)>;
	using SendFunction = std::function<void(const std::string&)>;
	using RecieveFuncion = std::function<std::string()>;

	Console(SendFunction send, RecieveFuncion recieve);
	virtual ~Console();

	enum class State
	{
		None = -1,
		Ok = 0,
		ErrorRegisterFull,
		ErrorInvalidCommand,
		ErrorXPos,
		ErrorYPos,
	};

	State registerCommand(const std::string &command, CommandFunction function, const uint32_t matchLength = 0);
	State splash();
	State run();
	State printRegistered();

private:
	State setCursor(const uint32_t x, const uint32_t y);
	State clear();
	State verticalLine();

	SendFunction m_send;
	RecieveFuncion m_recieve;
	std::array< std::tuple<std::string, CommandFunction, uint32_t>, CONS_MAX_COMMANDS > m_commands;
	uint32_t m_commandCount;
};


#endif /* INC_CONSOLE_H_ */
