#include <iostream>
#include <list>
#include <atomic>

void Func1(const char * str, int x = -1, int y = 3, int z = 4, int v = 5)

{
   std::cout<<x<<std::endl;
   std::cout<<y<<std::endl;
   std::cout<<z<<std::endl;
   std::cout<<v<<std::endl;
   std::cout<<str<<std::endl;
}
int main()
{
   
   /*
   * compiler error:  character constant too long for its type   the output is 543387760
   * std::cout<<'I love cpp'<<std::endl; 
   */
   auto x{10};
   Func1("Hello", 1, 2);
   int* scalarValHeapPtr = new int{0}; // Allocate new memory on the heap for integer value and initialize it to 0
   std::cout << "scalarValHeapPtr: " << *scalarValHeapPtr << std::endl;
   delete scalarValHeapPtr; // Deallocate memory on the heap
   scalarValHeapPtr = nullptr; // Reset the pointer to nullptr
   int *arrayHeapPtr = new int[4]{0,0,0,0}; // Allocate new memory on the heap for an array of 4 integers and initialize them to 0
   std::cout << "arrayHeapPtr: " << arrayHeapPtr[0] << std::endl;   
   delete[] arrayHeapPtr; // Deallocate memory on the heap
   arrayHeapPtr = nullptr; // Reset the pointer to nullptr
   int *arrayHeapRow1Ptr = new int[3]{0,0,0};
   int *arrayHeapRow2Ptr = new int[3]{0,0,0};
   int** matrixHeapPtr = new int*[2]; // Allocate new memory on the heap for an array of 2 integer pointers
    matrixHeapPtr[0] = arrayHeapRow1Ptr; // Assign the first row of the matrix
    matrixHeapPtr[1] = arrayHeapRow2Ptr; // Assign the second row of the matrix
    std::cout << "matrixHeapPtr: " << matrixHeapPtr[0][0] << std::endl;
    delete[] arrayHeapRow1Ptr; // Deallocate memory on the heap for the first row of the matrix  == delete[] matrixHeapPtr[0]
    delete[] arrayHeapRow2Ptr; // Deallocate memory on the heap for the second row of the matrix == delete[] matrixHeapPtr[1]
    delete[] matrixHeapPtr; // Deallocate memory on the heap for the matrix
    arrayHeapRow1Ptr = nullptr; // Reset the pointer to nullptr
    arrayHeapRow1Ptr = nullptr; // Reset the pointer to nullptr
    matrixHeapPtr = nullptr; // Reset the pointer to nullptr    

   return 0;
}