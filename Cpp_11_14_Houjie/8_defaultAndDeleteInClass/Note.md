在C++11标准中，新增了`default`和`delete`两个关键字，用于精细化控制类的特殊成员函数（默认构造函数、析构函数、拷贝构造函数、拷贝赋值运算符、移动构造函数、移动赋值运算符）的生成行为。这两个关键字解决了传统C++中默认函数控制不灵活的问题，是现代C++类设计的重要特性。

### 1. 核心概念：类的特殊成员函数
C++类默认会自动生成以下6个特殊成员函数（满足条件时）：
- 默认构造函数（无参构造）
- 析构函数
- 拷贝构造函数
- 拷贝赋值运算符（`operator=`）
- 移动构造函数（C++11新增）
- 移动赋值运算符（C++11新增）

`default`和`delete`用于显式指定：**让编译器生成默认版本** 或 **禁止生成该函数**。

### 2. `default`：显式要求编译器生成默认函数
#### 2.1 适用场景
- 当类中自定义了其他构造函数，编译器不再自动生成默认构造函数，可通过`default`手动恢复。
- 希望编译器生成高效的默认版本（比手动编写的更优，尤其是移动语义相关函数）。
- 显式声明函数为`default`，增强代码可读性，明确表达“使用默认实现”的意图。

#### 2.2 语法规则
- `default`只能用于类的特殊成员函数，或析构函数。
- 可在类内声明时使用，也可在类外定义时使用。
- 被`default`修饰的函数会被编译器标记为“trivial”（平凡），利于编译器优化。

#### 2.3 代码示例
```cpp
#include <iostream>
#include <utility> // 用于std::move

class MyClass {
public:
    // 1. 恢复默认构造函数（因自定义了带参构造，编译器不再自动生成）
    MyClass() = default;

    // 2. 自定义带参构造函数
    MyClass(int val) : value(val) {}

    // 3. 显式要求编译器生成默认拷贝构造函数
    MyClass(const MyClass&) = default;

    // 4. 显式要求编译器生成默认拷贝赋值运算符
    MyClass& operator=(const MyClass&) = default;

    // 5. 显式要求编译器生成默认移动构造函数（C++11）
    MyClass(MyClass&&) = default;

    // 6. 显式要求编译器生成默认移动赋值运算符（C++11）
    MyClass& operator=(MyClass&&) = default;

    // 7. 析构函数显式使用默认版本
    ~MyClass() = default;

    void print() const {
        std::cout << "Value: " << value << std::endl;
    }

private:
    int value = 0;
};

int main() {
    MyClass a; // 调用默认构造函数
    MyClass b(10); // 调用带参构造函数
    MyClass c = b; // 调用默认拷贝构造函数
    MyClass d = std::move(b); // 调用默认移动构造函数
    a = c; // 调用默认拷贝赋值运算符
    d = std::move(a); // 调用默认移动赋值运算符

    d.print(); // 输出：Value: 10
    return 0;
}
```

### 3. `delete`：显式禁止生成某个函数
#### 3.1 适用场景
- 禁止类的拷贝/移动行为（如单例类、只移动类型）。
- 禁止特定参数类型的重载函数（如禁止隐式类型转换）。
- 禁止编译器生成默认的特殊成员函数。

#### 3.2 语法规则
- `delete`可用于任何函数（包括普通成员函数、非成员函数），不限于特殊成员函数。
- 被`delete`修饰的函数被标记为“deleted”，调用该函数会触发编译错误。
- 可在类内声明时直接使用`delete`。

#### 3.3 代码示例
##### 示例1：禁止拷贝构造和拷贝赋值（只移动类）
```cpp
#include <iostream>
#include <string>

class NonCopyable {
public:
    // 构造函数
    NonCopyable(std::string msg) : message(std::move(msg)) {}

    // 禁止拷贝构造函数
    NonCopyable(const NonCopyable&) = delete;

    // 禁止拷贝赋值运算符
    NonCopyable& operator=(const NonCopyable&) = delete;

    // 允许移动构造
    NonCopyable(NonCopyable&&) = default;

    // 允许移动赋值
    NonCopyable& operator=(NonCopyable&&) = default;

    void show() const {
        std::cout << message << std::endl;
    }

private:
    std::string message;
};

int main() {
    NonCopyable obj1("Hello, C++11");
    // NonCopyable obj2 = obj1; // 编译错误：拷贝构造函数已删除
    // obj1 = obj2; // 编译错误：拷贝赋值运算符已删除

    NonCopyable obj2 = std::move(obj1); // 合法：调用移动构造
    obj2.show(); // 输出：Hello, C++11

    return 0;
}
```

##### 示例2：禁止特定重载函数（避免隐式类型转换）
```cpp
#include <iostream>

class Number {
public:
    // 接受int类型的构造函数
    Number(int num) : value(num) {}

    // 禁止接受double类型的构造函数（避免隐式转换）
    Number(double) = delete;

    int getValue() const { return value; }

private:
    int value;
};

void printNumber(const Number& n) {
    std::cout << n.getValue() << std::endl;
}

int main() {
    Number n1(10); // 合法：调用int版本构造函数
    printNumber(n1); // 输出：10

    // Number n2(3.14); // 编译错误：double版本构造函数已删除
    // printNumber(3.14); // 编译错误：无法隐式转换为Number（double构造被删除）

    return 0;
}
```

##### 示例3：禁止类的默认构造（强制使用带参构造）
```cpp
class OnlyParamCtor {
public:
    // 禁止默认构造函数
    OnlyParamCtor() = delete;

    // 强制使用带参构造
    OnlyParamCtor(int val) : data(val) {}

private:
    int data;
};

int main() {
    // OnlyParamCtor obj; // 编译错误：默认构造函数已删除
    OnlyParamCtor obj(5); // 合法
    return 0;
}
```

### 4. 关键注意事项
#### 4.1 `default`的限制
- 不能用于非特殊成员函数（如普通成员函数、全局函数）。
- 移动构造/移动赋值的`default`仅在类没有自定义拷贝构造、拷贝赋值、析构函数时才会生效（C++11规则）。

#### 4.2 `delete`的扩展用法
- 可用于静态成员函数、全局函数，实现“禁用某些调用方式”。
- 被`delete`的函数仍可用于重载决议（但调用时报错），可用于屏蔽特定类型的隐式转换。

#### 4.3 析构函数的特殊处理
- 析构函数标记为`default`时，若类的所有成员析构都是平凡的，则析构函数也是平凡的。
- 析构函数不能被`delete`（C++11及以后，若类被实例化则编译错误；仅声明不实例化可通过编译）。

### 5. C++14的补充
C++14对`default`/`delete`无核心语法变更，但扩展了`delete`的适用场景：
- 允许在函数模板中使用`delete`，实现对特定模板实例的禁用。
```cpp
template <typename T>
void func(T) {}

// 禁用T=int的版本
template <>
void func<int>(int) = delete;

int main() {
    // func(10); // 编译错误：int版本已删除
    func(3.14); // 合法：double版本
    return 0;
}
```

### 总结
`default`和`delete`是C++11引入的核心特性，解决了传统C++中“无法精细控制默认函数”的问题：
- `default`：显式复用编译器生成的高效默认实现，恢复被屏蔽的默认函数。
- `delete`：禁止特定函数的生成或调用，实现“只移动”“禁止拷贝”“强制带参构造”等设计意图。

在现代C++类设计中，合理使用这两个关键字可提升代码的可读性、安全性和性能，是侯捷老师强调的“现代C++类设计规范”的重要组成部分。