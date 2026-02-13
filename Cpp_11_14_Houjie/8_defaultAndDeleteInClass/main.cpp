#include <iostream>
#include <utility> // 用于std::move
#include <string>
class MyClass
{
public:
    // 1. 恢复默认构造函数（因自定义了带参构造，编译器不再自动生成）
    MyClass() = default;

    // 2. 自定义带参构造函数
    MyClass(int val) : value(val) {}

    // 3. 显式要求编译器生成默认拷贝构造函数
    MyClass(const MyClass &) = default;

    // 4. 显式要求编译器生成默认拷贝赋值运算符
    MyClass &operator=(const MyClass &) = default;

    // 5. 显式要求编译器生成默认移动构造函数（C++11）
    MyClass(MyClass &&) = default;

    // 6. 显式要求编译器生成默认移动赋值运算符（C++11）
    MyClass &operator=(MyClass &&) = default;

    // 7. 析构函数显式使用默认版本
    ~MyClass() = default;

    void print() const
    {
        std::cout << "Value: " << value << std::endl;
    }

private:
    int value = 0;
};

class NonCopyable
{
public:
    // 构造函数
    NonCopyable(std::string msg) : message(std::move(msg)) {}

    // 禁止拷贝构造函数
    NonCopyable(const NonCopyable &) = delete;

    // 禁止拷贝赋值运算符
    NonCopyable &operator=(const NonCopyable &) = delete;

    // 允许移动构造
    NonCopyable(NonCopyable &&) = default;

    // 允许移动赋值
    NonCopyable &operator=(NonCopyable &&) = default;

    void show() const
    {
        std::cout << message << std::endl;
    }

private:
    std::string message;
};

class Number
{
public:
    // 接受int类型的构造函数
    Number(int num) : value(num) {}

    // 禁止接受double类型的构造函数（避免隐式转换）
    Number(double) = delete;

    int getValue() const { return value; }

private:
    int value;
};

void printNumber(const Number &n)
{
    std::cout << n.getValue() << std::endl;
}

class OnlyParamCtor
{
public:
    // 禁止默认构造函数
    OnlyParamCtor() = delete;

    // 强制使用带参构造
    OnlyParamCtor(int val) : data(val) {}

private:
    int data;
};

int main()
{
    // 1. default的使用
    {
        MyClass a;                // 调用默认构造函数
        MyClass b(10);            // 调用带参构造函数
        MyClass c = b;            // 调用默认拷贝构造函数
        MyClass d = std::move(b); // 调用默认移动构造函数
        a = c;                    // 调用默认拷贝赋值运算符
        d = std::move(a);         // 调用默认移动赋值运算符

        d.print(); // 输出：Value: 10
    }
    // 2. delete的使用：禁止拷贝构造和拷贝赋值
    {
        NonCopyable obj1("Hello, C++11");
        // NonCopyable obj2 = obj1; // 编译错误：拷贝构造函数已删除
        // obj1 = obj2; // 编译错误：拷贝赋值运算符已删除

        NonCopyable obj2 = std::move(obj1); // 合法：调用移动构造
        obj2.show();                        // 输出：Hello, C++11
    }
    // 3. delete的使用：禁止隐式类型转换
    {
        Number n1(10); // 合法：int类型可以直接构造
        // Number n2 = 20.0; // 编译错误：double类型构造已删除
        printNumber(n1); // 输出：10
        // Number n2(3.14); // 编译错误：double版本构造函数已删除
        // printNumber(3.14); // 编译错误：无法隐式转换为Number（double构造被删除）
    }
    // 4. delete的使用：禁止使用delete关键字删除默认构造函数
    {
        // OnlyParamCtor obj; // 编译错误：默认构造函数已删除
        OnlyParamCtor obj(5); // 合法
    }
    return 0;
}