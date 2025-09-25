#include "utility.hpp"


Utility::Utility(
    const std::string& path, 
    const std::string& base, 
    const std::string& logfile
)
    : 
    traversal(path), 
    thPool(std::thread::hardware_concurrency())
{
    timer.start();
    parser.init(base);
    Logger::setLogFile(logfile);
}

void Utility::run() {
    std::cout << "Утилита в работе...\n";
    std::thread producer([this] () {
        traversal.run(queue);
        queue.allAddedNotifiation();
    });
    
    createWorkers(thPool.getThreadsNumber());

    producer.join();
    thPool.shutdown();
    thPool.joinAll();
}

void Utility::createWorkers(unsigned int n) {
    for (size_t i = 0; i < n; ++i) {
        thPool.enqueueTask([this] () {
            while (auto p = queue.pop()) {
                std::unique_lock lock(mtx);

                std::string hash = md5Calc.calculate(p->string());
                
                parser.findMalicious(hash, p->string());
                lock.unlock();
                
            }
        });
    }
}

void Utility::printReport() const {
    std::cout << "Обработано файлов: " << traversal.getProcessedCount() << std::endl;
    std::cout << "Из них вредоносных: " << parser.getCoincidencesCount() << std::endl;
    std::cout << "Количество ошибок чтения: " << md5Calc.getReadErrorsCount() << std::endl;
    std::cout << "Время выполнения утилиты: " << timer.getTime() << " сек" << std::endl;
}

