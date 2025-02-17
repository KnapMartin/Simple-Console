
#include "console.h"

#include <string.h>

Console::Console(SendFunction send, RecieveFuncion recieve)
	: m_send(send)
	, m_recieve(recieve)
	, m_commands{}
	, m_commandCount{0}
{
}

Console::~Console()
{
}

void Console::registerCommand(const std::string &command,
		CommandFunction function, const uint32_t matchLength)
{
	if (m_commandCount < CONS_MAX_COMMANDS)
	{
		m_commands[m_commandCount] = std::make_tuple(command, function, matchLength);
		++m_commandCount;
	}
	else
	{
		return;
	}
}

void Console::splash()
{
	this->clear();
	this->verticalLine();
	this->verticalLine();
	this->setCursor(32, 0);
	m_send("Test Console v");
	m_send(CONS_VERSION);
	m_send("\r\n");
	this->verticalLine();
	this->verticalLine();
}

void Console::run()
{
	m_send("> ");

	const char* commandRec = m_recieve().c_str();
	uint32_t missingMatchCtr{0};

	for (uint32_t commIdx{0}; commIdx < m_commandCount; ++commIdx)
	{
		uint32_t matchLength = std::get<2>(m_commands[commIdx]);
		const char* commandReg = std::get<0>(m_commands[commIdx]).c_str();

		if (!matchLength)
		{
			if (!strcmp(commandRec, commandReg))
			{
				std::get<1>(m_commands[commIdx])(commandRec);
			}
			else
			{
				++missingMatchCtr;
			}
		}
		else
		{
			if (!strncmp(commandRec, commandReg, matchLength))
			{
				std::get<1>(m_commands[commIdx])(commandRec);
			}
			else
			{
				++missingMatchCtr;
			}
		}
	}

	if (missingMatchCtr == m_commandCount)
	{
		m_send("Invalid command.\r\n");
	}
}

void Console::clear()
{
	for (uint32_t row{0}; row < CONS_ROWS; ++row)
	{
		m_send("\r\n");
	}
}

void Console::setCursor(const uint32_t x, const uint32_t y)
{
	if (x > CONS_COLS) return;
	if (y > CONS_ROWS) return;

	for (uint32_t col{0}; col < x; ++col)
	{
		m_send(" ");
	}

	for (uint32_t row{0}; row < y; ++row)
	{
		m_send("\n");
	}
}

void Console::printRegistered()
{
	m_send("Registered commands:\r\n");
	for (uint32_t commIdx{0}; commIdx < m_commandCount; ++commIdx)
	{
		std::string comm = std::get<0>(m_commands[commIdx]);
		m_send(comm);
		m_send("\n");
	}
}

void Console::verticalLine()
{
	for (uint32_t col{0}; col < CONS_COLS; ++col)
	{
		m_send("-");
	}
	m_send("\r\n");
}
