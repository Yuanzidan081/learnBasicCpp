#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <utility>
#include <algorithm>

// ======================== 1. Type Alias（类型别名） ========================
/**
 * Type Alias 分为两种：
 * 1. 普通类型别名：替代typedef，语法更直观
 * 2. 别名模板（Alias Template）：支持模板参数化别名，弥补typedef的缺陷
 */

// 1.1 普通类型别名（替代typedef）
// 传统typedef写法
typedef std::vector<int> IntVec_Typedef;
using IntVec_Using = std::vector<int>;

typedef void (*FuncPtr_Typedef)(int, const std::string &);
using FuncPtr_Using = void (*)(int, const std::string &);

// 1.2 别名模板（Alias Template）：支持模板参数化
// 为带默认分配器的容器定义简化别名
template <typename T>
using Vec = std::vector<T, std::allocator<T>>;

template <typename T>
using Lst = std::list<T, std::allocator<T>>;

template <typename T>
using UniquePtr = std::unique_ptr<T>;

void test_type_alias()
{
    std::cout << "===== Test Type Alias =====" << std::endl;

    // 普通类型别名使用
    IntVec_Typedef vec1 = {1, 2, 3, 4, 5};
    std::cout << "IntVec_Using size: " << vec1.size() << std::endl;

    // 别名模板使用
    Vec<std::string> vec2 = {"C++11", "Type Alias", "Alias"};
    Lst<int> lst = {10, 20, 30};

    UniquePtr<int> ptr = std::make_unique<int>(100);

    std::cout << "Vec<std::string> content: ";
    for (const auto &s : vec2)
    {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    std::cout << "UniquePtr value: " << *ptr << std::endl;
    std::cout << std::endl;
}

// ======================== 2. noexcept（异常说明符） ========================
/**
 * noexcept 作用：
 * 1. 声明函数不会抛出任何异常，编译器可做优化
 * 2. 移动构造/赋值标记为noexcept后，STL容器优先使用移动语义
 * 3. 若noexcept函数实际抛异常，程序会调用std::terminate终止
 */

class NoexceptDemo
{
private:
    int *data = nullptr;
    static const size_t DATA_SIZE = 10;

public:
    NoexceptDemo()
    {
        data = new int[DATA_SIZE]();
        std::cout << "NoexceptDemo default constructor" << std::endl;
    }

    // 拷贝构造：可能抛异常（内存分配失败），不标记noexcept
    NoexceptDemo(const NoexceptDemo &rhs)
    {
        std::cout << "NoexceptDemo copy constructor" << std::endl;
        data = new int[DATA_SIZE]; // 可能抛出bad_alloc异常
        std::copy(rhs.data, rhs.data + DATA_SIZE, data);
    }

    // 移动构造：仅转移指针，不会抛异常，标记noexcept
    NoexceptDemo(NoexceptDemo &&rhs) noexcept
    {
        std::cout << "NoexceptDemo move constructor" << std::endl;
        data = rhs.data;    // 转移资源
        rhs.data = nullptr; // 避免双重删除
    }

    // 拷贝赋值：可能抛异常
    NoexceptDemo &operator=(const NoexceptDemo &rhs)
    {
        if (this == &rhs)
            return *this; // 自赋值检查
        std::cout << "NoexceptDemo copy assignment operator" << std::endl;
        delete[] data;
        data = new int[DATA_SIZE]; // 可能抛出bad_alloc异常
        std::copy(rhs.data, rhs.data + DATA_SIZE, data);
        return *this;
    }

    // 移动赋值：不抛异常，标记noexcept
    NoexceptDemo &operator=(NoexceptDemo &&rhs) noexcept
    {
        if (this == &rhs)
            return *this; // 自赋值检查
        std::cout << "NoexceptDemo move assignment operator" << std::endl;
        delete[] data;
        data = rhs.data;    // 转移资源
        rhs.data = nullptr; // 避免双重删除
        return *this;
    }

    // 析构函数：默认noexcept
    ~NoexceptDemo()
    {
        delete[] data;
        std::cout << "NoexceptDemo: Destructor" << std::endl;
    }

    // 条件式noexcept：仅当T是平凡可拷贝时，函数不抛异常
    template <typename T>
    void conditional_noexcept(T &&val) noexcept(std::is_trivially_copyable<T>::value)
    {
        std::cout << "Conditional noexcept function called" << std::endl;
    }
};

void test_noexcept()
{
    std::cout << "==== Test noexcept ======" << std::endl;

    // 测试移动语义：STL容器优先使用移动构造/赋值
    std::vector<NoexceptDemo> vec;
    vec.reserve(2);

    // 插入临时对象，触发移动构造（因移动构造是noexcept）
    vec.push_back(NoexceptDemo());
    vec.emplace_back(); // 直接构造在容器内

    // 测试条件式noexcept
    NoexceptDemo demo;
    demo.conditional_noexcept(10);                   // int 是平凡可拷贝，函数noexcept
    demo.conditional_noexcept(std::string("Hello")); // std::string 不是平凡可拷贝，函数可能抛异常

    std::cout << std::endl;
}

// ======================== 3. override（虚函数重写校验） ========================
/**
 * override 作用：
 * 1. 显式标记派生类重写基类的虚函数
 * 2. 编译器校验：确保函数名、参数、const/volatile、返回值完全匹配
 * 3. 避免笔误导致的隐藏bug（如参数类型不匹配、函数名拼写错误）
 */

class BaseClass
{
public:
    // 带int参数虚函数 const限定
    virtual void show(int val) const
    {
        std::cout << "BaseClass::show(int) called with val = " << val << std::endl;
    }
    // 普通虚函数
    virtual void print()
    {
        std::cout << "BaseClass::print() called" << std::endl;
    }
    virtual ~BaseClass() = default; // 虚析构函数
};

// 错误示例：无override，隐藏重写错误
class WrongDerived : public BaseClass
{
public:
    // 笔误：参数是double而非int，实际是新函数，不是重写
    // 无override时编译器不报错，运行时调用基类函数，隐藏bug
    virtual void show(double val) const
    {
        std::cout << "WrongDerived: show(" << val << ") [double, const] (NOT override)" << std::endl;
    }
};

// 正确示例：使用override，编译器校验重写正确性
class CorrectDerived : public BaseClass
{
public:
    // 加override后，编译器强制校验：参数、const必须与基类一致
    // 若写成show(double val) const override，会直接编译报错
    void show(int val) const override
    {
        std::cout << "CorrectDerived: show(" << val << ") [const] (override)" << std::endl;
    }

    // 重写print函数，加override明确意图
    void print() override
    {
        std::cout << "CorrectDerived: print() (override)" << std::endl;
    }
};

void test_override()
{
    std::cout << "==== Test override ======" << std::endl;

    BaseClass *basePtr1 = new WrongDerived();

    // 调用show函数，WrongDerived的show(double)不会被调用，调用基类的show(int)，隐藏bug
    basePtr1->show(42); // 实际调用BaseClass::show(int)，输出基类版本
    delete basePtr1;

    BaseClass *basePtr2 = new CorrectDerived();
    // 调用show函数，CorrectDerived正确重写，调用派生类版本
    basePtr2->show(42); // 输出CorrectDerived版本
    basePtr2->print();  // 输出CorrectDerived版本
    delete basePtr2;

    std::cout << std::endl;
}

// ======================== 4. final（禁止继承/重写） ========================
/**
 * final 作用：
 * 1. 修饰类：禁止该类被继承
 * 2. 修饰虚函数：禁止派生类重写该函数
 * 3. 明确设计意图，编译器可做优化（如虚函数调用转为直接调用）
 */

// 示例1：final修饰类，禁止继承
class FinalClass final
{
public:
    void func()
    {
        std::cout << "FinalClass: This class cannot be inherited" << std::endl;
    }
};

// 编译错误：无法从final类继承
// class DerivedFromFinal : public FinalClass {};

// 示例2：final修饰虚函数，禁止重写
class BaseFinal
{
public:
    // final修饰虚函数，禁止派生类重写
    virtual void show() final
    {
        std::cout << "BaseFinal: show() [final, cannot be overridden]" << std::endl;
    }

    // 普通虚函数，可被重写
    virtual void print()
    {
        std::cout << "BaseFinal: print()" << std::endl;
    }
    virtual void demo()
    {
        std::cout << "BaseFinal: demo()" << std::endl;
    }
    virtual ~BaseFinal() = default;
};

class DerivedFinal : public BaseFinal
{
public:
    // 编译错误：无法重写final函数
    // void show() override {}

    // 合法：print未被final修饰，可重写
    void print() override
    {
        std::cout << "DerivedFinal: print() (override, show() is final)" << std::endl;
    }

    // final与override配合：先标记重写，再禁止后续派生类重写
    virtual void demo() override final
    {
        std::cout << "DerivedFinal: demo() [override + final]" << std::endl;
    }
};

// 测试final的函数
void test_final()
{
    std::cout << "===== Test final =====" << std::endl;

    // final类的使用
    FinalClass final_obj;
    final_obj.func();

    // final虚函数的使用
    BaseFinal *ptr = new DerivedFinal();
    ptr->show();  // 调用BaseFinal::show()（final，无法重写）
    ptr->print(); // 调用DerivedFinal::print()（可重写）
    ptr->demo();  // 调用DerivedFinal::demo()（override + final）
    delete ptr;

    std::cout << std::endl;
}

// ======================== 主函数：执行所有测试 ========================
int main()
{
    // 执行Type Alias测试
    test_type_alias();

    // 执行noexcept测试
    test_noexcept();

    // 执行override测试
    test_override();

    // 执行final测试
    test_final();

    return 0;
}