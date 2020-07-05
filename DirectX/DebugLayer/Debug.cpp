#include "Debug.h"
#include "DebugUtility.h"
#include "Log.h"

void Debug::windowMessage(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::windowMessage(message);
#endif // _DEBUG
}

void Debug::log(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::log()->log(message);
#endif // _DEBUG
}

void Debug::logError(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::log()->logError(message);
#endif // _DEBUG
}

void Debug::logWarning(const std::string& message) {
#ifdef _DEBUG
    DebugUtility::log()->logWarning(message);
#endif // _DEBUG
}
