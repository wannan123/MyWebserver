#pragma once
#include <cassert>
#include <stdexcept>
#define OS_LINUX


#define DISALLOW_COPY(cname)  \
    cname(const cname &) = delete;  \
    cname &operator =(const cname &) = delete;

#define DISALLOW_MOVE(cname)  \
    cname(const cname &&) = delete;  \
    cname &operator =(const cname &&) = delete;

#define DISALLOW_COPY_AND_MOVE(cname)  \
    DISALLOW_COPY(cname);    \
    DISALLOW_MOVE(cname);    

#define ASSERT(expr, message) assert((expr) && (message))

#define UNREACHABLE(message) throw std::logic_error(message)