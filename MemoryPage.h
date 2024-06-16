#ifndef MEMORYPAGE_H
#define MEMORYPAGE_H

#include <vector>
#include <windows.h>

struct MemoryPage {
    std::vector<BYTE> bytes;
    std::size_t size;
};

#endif // MEMORYPAGE_H
