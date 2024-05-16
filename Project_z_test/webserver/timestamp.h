#pragma once

#include <iostream>
#include <string>
#include <time.h>

class timestamp {
public:
    timestamp();
    explicit timestamp(int64_t microSecondsSinceEpoch);
    static timestamp now();
    std::string toString() const;
private:
    int64_t microSecondsSinceEpoch_;
};