#include "utility.hpp"

Utility::Utility(const std::string& path, const std::string& base, const std::string& logfile) 
    : travelsal(path)
    {
    timer.start();
}

void Utility::run() {
    travelsal.run(queue);
}

void Utility::printReport() const {
    std::cout << "Обработано файлов: " << travelsal.getProcessedCount() << std::endl;
    std::cout << "Из них вредоносных: " << parser.getCoincidencesCount() << std::endl;
    std::cout << "Количество ошибок чтения: " << md5Calc.getReadErrorsCount() << std::endl;
    std::cout << "Время выполнения утилиты: " << timer.getTime() << " сек" << std::endl;
}

