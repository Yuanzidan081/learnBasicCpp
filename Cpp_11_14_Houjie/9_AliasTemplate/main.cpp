#include <vector>
#include <iostream>
using namespace std;
#define SIZE 10

template <typename T>
void output_static_data(T)
{
    cout << "static data of type " << typeid(T).name() << endl;
}
/*
// test 1
// Error: Container and T are not defined in this scope
void test_moveable(Container cntr, T elem)
{
    Container<T> c;
    for (long i = 0; i < SIZE; ++i)
        c.insert(c.end(), T());
    output_static_data(T());
    Container<T> c1(c);
    Container<T> c2(std::move(c));
    c1.swap(c2);
}
*/

/*
// test 2
// Container is not a template, so Container<T> is invalid
template <typename Container, typename T>
void test_moveable(Container cntr, T elem)
{
    Container<T> c;
    for (long i = 0; i < SIZE; ++i)
        c.insert(c.end(), T());
    output_static_data(T());
    Container<T> c1(c);
    Container<T> c2(std::move(c));
    c1.swap(c2);
}

template <typename Container, typename T>
void test_moveable(Container cntr, T elem)
{
    typename Container<T> c;
    for (long i = 0; i < SIZE; ++i)
        c.insert(c.end(), T());
    output_static_data(T());
    Container<T> c1(c);
    Container<T> c2(std::move(c));
    c1.swap(c2);
}
*/

template <typename Container, typename T>
void test_moveable(Container, T elem)
{
    Container c;
    for (long i = 0; i < SIZE; ++i)
        c.insert(c.end(), T());
    output_static_data(T());
    Container c1(c);
    Container c2(std::move(c));
    c1.swap(c2);
}

template <typename Container>
void test_moveable(Container c)
{
    typedef typename iterator_traits<typename Container::iterator>::value_type ValType;
    using ValType2 = typename iterator_traits<typename Container::iterator>::value_type;
    for (long i = 0; i < SIZE; ++i)
        c.insert(c.end(), ValType());
    output_static_data(ValType2());
    Container c1(c);
    Container c2(std::move(c));
    c1.swap(c2);
}

template <typename T>
using IntContainer = vector<T, allocator<T>>;

int main()
{
    test_moveable(vector<string>(), string(""));
    test_moveable(vector<double>());
    test_moveable(IntContainer<int>());
    return 0;
}