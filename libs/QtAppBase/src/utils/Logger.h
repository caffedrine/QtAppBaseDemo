#ifndef _BASE_LOGGER_H_
#define _BASE_LOGGER_H_

#include <QPlainTextEdit>
#include <spdlog/spdlog.h>

namespace base
{
	/* Logger instance - multi-threaded */
	extern spdlog::logger *logger;

	/* Logger viewer inside application */
	extern QPlainTextEdit *loggerContentGui;

	void setup_logger(bool consoleMode);
}
#endif // _BASE_LOGGER_H_