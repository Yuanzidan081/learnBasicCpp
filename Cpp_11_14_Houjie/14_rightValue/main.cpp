#include <iostream>
#include <cstring>
#include <chrono>
#include <vector>
using namespace std;

class MyString
{
public:
    static bool DebugLog;

private:
    char *_data;

public:
    // 默认构造
    MyString() : _data(nullptr)
    {
        cout << "MyString default constructor" << endl;
    }

    // 带参构造
    explicit MyString(const char *str)
    {
        if (str)
        {
            _data = new char[strlen(str) + 1];
            strcpy(_data, str);
        }
        else
        {
            _data = nullptr;
        }
        if (DebugLog)
            cout << "MyString constructor(const char *) " << endl;
    }

    // 拷贝构造函数（深拷贝）
    MyString(const MyString &other)
    {
        if (other._data)
        {
            _data = new char[strlen(other._data) + 1];
            strcpy(_data, other._data);
        }
        else
            _data = nullptr;
        if (DebugLog)
            cout << "MyString copy constructor" << endl;
    }

    // 移动构造函数（转移资源）
    MyString(MyString &&other) noexcept : _data(other._data)
    {
        other._data = nullptr;
        if (DebugLog)
            cout << "MyString move constructor" << endl;
    }

    // 拷贝赋值运算符
    MyString &operator=(const MyString &other)
    {
        if (this != &other)
        {
            delete[] _data;
            if (other._data)
            {
                _data = new char[strlen(other._data) + 1];
                strcpy(_data, other._data);
            }
            else
            {
                _data = nullptr;
            }
        }
        if (DebugLog)
            cout << "MyString copy assignment operator" << endl;
        return *this;
    }

    // 移动赋值运算符
    MyString &operator=(MyString &&other) noexcept
    {
        if (this != &other)
        {
            delete[] _data;
            _data = other._data;
            other._data = nullptr;
        }
        if (DebugLog)
            cout << "MyString move assignment operator" << endl;
        return *this;
    }

    // 析构函数
    ~MyString()
    {
        delete[] _data;
        if (DebugLog)
            cout << "MyString destructor" << endl;
    }

    const char *c_str() const { return _data ? _data : ""; }
};
bool MyString::DebugLog = false;

// 测试函数，演示右值引用和移动语义
template <typename M>
void test_moveable(M &&c, long &value)
{
    auto start = chrono::high_resolution_clock::now();
    for (long i = 0; i < value; ++i)
    {
        MyString str("test");
        c.insert(c.end(), std::move(str));
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time taken: " << duration.count() << " ms" << endl;
}

int main()
{
    // 1. 基础测试，验证拷贝和移动行为
    {
        MyString::DebugLog = true;
        MyString s1("hello");
        MyString s2 = s1;            // 拷贝构造
        MyString s3 = std::move(s1); // 移动构造，s1资源被转移到s3
        cout << "s2: " << s2.c_str() << endl;
        cout << "s3: " << s3.c_str() << endl;
    }

    // 2. 性能测试，对比vector插入时的拷贝和移动开销
    {
        MyString::DebugLog = false;
        const long N = 100000;
        vector<MyString> vec_copy;
        vector<MyString> vec_move;

        cout << "\n --- Test copy insertion ---" << endl;
        auto start_copy = chrono::high_resolution_clock::now();
        for (long i = 0; i < N; ++i)
        {
            MyString str("data");
            vec_copy.insert(vec_copy.end(), str);
        }
        auto end_copy = chrono::high_resolution_clock::now();
        auto dur_copy = chrono::duration_cast<chrono::milliseconds>(end_copy - start_copy);
        cout << "Copy insertion time: " << dur_copy.count() << " ms" << endl;

        auto start_move = chrono::high_resolution_clock::now();
        for (long i = 0; i < N; ++i)
        {
            MyString str("data");
            vec_move.insert(vec_move.end(), std::move(str));
        }
        auto end_move = chrono::high_resolution_clock::now();
        auto dur_move = chrono::duration_cast<chrono::milliseconds>(end_move - start_move);
        cout << "Move insertion time: " << dur_move.count() << " ms" << endl;
    }

    // template测试函数
    {
        MyString::DebugLog = false;
        long testSize = 10000;
        vector<MyString> vec;
        cout << "\n --- Calling test_moveable ---" << endl;
        test_moveable(vec, testSize);
    }
}