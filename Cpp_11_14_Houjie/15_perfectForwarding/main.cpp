#include <iostream>

using namespace std;

void process(const int &i)
{
    cout << "process(int& i)"
         << endl;
}

void process(int &&i)
{
    cout << "process(int&& i)"
         << endl;
}

void forward(int &&i)
{
    cout << "forward(int&& i)" << endl;
    process(i);
}

void forward(const int &i)
{
    cout << "forward(int& i)" << endl;
    process(i);
}

void test_basic()
{
    cout << "--- Test Basic ---" << endl;
    int a = 0;
    cout << "process(a): ";
    process(a); // 调用 process(int& i) a是左值
    cout << "process(1): ";
    process(1); // 调用 process(int&& i) 1是右值
    cout << "process(move(a)): ";
    process(move(a)); // 调用 process(int&& i) a是左值，但是move(a)是右值
    const int &b = 1;
    cout << "process(b): ";
    process(b); // 调用 process(int& i) b是左值
    cout << "process(move(b)): ";
    process(move(b)); // 调用 process(int& i) ? move(b)是右值常量，不匹配右值的函数！

    cout << endl;
}

void test_unperfect_forwarding()
{
    cout << "--- Test Unperfect Forwarding ---" << endl;
    int a = 0;
    cout << "forward(2): ";
    forward(2); // 调用 forward(int&& i) 2是右值，但是forward里传递给另一个函数会变为左值
    cout << "forward(move(a)): " << endl;
    forward(move(a)); // 调用 forward(int&& i) move(a)是右值，但是forward里传递给另一个函数会变为左值
    cout << "forward(a): " << endl;
    forward(a); // 调用 forward(int& i) a是左值，forward里传递也是左值

    // int &x(5); // 引用要是左值
    cout << endl;
}

template <typename T>
void perfect_forward(T &&t)
{
    cout << "template perfect_forward(T&& t)" << endl;
    process(forward<T>(t));
}

void test_perfect_forwarding()
{
    cout << "--- Test Perfect Forwarding ---" << endl;
    int a = 0;
    cout << "perfect_forward(a): ";
    perfect_forward(a); // 调用 perfect_forward(int& i) a是左值，perfect_forward里传递也是左值
    cout << "perfect_forward(2): ";
    perfect_forward(2); // 调用 perfect_forward(int&& i) 2是右值，perfect_forward里传递也是右值
    cout << "perfect_forward(move(a)): ";
    perfect_forward(move(a)); // 调用 perfect_forward(int&& i) move(a)是右值，perfect_forward里传递也是右值

    cout << endl;
}

int main()
{
    test_basic();

    test_unperfect_forwarding();

    test_perfect_forwarding();
    return 0;
}