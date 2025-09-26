#include "md5.hpp"

MD5Calculator::MD5Calculator() {
    readErrorsCounter = 0;

    fileHash = std::vector<unsigned char>(MD5_DIGEST_LENGTH);
    buf = std::vector<char>(BUF_SIZE);

    MD5_Init(&md5);
}

std::string MD5Calculator::calculate(const fs::path& filepath) {
    oss.str("");
    oss.clear();

    MD5_Init(&md5);

    file.open(filepath, std::ios::binary);
    if (!file.is_open()) {
        ++readErrorsCounter;
        std::cout << "Ошибка чтения " << filepath << std::endl;
        return "";
    }
    else {
        while (file.read(buf.data(), buf.size()) || file.gcount() > 0) {
            MD5_Update(&md5, buf.data(), file.gcount());
        }
    }
    MD5_Final(fileHash.data(), &md5);
    format();
    file.close();
    return oss.str();
}

void MD5Calculator::format() {
    char b[3];
    for (unsigned char c : fileHash) {
        std::snprintf(b, sizeof(b), "%02x", c);
        oss << b;
    }
}

std::string MD5Calculator::getHash() const {
    return oss.str();
}

unsigned int MD5Calculator::getReadErrorsCount() const {
    return readErrorsCounter;
}
