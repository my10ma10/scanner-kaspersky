#include "traversal.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32

Traversal::Traversal() 
    : Traversal(fs::current_path())  
    {}

Traversal::Traversal(const fs::path& p) 
    : path(p), processedCounter(0)
    {}

void Traversal::run(ThreadSafeQueue<fs::path>& queue) {
    try {
        for (auto it = fs::recursive_directory_iterator(path, 
                fs::directory_options::skip_permission_denied |
                 fs::directory_options::follow_directory_symlink
            ); it != fs::recursive_directory_iterator(); ) 
        {
            try {
                if (isJunction(it->path())) {
                    std::cout << "Junction-ссылка: " << it->path().string() << std::endl;
                    it.disable_recursion_pending();
                    ++it;
                    continue;
                }
                auto status = fs::symlink_status(it->path());
                
                if (status.type() == fs::file_type::symlink) {
                    std::cout << "Symlink: " << it->path() << std::endl;
                    it.disable_recursion_pending();
                }

                if (it->is_regular_file()) {
                    ++processedCounter;
                    queue.push(it->path());
                }
                ++it;
            }
            catch (fs::filesystem_error& er) {
                std::cout << "Ошибка доступа " << it->path() << ": " << er.what() << std::endl;
                it.disable_recursion_pending();
                ++it;
            }
        }
    }
    catch (const fs::filesystem_error& err) {
        std::cout << "Ошибка обхода: " << err.what() << std::endl;
    }
}

bool Traversal::isJunction(const fs::path& p) {
    DWORD attrs = GetFileAttributesW(p.wstring().c_str());
    
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    
    if ((attrs & FILE_ATTRIBUTE_DIRECTORY) && (attrs & FILE_ATTRIBUTE_REPARSE_POINT)) {
        return true;
    }
    return false;
}


unsigned int Traversal::getProcessedCount() const {
    return processedCounter;
}