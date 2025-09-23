#include "utility.hpp"

Utility::Utility(const std::string& path, const std::string& base, const std::string& logfile) 
    : travelsal(path)
    {}

void Utility::execute() {
    travelsal.execute();
}

void Utility::printReport() const {
    std::cout << "Обработано файлов: " << travelsal.getProcessedCount() << std::endl;
    std::cout << "Из них вредоносных: " << parser.getCoincidencesCount() << std::endl;
    std::cout << "Количество ошибок чтения: " << travelsal.getReadErrorsCount() << std::endl;
    std::cout << "Время выполнения утилиты: " << "" << std::endl;
}

