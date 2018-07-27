#include <iostream>
#include "uvc_capturer.h"

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0]
                  << " camId1, camId2, exposure in ms\n";
        exit(0);
    }

    uvc_capturer cameras(std::atoi(argv[1]), std::atoi(argv[2]), std::atoi(argv[3]));
    cameras.prepare();
    cameras.loop(0);
    return 0;
}
