#include <iostream>
#include "./InterestingDirectory/Division.h"

int main(int argc, char* argv[])
{
    std::cout << "main is running" << std::endl;
    int a = 2;
    int b = 4;
    int c = Division(a,b);
    std::cout << a << "/" << b << "=" << c << std::endl;

    return 0;
}