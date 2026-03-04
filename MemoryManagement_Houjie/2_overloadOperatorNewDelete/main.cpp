#include <iostream>
using namespace std;

void *myAlloc(size_t size)
{
    return malloc(size);
}

void myFree(void *ptr)
{
    free(ptr);
}

inline void *operator new(size_t size)
{
    cout << "my global new()" << endl;
    return myAlloc(size);
}

inline void *operator new[](size_t size)
{
    cout << "my global new[]()" << endl;
    return myAlloc(size);
}

inline void operator delete(void *ptr)
{
    cout << "my global delete()" << endl;
    myFree(ptr);
}

inline void operator delete[](void *ptr)
{
    cout << "my global delete[]()" << endl;
    myFree(ptr);
}

class Foo
{
public:
    Foo() { cout << "Foo ctor" << endl; }
    ~Foo() { cout << "Foo dtor" << endl; }
    void *operator new(size_t size)
    {
        cout << "Foo global new()" << endl;
        return myAlloc(size);
    }
    void operator delete(void *ptr)
    {
        cout << "Foo global delete()" << endl;
        myFree(ptr);
    }
    void *operator new[](size_t size)
    {
        cout << "Foo global new[]()" << endl;
        return myAlloc(size);
    }
    void operator delete[](void *ptr)
    {
        cout << "Foo global delete[]()" << endl;
        myFree(ptr);
    }
};
int main()
{
    void *p = operator new(sizeof(int));
    operator delete(p);

    void *p2 = ::operator new[](sizeof(int) * 10);
    ::operator delete[](p2);

    Foo *p3 = new Foo;
    delete p3;

    Foo *p4 = new Foo[3];
    delete[] p4;
    return 0;
}