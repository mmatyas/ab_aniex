#include "AniFile.h"

#include <stdexcept>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        printf("Usage: %s ANIFILE\n", argv[0]);
        return 1;
    }

    try {
        AniFile ani(argv[1]);
    }
    catch (const std::exception& error)
    {
        perror(error.what());
        return 1;
    }
    return 0;
}
