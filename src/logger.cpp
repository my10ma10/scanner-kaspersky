#include "logger.hpp"

std::unique_ptr<Logger> Logger::instance = nullptr;
std::ofstream Logger::file;
std::mutex Logger::mtx;

Logger& Logger::getInstance() {
    std::scoped_lock lock(mtx);
    if (!instance) {
        instance.reset(new Logger());
    }
    return *instance;
}

void Logger::setLogFile(const std::string& filepath) {
    file.open(filepath, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл лога!" << std::endl;
    }
}

bool Logger::createLog(
    const std::string& filePath,
    const std::string& hash,
    const std::string& verdict
    )
{
    if (file.is_open()) {
        file << "Путь: " << filePath << std::endl;
        file << "Хэш: " << hash << std::endl;
        file << "Вердикт: " << verdict << std::endl;
        return true;
    }
    std::cout << "Ошибка записи в лог!" << std::endl;
    return false;
}