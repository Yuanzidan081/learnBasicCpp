#include <iostream>
#include <string>
#include <new>
#include <cassert>
using namespace std;

void noMoreMemory()
{
    cout << "Out of memory!" << endl;
    abort();
}

int main()
{
    // gcc显示Out of memory, msvc没有显示
    set_new_handler(noMoreMemory);

    {
        int *p = new (nothrow) int[1000000000000]; // 试图分配一个非常大的数组，可能会失败
        if (p == nullptr)
        {
            cout << "nothrow new failed to allocate memory." << endl;
        }
    }

    {
        int *p = new int[1000000000000]; // 试图分配一个非常大的数组，可能会失败
        assert(p != nullptr);            // 如果分配失败，noMoreMemory会被调用，程序会终止
    }

    return 0;
}