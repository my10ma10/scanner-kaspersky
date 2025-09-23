#pragma once
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdio>

#include <openssl/md5.h>

#define BUF_SIZE 1024

class MD5Calculator {
    MD5_CTX md5;
    std::ifstream file;
    std::ostringstream oss;
    std::vector<char> buf;

    std::vector<unsigned char> fileHash;

public:
    MD5Calculator(const std::string& filename);
    
    std::string calculate();

    void readFile();
    void format();

    std::string getHash() const;
};