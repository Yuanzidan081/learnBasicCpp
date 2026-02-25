#include "malloc.h"
#include <complex>
#include <iostream>
#include <memory>
#if __GNUC__
#include "ext/pool_allocator.h"
#endif
using namespace std;
int main()
{
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

    return 0;
}