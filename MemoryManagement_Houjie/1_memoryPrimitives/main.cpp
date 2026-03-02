#include "malloc.h"
#include <complex>
#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <new>
#if __GNUC__
#include "ext/pool_allocator.h"
#endif
using namespace std;

class A
{
public:
    int id;
    A(int i) : id(i) { cout << "ctor. this = " << this << " id = " << id << endl; }
    ~A() { cout << "dtor. this = " << this << endl; }
};

class B
{
public:
    int id;
    B() : id(0) { cout << "default ctor. this = " << this << " id = " << id << endl; }
    B(int i) : id(i) { cout << "ctor. this = " << this << " id = " << id << endl; }
    ~B() { cout << "dtor. this = " << this << " id = " << id << endl; }
};
int main()
{
    // Test 1: malloc, free, new, delete, operator new, operator delete, allocator
    {
        cout << "--- Test 1: malloc, free, new, delete, operator new, operator delete, allocator ---" << endl;
        void *p1 = malloc(512);
        std::cout << "malloc: " << p1 << std::endl;
        free(p1);

        complex<int> *p2 = new complex<int>;
        std::cout << "new: " << p2 << std::endl;
        delete p2;

        void *p3 = ::operator new(512);
        std::cout << "operator new: " << p3 << std::endl;
        ::operator delete(p3);

#ifdef _MSC_VER
        int *p4 = allocator<int>().allocate(3, (int *)0);
        std::cout << "allocator<int> allocate:" << p4 << std::endl;
        allocator<int>().deallocate(p4, 3);
#endif

#ifdef __GNUC__
        void *p4 = allocator<int>().allocate(7);
        std::cout << "allocator<int>().allocate" << p4 << std::endl;
        allocator<int>().deallocate((int *)p4, 7);

        void *p5 = __gnu_cxx::__pool_alloc<int>().allocate(7);
        std::cout << "__gnu_cxx::__pool_alloc<int>().allocate" << p5 << std::endl;
        __gnu_cxx::__pool_alloc<int>().deallocate((int *)p5, 7);
#endif
    }

    // Test 2: constructor, destructor
    {
        cout << "--- Test 3: constructor, destructor ---" << endl;
        string *pstr = new string("hello");
        std::cout << "str=" << *pstr << endl;

        // pstr->string::string("jjj"); // MSVC and g++ Error: 类 "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>" 没有成员 "string"
        pstr->~string(); // MSVC and g++ OK
        std::cout << "str=" << *pstr << endl;
        // delete pstr;

        A *pa = new A(100);
        std::cout << "pa->id = " << pa->id << endl;
#ifdef _MSC_VER
        pa->A::A(3); // MSVC OK, g++ Error: cannot call constructor 'A::A' directly
        A::A(5);     // MSVC OK, , g++ Error: cannot call constructor 'A::A' directly
#endif
        std::cout << pa->id << endl;
        delete pa;
    }

    // Test 3: array new and delete
    {
        B *buf = new B[3];
        B *tmp = buf;
        cout << "buf=" << buf << endl;
        for (int i = 0; i < 3; ++i)
            new (tmp++) B(i);
        cout << "buf=" << buf << " tmp=" << tmp << endl;
        delete[] buf; // dtor 3 times
        // delete buf; // dtor once, may cause memory leak and error!
    }

    // Test 4: placement new
    {
        cout << "--- Test 4: placement new ---" << endl;
        char *buf = new char[sizeof(A) * 3];
        A *pa = new (buf) A(1);
        A *pb = new (buf + sizeof(A)) A(2);
        A *pc = new (buf + sizeof(A) * 2) A(3);
        cout << "pa->id=" << pa->id << " pb->id=" << pb->id << " pc->id=" << pc->id << endl;

        delete[] buf;
    }
    system("pause");
    return 0;
}