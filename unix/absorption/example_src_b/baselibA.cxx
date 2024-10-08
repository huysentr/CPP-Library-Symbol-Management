#include <iostream>
#include <string>
#include "baselib.h"

__attribute__((visibility("hidden"))) void basefunc()
{
    std::cout << "baselibA" << std::endl;
}
