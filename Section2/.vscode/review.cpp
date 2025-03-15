#include <iostream>
#include <list>
#include <atomic>

void Func1(const char * str, int x = -1, int y = 3, int z = 4)

{
    std::cout<<str<<std::endl;
    std::cout<<x<<std::endl;
    std::cout<<y<<std::endl;
    std::cout<<z<<std::endl;
}
int main()
{
    /*
    * compiler error:  character constant too long for its type   the output is 543387760
    * std::cout<<'I love cpp'<<std::endl; 
    */
   auto x{10};
   Func1("Hello", 1, 2);
    return 0;
}