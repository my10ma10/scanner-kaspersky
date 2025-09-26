#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <filesystem>

#include <openssl/md5.h>

#define BUF_SIZE 4096

namespace fs = std::filesystem;

class MD5Calculator {
    MD5_CTX md5;
    std::ifstream file;
    std::ostringstream oss;
    std::vector<char> buf;

    std::vector<unsigned char> fileHash;
    unsigned int readErrorsCounter;

    void format();
public:
    MD5Calculator();
    
    std::string calculate(const fs::path& filepath);

    unsigned int getReadErrorsCount() const;
    std::string getHash() const;
};