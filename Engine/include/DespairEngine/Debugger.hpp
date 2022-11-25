#ifndef ENGINE_DEBUGGER_HPP
#define ENGINE_DEBUGGER_HPP

#include <spdlog/spdlog.h>

namespace engine
{
#ifndef NDEBUG
#define DEBUG_INFO(...)		spdlog::info(__VA_ARGS__)
#define DEBUG_WARNING(...)	spdlog::warn(__VA_ARGS__)
#define DEBUG_ERROR(...)	spdlog::error(__VA_ARGS__)
#define DEBUG_CRITICAL(...)	spdlog::critical(__VA_ARGS__)
#else
#define DEBUG_INFO(...)
#define DEBUG_WARNING(...)
#define DEBUG_ERROR(...)
#define DEBUG_CRITICAL(...)
#endif // NDEBUG
}

#endif // !ENGINE_DEBUGGER_HPP