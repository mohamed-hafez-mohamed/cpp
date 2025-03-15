#include <iostream>
#include <list>
#include <atomic>
using namespace std;

// class c{
//     double x{0.0};
//     double y{0.0};
//     public:
//         funct(double x_, double y_) : x{x_}, y{y_}   // initializer list
//         { 
            
//         }
        
// };

typedef struct emplo
    {
        int id{0};
        string name{"employee"}; // member initialization
    }employee;

int main()
{
    int a = 10;  // copy initialization
    int b(20);   // direct initialization
    int c{30};   // uniform initialization
    int * dynamicMemory = new int[3]{1,2,3}; // dynamic initialization
    list<int> l{1,2,3,4,5}; // uniform initialization for linked list container
    list<int> l2(3,1); // direct initialization for linked list container
    atomic<int> ai{0}; // uniform initialization for non copyable object
    auto f{4.2}; // uniform initialization for auto type
    const int arg{40}; // constant initialization
    constexpr auto consex{arg * 2}; // constant initialization
    // cout << "Hello, World!\n"; // \n is escape sequence (control character)
    // cout << "Hello, World!" << endl; // endl is manipulator automatically adds a newline character and flushes the output buffer
    // char buffer[2];
    // char buffer2[2];
    // cout << "Enter your name: " << endl;
    // cin >> buffer;
    // cout << "Your name is: " << buffer << endl;
    // cout << "Enter your name: " << endl;
    // cin.getline(buffer2, 10,'\n');
    // cout << "Your name is: " << buffer2 << endl;
    int arr[]{1,2,3};
    int (*ptr)[3] = &arr;
    int *ptr2 = &arr[0];
    for(auto & index: arr)
    { 
        index = 5;
        cout << index << endl;
        cout << arr[0] << endl;
    }
    *(ptr)[0] = 10;
    cout << arr[0] << endl;
    *(*ptr) = 20;
    cout << arr[0] << endl;
    *(*ptr+ 1 ) = 30;
    cout << arr[1] << endl;
    ptr2[2] = 40;
    cout << arr[2] << endl;
    int &&rval_ref{10};
    auto &&ref_rval = {1,2};
    auto &&ref10{arr};
    return 0;
}