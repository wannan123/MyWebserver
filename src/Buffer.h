#pragma once
#include <string>
#include <iostream>
class Buffer
{
    private:
        std::string s;
    public:
        void append(const char* _str, int _size);
        ssize_t size();
        const char* c_str();
        void clear();
        void getline();
        Buffer();
        ~Buffer();
};

