#pragma once
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdio>

#include <openssl/md5.h>

#define BUF_SIZE 4096

class MD5Calculator {
    MD5_CTX md5;
    std::ifstream file;
    std::ostringstream oss;
    std::vector<char> buf;

    std::vector<unsigned char> fileHash;
    unsigned int readErrorsCounter;

    void format();
    void readFile(const std::string& filepath);
public:
    MD5Calculator();
    
    std::string calculate(const std::string& filepath);

    unsigned int getReadErrorsCount() const;
    std::string getHash() const;
};