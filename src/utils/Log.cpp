#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Log::logger;

void Log::init()
{
    spdlog::set_pattern("%^[%T] %s:%!:%#: %v%$");
    logger = spdlog::stdout_color_mt("Logger");
    logger->set_level(spdlog::level::trace);
}
