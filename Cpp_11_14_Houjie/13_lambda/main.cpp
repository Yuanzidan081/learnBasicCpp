#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
void test_basic_lambda()
{
    std::cout << "===== 1. test basic lambda =====" << std::endl;
    []
    { std::cout << "Hello, Lambda!" << std::endl; }(); // 定义并立即调用Lambda表达式

    auto l = []
    { std::cout << "Hello, Lambda with auto!" << std::endl; };
    l(); // 调用Lambda表达式
}

void test_mutable_lambda()
{
    std::cout << "===== 2. test mutable Lambda(passed by value) and lambda(passed by reference)=====" << std::endl;
    {

        int x = 10;
        std::cout << "Test l1 lambda" << std::endl;
        auto l1 = [x]() mutable
        {
            ++x; // mutable允许修改捕获的变量副本
            std::cout << "Inside mutable lambda, x = " << x << std::endl;
        };
        x = 42;
        l1();
        l1();
        l1();                                                  // 输出：Inside mutable lambda, x = 15
        std::cout << "Outside lambda, x = " << x << std::endl; // 输出：Outside lambda, x = 10
    }
    {
        int x = 10;
        std::cout << "Test l2 lambda" << std::endl;
        auto l2 = [&x](int param)
        {
            x += param;
            std::cout << "Inside lambda l2, x = " << x << std::endl;
        };
        x = 42;
        l2(5); // 调用l2，x变为15
        l2(5);
        l2(5);
        std::cout << "Outside lambda l2, x = " << x << std::endl; // 输出：Outside lambda l2, x = 15
    }
    {
        // Error: x is read only in this context
        /* int x = 10;
        std::cout << "Test l1 lambda" << std::endl;
        auto l3 = [x]()
        {
            ++x; // mutable允许修改捕获的变量副本
            std::cout << "Inside mutable lambda, x = " << x << std::endl;
        };
        x = 42;
        l3();
        l3();
        l3();                                                  // 输出：Inside mutable lambda, x = 15
        std::cout << "Outside lambda, x = " << x << std::endl; // 输出：Outside lambda, x = 10
        */
    }
}

struct Person
{
    std::string lastName;
    std::string firstName;
    Person(std::string ln, std::string fn) : lastName(ln), firstName(fn) {}
};

void test_decltype_lambda()
{
    std::cout << "===== 3. test decltype lambda =====" << std::endl;
    auto cmp = [](const Person &x, const Person &y)
    { return x.lastName < y.lastName || (x.lastName == y.lastName && x.firstName < y.firstName); };
    std::set<Person, decltype(cmp)> sorted_set(cmp); // 用Lambda表达式作为比较器
    // std::set<Person, decltype(cmp)> sorted_set2; // Error: no matching function for call to ‘std::set<Person, decltype(cmp)>::set()’（无法默认构造，必须提供比较器实例）
    std::cout << std::endl;
}

void test_lambda_capture_in_algorithm()
{
    std::cout << "===== 4. test lambda capture in STL algorithm =====" << std::endl;

    std::vector<int> vi{5, 28, 50, 83, 70, 90, 12, 45, 67, 33};
    int x = 30;
    int y = 100;
    vi.erase(std::remove_if(vi.begin(), vi.end(), [x, y](int val)
                            { return val < x || val > y; }),
             vi.end());
    for (int i : vi)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main()
{
    test_basic_lambda();
    test_mutable_lambda();
    test_decltype_lambda();
    test_lambda_capture_in_algorithm();
    return 0;
}