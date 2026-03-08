#include <iostream>
#include <cstddef>

using namespace std;

class Screen
{
public:
    Screen(int x) : id(x) {}
    int get() { return id; }

    void *operator new(size_t size)
    {
        Screen *p;
        if (!freeStore)
        {
            size_t chunk = screenChunk * size;
            freeStore = reinterpret_cast<Screen *>(new char[chunk]);
            for (p = freeStore; p != &freeStore[screenChunk - 1]; ++p)
                p->next = p + 1;
            freeStore[screenChunk - 1].next = nullptr;
        }
        p = freeStore;
        freeStore = freeStore->next;
        std::cout << "Screen::operator new(), return " << p << std::endl;
        return p;
    }

    void operator delete(void *ptr)
    {
        Screen *p = reinterpret_cast<Screen *>(ptr);
        p->next = freeStore;
        std::cout << "Screen::operator delete(), ptr = " << ptr << std::endl;
        freeStore = p;
    }

private:
    Screen *next;
    static Screen *freeStore;
    static const int screenChunk;

private:
    int id;
};

Screen *Screen::freeStore = nullptr;
const int Screen::screenChunk = 24;

int main()
{
    {
        std::cout << sizeof(Screen) << std::endl;

        size_t const N = 8;
        Screen *p[N];
        for (size_t i = 0; i < N; ++i)
            p[i] = new Screen(i);

        for (size_t i = 0; i < N; ++i)
            cout << p[i] << std::endl;

        for (size_t i = 0; i < N; ++i)
            delete p[i];
    }
}