#pragma once
#include "variadicTemplate_printX.hpp"
#include "variadicTemplate_hash.hpp"
#include "variadicTemplate_tuple.hpp"
void testVaridicTemplatePrintX();
void testVariadicTemplateHash();
void testVariadicTemplateTuple();
void test();

void testVaridicTemplatePrintX()
{
    cout << "------------TestVaridicPrintX-------------" << endl;
    PrintX(7.5, "hello", bitset<16>(377), 42);
    cout << "==========================================" << endl
         << endl;
}

void testVariadicTemplateHash()
{
    cout << "------------TestVaridicHash-------------" << endl;
    Customer c1("John", "Doe", 123);
    Customer c2("Jane", "Smith", 456);
    Customer c3("John", "Doe", 123); // Same as c1
    CustomerHash hashFunc;
    cout << "Hash value of c1: " << hashFunc(c1) << endl;
    cout << "Hash value of c2: " << hashFunc(c2) << endl;
    cout << "Hash value of c3: " << hashFunc(c3) << endl; // Should be the same as c1
    cout << "==========================================" << endl
         << endl;
}

void testVariadicTemplateTuple()
{
    cout << "------------TestVaridicTuple-------------" << endl;
    Tuple<int, float, std::string> t(42, 3.14, "hello");
    cout << t.head() << endl;               // 输出 42
    cout << t.tail().head() << endl;        // 输出 3.14
    cout << t.tail().tail().head() << endl; // 输出 "hello"
    cout << "==========================================" << endl
         << endl;
}

void testPrintf()
{
    cout << "------------TestPrintf-------------" << endl;
    printf("Hello, %s! You have %d new messages.\n", "Alice", 5);
    cout << "==========================================" << endl;
}
void test()
{
    testVaridicTemplatePrintX();
    testVariadicTemplateHash();
    testVariadicTemplateTuple();
    testPrintf();
}