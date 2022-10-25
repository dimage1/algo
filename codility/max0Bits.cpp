#include <iostream>

template<class T>
size_t calcMax0(T val)
{
    size_t maxCount = 0;
    size_t lastCount = 0;

    while (val)
    {
        if (val & 1)
        {
            if (lastCount > maxCount)
            {
                maxCount = lastCount;
            }
            lastCount = 0;
        }
        else
        {
            lastCount++;
        }
        val >>= 1;
    }
    return maxCount;
}

int main(int argc, char **argv)
{
    std::cout << calcMax0(0b10001) << std::endl;
    std::cout << calcMax0(0) << std::endl;
    std::cout << calcMax0(0xFF00000000000000) << std::endl;

    if (argc > 1)
    {
        std::cout << calcMax0(atoi(argv[1])) << std::endl;
    }
}
