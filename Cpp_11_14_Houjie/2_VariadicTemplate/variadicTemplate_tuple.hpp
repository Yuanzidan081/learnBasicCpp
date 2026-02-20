#pragma once
template <typename... Values>
class Tuple;

template <>
class Tuple<>
{
};

template <typename Head, typename... Tail>
class Tuple<Head, Tail...> : private Tuple<Tail...>
{
    typedef Tuple<Tail...> inherited;

protected:
    Head m_head;

public:
    Tuple() {}
    Tuple(Head v, Tail... vtail) : m_head(v), inherited(vtail...) {}

    auto head() -> decltype(m_head) { return m_head; }
    inherited &tail()
    {
        return *this; // 通过隐式类型转换转换为基类引用
    }
};