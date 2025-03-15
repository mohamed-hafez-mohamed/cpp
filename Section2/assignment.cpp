#include <iostream>
using namespace std;

int Add(const int *a, const int *b) ;    //Add two numbers and return the sum
void AddVal(const int *a, const int *b, int *result); //Add two numbers and return the sum through the third pointer argument
void Swap(int *a, int *b) ;  //Swap the value of two integers
void Factorial(const int *a, int *result);   //Generate the factorial of a number and return that through the second pointer argument  
void Swa2(int &a, int &b) ;  //Swap the value of two integers
int main()
{
    // const int a {10};
    // const int b {20};
    // int aa {5};
    // int bb {7};
    // int result1 {0};
    // int result2 {0};
    // int result3 {0};
    // result1 = Add(&a, &b);
    // AddVal(&a, &b, &result2);
    // Swap(&aa, &bb);
    // Factorial(&a, &result3);
    // cout << "ADD = " << result1 << endl << "ADDVAL = " << result2 << endl <<  "a = " << aa << endl << "b = " << bb << endl << "a fact = " << result3 << endl;
    //Try to modify x1 & x2 and see the compilation output
    int x = 5 ;
    const int MAX = 12 ;
    int &ref_x1 = x ;
    const int &ref_x2 = x ;
    //Try to modify the pointer (e.g. ptr1) and the pointee value (e.g. *ptr1)
    const int *ptr1 = &x ;
    int *const ptr2 = &x ;
    const int * const ptr3 = &x ;
     
    //Find which declarations are valid. If invalid, correct the declaration
    // const int *ptr3 = &MAX ;
    // int *ptr4 = &MAX ;
     
    // const int &r1 = ref_x1 ;
    // int &r2 = ref_x2 ;
     
    // int *&p_ref1 = ptr1 ;
    // const int*&p_ref2 = ptr2 ;
    return 0;
}

int Add(const int *a, const int *b)
{
    return *a + *b;
} 

void AddVal(const int *a, const int *b, int *result)
{
    *result = *a + *b;
}

void Swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void Factorial(const int *a, int *result)
{
    *result = 1;
    for(int i = 1; i <= *a; i++)
    {
        *result *= i;
    }
}

void Swa2(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}