#ifndef __LOG_H_
#define __LOG_H_

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_DEBUG_ON
#define SPDLOG_TRACE_ON

#include <spdlog/spdlog.h>
#include <memory.h>

class Log {
public:
    static void init();
    inline static std::shared_ptr<spdlog::logger>& getLogger() { return logger; }

private:
    static std::shared_ptr<spdlog::logger> logger;
};

#define TRACE(...) SPDLOG_LOGGER_TRACE(Log::getLogger(), __VA_ARGS__)
#define DEBUG(...) SPDLOG_LOGGER_DEBUG(Log::getLogger(), __VA_ARGS__)
#define INFO(...) SPDLOG_LOGGER_INFO(Log::getLogger(), __VA_ARGS__)
#define WARN(...) SPDLOG_LOGGER_WARN(Log::getLogger(), __VA_ARGS__)
#define ERROR(...) SPDLOG_LOGGER_ERROR(Log::getLogger(), __VA_ARGS__)
#define CRITICAL(...) SPDLOG_LOGGER_CRITICAL(Log::getLogger(), __VA_ARGS__)

#endif // __LOG_H_
