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

class Foo
{
public:
    long L;
    string str;
    static Allocator alloc;

public:
    Foo(long l, const string &s) : L(l), str(s) {}
    static void *operator new(size_t size)
    {
        return alloc.allocate(size);
    }
    static void operator delete(void *ptr, size_t size)
    {
        alloc.deallocate(ptr, size);
    }
};

Allocator Foo::alloc;

class Goo
{
public:
    complex<double> c;
    static Allocator alloc;

public:
    Goo(const complex<double> &x) : c(x) {}
    static void *operator new(size_t size)
    {
        return alloc.allocate(size);
    }
    static void operator delete(void *ptr, size_t size)
    {
        alloc.deallocate(ptr, size);
    }
};

Allocator Goo::alloc;

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
