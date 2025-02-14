
#include "console.h"

#include <string.h>

Console::Console(UartController &uart)
	: m_uart{uart}
	, m_commands{}
{
}

Console::~Console()
{
}

void Console::registerCommand(const std::string &command,
		CommandFuncion function)
{
	m_commands[command] = function;

#if UART_CONSOLE_DEBUG == 1
	m_uart.send("Registered commands: \n");
	for (auto comm : m_commands)
	{
		m_uart.send(comm.first);
	}
#endif
}

void Console::splash()
{
	this->clear();
	this->verticalLine();
	this->verticalLine();
	this->setCursor(32, 0);
	m_uart.send("Test Console v0.0\r\n");
	this->verticalLine();
	this->verticalLine();
}

void Console::run()
{
	m_uart.send("> ");
	std::string command = m_uart.receive(true);

	uint32_t noMatchCtr{0};

	for (auto comm : m_commands)
	{
		if (!strcmp(comm.first.c_str(), command.c_str()))
		{
			CommandFuncion func = comm.second;
			func(command);
		}
		else
		{
			++noMatchCtr;
		}
	}

	if (noMatchCtr == m_commands.size())
	{
		m_uart.send("Invalid command.\r\n");
	}
}

void Console::clear()
{
	for (uint32_t row{0}; row < CONS_ROWS; ++row)
	{
		m_uart.send("\r\n");
	}
}

void Console::setCursor(const uint32_t x, const uint32_t y)
{
	if (x > CONS_COLS) return;
	if (y > CONS_ROWS) return;

	for (uint32_t col{0}; col < x; ++col)
	{
		m_uart.send(" ");
	}

	for (uint32_t row{0}; row < y; ++row)
	{
		m_uart.send("\n");
	}
}

void Console::verticalLine()
{
	for (uint32_t col{0}; col < CONS_COLS; ++col)
	{
		m_uart.send("-");
	}
	m_uart.send("\r\n");
}
