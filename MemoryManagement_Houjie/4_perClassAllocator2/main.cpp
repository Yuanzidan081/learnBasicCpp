#include <iostream>
#include <cstddef>

using namespace std;

class Airplane
{
private:
    struct AirplaneRep
    {
        unsigned long miles;
        char type;
    };

private:
    union
    {
        AirplaneRep rep; // 针对使用中的object
        Airplane *next;  // 针对在free list中的object
    };

public:
    unsigned long getMiles() const { return rep.miles; }
    char getType() const { return rep.type; }
    void set(unsigned long m, char t)
    {
        rep.miles = m;
        rep.type = t;
    }

public:
    static void *operator new(size_t size)
    {
        // 继承发生时可能会调用基类的operator new，导致size不等于Airplane的大小
        if (size != sizeof(Airplane))
            return ::operator new(size);
        Airplane *p = headOfFreeList;
        if (p)
            headOfFreeList = p->next;
        else
        {
            // free list为空，分配一个新的块
            Airplane *newBlock = static_cast<Airplane *>(::operator new(BLOCK_SIZE * sizeof(Airplane)));
            // 跳过索引0，因为它将被返回给调用者
            for (int i = 1; i < BLOCK_SIZE - 1; ++i)
                newBlock[i].next = &newBlock[i + 1];
            newBlock[BLOCK_SIZE - 1].next = nullptr;
            p = newBlock;
            headOfFreeList = &newBlock[1];
        }
        return p;
    }
    static void operator delete(void *deadObject, size_t size)
    {
        if (deadObject == nullptr)
            return;
        if (size != sizeof(Airplane))
        {
            ::operator delete(deadObject);
            return;
        }
        Airplane *p = static_cast<Airplane *>(deadObject);
        p->next = headOfFreeList;
        headOfFreeList = p;
    }

private:
    static const int BLOCK_SIZE;
    static Airplane *headOfFreeList;
};

Airplane *Airplane::headOfFreeList = nullptr;
const int Airplane::BLOCK_SIZE = 512;
int main()
{
    {
        std::cout << sizeof(Airplane) << std::endl;

        size_t const N = 100;
        Airplane *p[N];
        for (size_t i = 0; i < N; ++i)
            p[i] = new Airplane();
        p[1]->set(1000, 'A');
        p[2]->set(2000, 'B');
        p[9]->set(9000, 'C');
        for (size_t i = 0; i < 10; ++i)
            cout << p[i] << std::endl;

        for (size_t i = 0; i < N; ++i)
            delete p[i];
    }
}