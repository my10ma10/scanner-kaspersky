#include "logger.hpp"

void Logger::setLogFile(const std::string& filepath) {
    file.open(filepath, std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл лога!");
    }
}

bool Logger::createLog(
    const std::string& filePath,
    const std::string& hash,
    const std::string& verdict
    )
{
    std::lock_guard<std::mutex> lock(mtx);
    if (file.is_open()) {
        file << "Путь: " << filePath << std::endl;
        file << "Хэш: " << hash << std::endl;
        file << "Вердикт: " << verdict << std::endl;
        return true;
    }
    std::cout << "Ошибка записи в лог!" << std::endl;
    return false;
}