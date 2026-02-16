#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <string>

#define SIZE 10
template <typename T>
void output_static_data(T)
{
    std::cout << "static data of type " << typeid(T).name() << std::endl;
}

template <typename T,
          template <class> class Container>
class XCIs
{
private:
    Container<T> c;

public:
    XCIs()
    {
        for (long i = 0; i < SIZE; ++i)
            c.insert(c.end(), T());
        output_static_data(T());
        Container<T> c1(c);
        Container<T> c2(std::move(c));
        c1.swap(c2);
    }
};

template <typename T,
          template <typename> class Container>
class XCIs2
{
private:
    Container<T> c;

public:
    XCIs2()
    {
        for (long i = 0; i < SIZE; ++i)
            c.insert(c.end(), T());
        output_static_data(T());
        Container<T> c1(c);
        Container<T> c2(std::move(c));
        c1.swap(c2);
    }
};

template <typename T,
          template <class> typename Container>
class XCIs3
{
private:
    Container<T> c;

public:
    XCIs3()
    {
        for (long i = 0; i < SIZE; ++i)
            c.insert(c.end(), T());
        output_static_data(T());
        Container<T> c1(c);
        Container<T> c2(std::move(c));
        c1.swap(c2);
    }
};

template <typename T,
          template <typename> typename Container>
class XCIs4
{
private:
    Container<T> c;

public:
    XCIs4()
    {
        for (long i = 0; i < SIZE; ++i)
            c.insert(c.end(), T());
        output_static_data(T());
        Container<T> c1(c);
        Container<T> c2(std::move(c));
        c1.swap(c2);
    }
};

template <typename T>
using Vec = std::vector<T, std::allocator<T>>;
template <typename T>
using Lst = std::list<T, std::allocator<T>>;
template <typename T>
using Deq = std::deque<T, std::allocator<T>>;
int main()
{
    // XCIs<int, std::vector> xcis; // Error: std::vector has two template parameters, but only one is expected
    XCIs<int, Vec> xcisVec;
    XCIs<std::string, Vec> xcisStrVec;
    XCIs<int, Lst> xcisLst;
    XCIs<std::string, Lst> xcisStrLst;
    XCIs<int, Deq> xcisDeq;
    XCIs<std::string, Deq> xcisStrDeq;

    XCIs2<int, Vec> xcis2Vec;

    XCIs3<int, Vec> xcis3Vec;

    XCIs4<int, Vec> xcis4Vec;

    return 0;
}