#include "AniFile.h"

#include <stdexcept>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    try {
        AniFile ani("STAND.ANI");
    }
    catch (const std::exception& error)
    {
        perror(error.what());
        return 1;
    }
    return 0;
}
