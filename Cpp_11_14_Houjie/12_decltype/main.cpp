#include <iostream>
#include <vector>
#include <map>
#include <utility>     // std::forward、std::declval
#include <typeinfo>    // typeid
#include <type_traits> // std::remove_const_t, std::remove_reference_t

#ifdef _WIN32
#include <windows.h>
void set_console_utf8()
{
    // 设置控制台输入/输出编码为UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
}
#else
// Linux/Mac无需处理
void set_console_utf8() {}
#endif

// ======================== 1. decltype 基础推导规则 ========================
/**
 * decltype 核心特性：
 * 1. 推导表达式的"原始类型"，保留const/volatile、引用等所有特性
 * 2. 工作在未求值上下文：仅推导类型，不执行表达式
 * 3. 双层括号(())会将普通变量推导为左值引用
 */
void basic_decltype_deduction()
{
    std::cout << "===== 1. 基础推导规则 =====" << std::endl;

    // 1.1 普通变量/常量的推导
    int a = 10;
    const int b = 20;
    int &c = a;          // c是int的左值引用
    volatile int d = 30; // d是volatile int

    decltype(a) t1 = 100; // t1: int（普通变量，推导为原始类型）
    decltype(b) t2 = 200; // t2: const int（保留const特性）
    decltype(c) t3 = a;   // t3: int&（保留引用特性，必须初始化）
    decltype(d) t4 = 300; // t4: volatile int（保留volatile特性）

    std::cout << "t1 type: " << typeid(t1).name() << " (int)" << std::endl;
    std::cout << "t2 type: " << typeid(t2).name() << " (const int)" << std::endl;
    std::cout << "t3 type: " << typeid(t3).name() << " (int&)" << std::endl;
    std::cout << "t4 type: " << typeid(t4).name() << " (volatile int)" << std::endl;

    // 1.2 表达式推导（未求值，仅推导类型）
    decltype(a + b) t5 = a + b; // t5: int（a+b是int，忽略b的const）
    decltype((a)) t6 = a;       // t6: int&（双层括号表示左值表达式，推导为引用）
    decltype(100) t7 = 400;     // t7: int（右值表达式，推导为原始类型）

    std::cout << "t5 type: " << typeid(t5).name() << " (int，a+b的类型)" << std::endl;
    std::cout << "t6 type: " << typeid(t6).name() << " (int&，左值表达式推导为引用)" << std::endl;
    std::cout << "t7 type: " << typeid(t7).name() << " (int，右值表达式)" << std::endl;

    // 1.3 类成员推导（无需实例化对象）
    std::map<std::string, float> coll;
    // 等价于 std::map<std::string, float>::value_type
    decltype(coll)::value_type elem = {"test", 3.14f};
    std::cout << "elem type: pair<const string, float>，值：" << elem.first << " " << elem.second << std::endl;

    auto cmp = [](const int &x, const int &y)
    { return x < y; };
    std::map<int, std::string, decltype(cmp)> sorted_map(cmp); // 用Lambda表达式作为比较器
    std::cout << std::endl;
}

// ======================== 2. 函数返回类型推导（后置返回类型） ========================
/**
 * decltype + 后置返回类型(->)：
 * 解决"函数返回类型依赖参数类型"的问题，实现泛型函数的返回类型推导
 */
// 传统泛型加法：返回类型固定为T1，可能丢失精度
template <typename T1, typename T2>
T1 add_traditional(T1 a, T2 b)
{
    return a + b;
}

// 现代泛型加法：用decltype推导a+b的实际类型
template <typename T1, typename T2>
auto add_modern(T1 a, T2 b) -> decltype(a + b)
{
    return a + b; // 返回类型是a+b的真实类型（如int+double→double）
}

void test_return_type_deduction()
{
    std::cout << "===== 2. 函数返回类型推导 =====" << std::endl;

    int x = 5;
    double y = 3.14;

    // 传统写法缺陷：返回类型是int，3.14被截断
    auto res_trad = add_traditional(x, y);
    std::cout << "传统加法结果：" << res_trad << "，类型：" << typeid(res_trad).name() << " (int，精度丢失)" << std::endl;

    // 现代写法：返回类型是double，保留精度
    auto res_modern = add_modern(x, y);
    std::cout << "现代加法结果：" << res_modern << "，类型：" << typeid(res_modern).name() << " (double，精度保留)" << std::endl;

    std::cout << std::endl;
}

// ======================== 3. 元编程：获取Lambda类型 ========================
/**
 * Lambda表达式的类型是编译器生成的匿名类型，无法手动声明，只能通过decltype获取
 */
void test_lambda_type()
{
    std::cout << "===== 3. 元编程：Lambda类型推导 =====" << std::endl;

    // 定义Lambda表达式（匿名类型）
    auto lambda = [](int num) -> int
    {
        return num * 2;
    };

    // 用decltype获取Lambda类型，实现Lambda拷贝
    decltype(lambda) lambda_copy = lambda;
    int res = lambda_copy(10);
    std::cout << "Lambda调用结果：" << res << "（类型推导自decltype(lambda)）" << std::endl;

    std::cout << std::endl;
}

// ======================== 4. 简化复杂类型声明 ========================
/**
 * decltype 简化STL容器等复杂嵌套类型的声明，避免冗长代码
 */
void test_simplify_complex_type()
{
    std::cout << "===== 4. 简化复杂类型声明 =====" << std::endl;

    // 复杂嵌套容器
    std::vector<std::map<int, std::string>> complex_container;
    complex_container.push_back({{1, "C++"}, {2, "decltype"}});

    // 传统写法：冗长且易出错
    std::vector<std::map<int, std::string>>::iterator it1 = complex_container.begin();
    // decltype写法：简洁且不易出错
    decltype(complex_container)::iterator it2 = complex_container.begin();

    // 遍历容器，用decltype推导元素类型
    for (const auto &elem : complex_container)
    {
        // 推导map的key_type和mapped_type
        using MapType = std::remove_const_t<std::remove_reference_t<decltype(elem)>>;
        MapType::key_type key = 1;
        MapType::mapped_type val = "test";
        std::cout << "map key类型：" << typeid(key).name() << " (int)，value类型：" << typeid(val).name() << " (string)" << std::endl;
    }

    std::cout << "it1与it2类型是否一致：" << (typeid(it1) == typeid(it2) ? "是" : "否") << std::endl;
    std::cout << std::endl;
}

// ======================== 5. 完美转发（Perfect Forwarding） ========================
/**
 * decltype + std::forward：
 * 保留参数的左值/右值特性，实现完美转发，是泛型函数的核心优化
 */
// 重载函数：区分左值/右值参数
void print(int &x)
{
    std::cout << "左值参数：" << x << std::endl;
}
void print(int &&x)
{
    std::cout << "右值参数：" << x << std::endl;
}

// 泛型转发函数：保留参数的左值/右值特性
template <typename Func, typename... Args>
auto forward_func(Func &&func, Args &&...args) -> decltype(func(std::forward<Args>(args)...))
{
    return func(std::forward<Args>(args)...);
}

void test_perfect_forwarding()
{
    std::cout << "===== 5. 完美转发 =====" << std::endl;

    int a = 10;
    forward_func(static_cast<void (*)(int &)>(print), a);   // 转发左值，调用print(int&)
    forward_func(static_cast<void (*)(int &&)>(print), 20); // 转发右值，调用print(int&&)

    std::cout << std::endl;
}

// ======================== 6. decltype vs auto 对比 ========================
/**
 * decltype：推导"原始类型"（保留所有特性）
 * auto：推导"初始化类型"（丢弃顶层const/引用）
 */
void compare_auto_decltype()
{
    std::cout << "===== 6. decltype vs auto 对比 =====" << std::endl;

    const int &a = 10; // a: const int&

    auto b = a;          // b: int（auto丢弃const和引用）
    decltype(a) c = a;   // c: const int&（decltype保留const和引用）
    auto &d = a;         // d: const int&（auto+&显式保留引用）
    decltype((a)) e = a; // e: const int&（双层括号推导左值引用）

    std::cout << "auto b = a → b类型：" << typeid(b).name() << " (int，丢弃const/引用)" << std::endl;
    std::cout << "decltype(a) c = a → c类型：" << typeid(c).name() << " (const int&，保留特性)" << std::endl;
    std::cout << "auto& d = a → d类型：" << typeid(d).name() << " (const int&，显式保留引用)" << std::endl;
    std::cout << "decltype((a)) e = a → e类型：" << typeid(e).name() << " (const int&，左值表达式)" << std::endl;

    std::cout << std::endl;
}

// ======================== 7. 高级特性：declval 配合使用 ========================
/**
 * std::declval：将任意类型转为引用类型，无需实例化对象即可访问类成员
 * 解决"类无默认构造函数时，无法通过实例化推导成员类型"的问题
 */
// 无默认构造函数的类（必须传int初始化）
class NoDefaultCtor
{
public:
    NoDefaultCtor(int x) : val(x) {}    // 无默认构造函数
    int get_val() const { return val; } // 成员函数
private:
    int val;
};

void test_declval()
{
    std::cout << "===== 7. decltype + declval 高级用法 =====" << std::endl;

    // 错误：NoDefaultCtor无默认构造函数，无法实例化对象
    // NoDefaultCtor obj;
    // decltype(obj.get_val()) t1;

    // 正确：declval将NoDefaultCtor转为引用，无需实例化即可推导成员函数类型
    decltype(std::declval<NoDefaultCtor>().get_val()) t2 = 100;
    std::cout << "推导NoDefaultCtor::get_val()的返回类型：" << typeid(t2).name() << " (int)" << std::endl;

    std::cout << std::endl;
}

// ======================== 主函数：执行所有测试 ========================
int main()
{
    set_console_utf8();

    // 1. 基础推导规则
    basic_decltype_deduction();

    // 2. 函数返回类型推导
    test_return_type_deduction();

    // 3. Lambda类型推导
    test_lambda_type();

    // 4. 简化复杂类型声明
    test_simplify_complex_type();

    // 5. 完美转发
    test_perfect_forwarding();

    // 6. decltype vs auto 对比
    compare_auto_decltype();

    // 7. declval 高级用法
    test_declval();

    return 0;
}