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

class FooWithoutVirtual
{
private:
    int id;

public:
    FooWithoutVirtual() : id(0) { cout << "FooWithoutVirtual ctor(), id = " << id << ", this = " << this << endl; }
    FooWithoutVirtual(int id) : id(id) { cout << "FooWithoutVirtual ctor(int id), id = " << id << ", this = " << this << endl; }
    ~FooWithoutVirtual() { cout << "FooWithoutVirtual dtor(), id = " << id << ", this = " << this << endl; }
    void *operator new(size_t size)
    {
        void *p = malloc(size);
        cout << "FooWithoutVirtual::operator new()" << ", return " << p << endl;
        return p;
    }
    void operator delete(void *ptr)
    {
        cout << "FooWithoutVirtual::operator delete()" << ", ptr = " << ptr << endl;
        myFree(ptr);
    }
    void *operator new[](size_t size)
    {
        void *p = malloc(size);
        cout << "FooWithoutVirtual::operator new[]()" << ", return " << p << endl;
        return p;
    }
    void operator delete[](void *ptr)
    {
        cout << "FooWithoutVirtual::operator delete[]()" << ", ptr = " << ptr << endl;
        myFree(ptr);
    }
};

class FooWithVirtual
{
private:
    int id;

public:
    FooWithVirtual() : id(0) { cout << "FooWithVirtual ctor(), id = " << id << ", this = " << this << endl; }
    FooWithVirtual(int id) : id(id) { cout << "FooWithVirtual ctor(int id), id = " << id << ", this = " << this << endl; }
    virtual ~FooWithVirtual() { cout << "FooWithVirtual dtor(), id = " << id << ", this = " << this << endl; }

    void *operator new(size_t size)
    {
        void *p = malloc(size);
        cout << "FooWithoutVirtual::operator new()" << ", return " << p << endl;
        return p;
    }
    void operator delete(void *ptr)
    {
        cout << "FooWithoutVirtual::operator delete()" << ", ptr = " << ptr << endl;
        myFree(ptr);
    }
    void *operator new[](size_t size)
    {
        void *p = malloc(size);
        cout << "FooWithVirtual::operator new[]()" << ", return " << p << endl;
        return p;
    }
    void operator delete[](void *ptr)
    {
        cout << "FooWithoutVirtual::operator delete[]()" << ", ptr = " << ptr << endl;
        myFree(ptr);
    }
};

class FooOperatorOverload
{
private:
    int id;

public:
    FooOperatorOverload() : id(0) { cout << "FooOperatorOverload ctor(), this = " << this << endl; }
    FooOperatorOverload(int id) : id(id) { cout << "FooOperatorOverload ctor(int id), id = " << id << ", this = " << this << endl; }
    inline int GetId() const { return id; }

    void *operator new(size_t size)
    {
        void *p = malloc(size);
        cout << "FooOperatorOverload::operator new()" << ", return " << p << endl;
        return p;
    }

    void *operator new(size_t size, void *start)
    {
        void *p = start;
        cout << "FooOperatorOverload::operator new(size_t size, void* start)" << ", return " << p << endl;
        return p;
    }

    void *operator new(size_t size, long extra)
    {
        void *p = malloc(size + extra);
        cout << "FooOperatorOverload::operator new(size_t size, long extra)" << ", return " << p << endl;
        return p;
    }

    void *operator new(size_t size, long extra, char init)
    {
        void *p = malloc(size + extra);
        cout << "FooOperatorOverload::operator new(size_t size, long extra, char init)" << ", return " << p << endl;
        return p;
    }
};

class FooOperatorOverloadDefaultDelete
{
private:
    int id;

public:
    FooOperatorOverloadDefaultDelete() : id(0) { cout << "FooOperatorOverloadDelete ctor(), this = " << this << endl; }

public:
// 要求编译器生成默认的类成员版 operator new
#ifdef _MSC_VER
    static void *operator new(std::size_t size) noexcept(false);
    static void operator delete(void *ptr) noexcept;
#endif
    // 错误写法：测试g++和MSVC都编译报错
    // static void *operator new(size_t size) noexcept(false) = default; // default;
    // static void operator delete(void *ptr) noexcept = default;        // default;

    static void *operator new[](size_t size) noexcept(false) = delete; // delete;
    static void operator delete[](void *ptr) noexcept = delete;
};

int main()
{
    {

        void *p1 = operator new(sizeof(int));
        operator delete(p1);

        void *p2 = ::operator new[](sizeof(int) * 2);
        ::operator delete[](p2);
    }

    {
        std::cout << "sizeof(FooWithoutVirtual) = " << sizeof(FooWithoutVirtual) << endl;

        FooWithoutVirtual *p1 = ::new FooWithoutVirtual;
        ::delete p1;

        FooWithoutVirtual *p2 = new FooWithoutVirtual;
        delete p2;

        FooWithoutVirtual *p3 = new FooWithoutVirtual[2];
        delete[] p3;
    }

    {
        std::cout << "sizeof(FooWithVirtual) = " << sizeof(FooWithVirtual) << endl;
        FooWithVirtual *p1 = new FooWithVirtual;
        delete p1;

        FooWithVirtual *p2 = new FooWithVirtual[2];
        delete[] p2;
    }

    {
        std::cout << "sizeof(FooOperatorOverload) = " << sizeof(FooOperatorOverload) << endl;
        FooOperatorOverload start;
        FooOperatorOverload *p1 = new FooOperatorOverload;
        delete p1;

        FooOperatorOverload *p2 = new (&start) FooOperatorOverload;
        std::cout << p2->GetId() << std::endl;

        FooOperatorOverload *p3 = new (100) FooOperatorOverload;
        delete p3;

        FooOperatorOverload *p4 = new (200, 'a') FooOperatorOverload;
        delete p4;

        FooOperatorOverload *p5 = new (&start) FooOperatorOverload(100);
        std::cout << p2->GetId() << std::endl;
        std::cout << p5->GetId() << std::endl;
    }

    {
        FooOperatorOverloadDefaultDelete *p1 = new FooOperatorOverloadDefaultDelete; // 编译错误，operator new被删除了
        delete p1;                                                                   // 编译错误，operator delete被删除了
        // FooOperatorOverloadDefaultDelete *p2 = new FooOperatorOverloadDefaultDelete[3]; // 编译错误，operator new被删除了
        // delete[] p2;                                                                    // 编译错误，operator delete被删除了
    }
    return 0;
}