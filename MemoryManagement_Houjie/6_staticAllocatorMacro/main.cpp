#include <iostream>
#include <string>
#include <complex>
using namespace std;

class Allocator
{
private:
    struct obj
    {
        struct obj *next;
    };

public:
    void *allocate(size_t size)
    {
        obj *p;
        if (!freeStore)
        {
            size_t chunk = CHUNK * size;
            freeStore = p = (obj *)malloc(chunk);
            for (int i = 0; i < CHUNK - 1; ++i)
            {
                p->next = (obj *)((char *)p + size);
                p = p->next;
            }
            p->next = nullptr;
        }
        p = freeStore;
        freeStore = freeStore->next;
        return p;
    }
    void deallocate(void *ptr, size_t size)
    {
        ((obj *)ptr)->next = freeStore;
        freeStore = (obj *)ptr;
    }

private:
    obj *freeStore = nullptr;
    const int CHUNK = 5;
};

// DECLARE_POOL_ALLOC -used in class definition
#define DECLARE_POOL_ALLOC()                                                      \
public:                                                                           \
    void *operator new(size_t size) { return alloc.allocate(size); }              \
    void operator delete(void *ptr, size_t size) { alloc.deallocate(ptr, size); } \
                                                                                  \
protected:                                                                        \
    static Allocator alloc;

// IMPLEMENT_POOL_ALLOC -used in class implementation
#define IMPLEMENT_POOL_ALLOC(className) Allocator className::alloc;
class Foo
{
    DECLARE_POOL_ALLOC()
public:
    long L;
    string str;

public:
    Foo(long l, const string &s) : L(l), str(s) {}
};

IMPLEMENT_POOL_ALLOC(Foo)

class Goo
{
    DECLARE_POOL_ALLOC()
public:
    complex<double> c;

public:
    Goo(const complex<double> &x) : c(x) {}
};

IMPLEMENT_POOL_ALLOC(Goo)

int main()
{
    {
        Foo *p[10];
        cout << "sizeof(Foo) = "
             << sizeof(Foo) << endl;
        for (int i = 0; i < 10; ++i)
        {
            p[i] = new Foo(i, "hello");
            cout << p[i] << " " << p[i]->L << endl;
        }
        for (int i = 0; i < 10; ++i)
            delete p[i];
    }

    {
        Goo *p[10];
        cout << "sizeof(Goo) = " << sizeof(Goo) << endl;
        for (int i = 0; i < 10; ++i)
        {
            p[i] = new Goo(complex<double>(i, i));
            cout << p[i] << " " << p[i]->c << endl;
        }
        for (int i = 0; i < 10; ++i)
            delete p[i];
    }
    return 0;
}
