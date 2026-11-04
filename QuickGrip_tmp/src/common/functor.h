#pragma once

namespace Common {

class Identity
{
public:
    template <typename T>
    T operator ()(const T& a) const
    {
        return a;
    }

};

inline Identity identity() { return Identity(); }

class Negation
{
public:
    template <typename T>
    T operator ()(const T& a) const
    {
        return !a;
    }

};

inline Negation negation() { return Negation(); }

#define BINARY_OPERATOR_FUNCTOR(FUNCTOR, FUNC, R, OP)   \
    template <typename T0>                              \
    class FUNCTOR  {                                    \
        public:                                         \
            FUNCTOR(const T0& a) : m_a(a) {}            \
            template <typename T1>                      \
            R operator ()(const T1& b) const {          \
                return m_a OP b;                        \
            }                                           \
        private:                                        \
            T0 m_a;                                     \
    };                                                  \
    template <typename T>                               \
    FUNCTOR<T> FUNC(T a) { return FUNCTOR<T>(a); }

BINARY_OPERATOR_FUNCTOR(Eq, eq, bool, ==)
BINARY_OPERATOR_FUNCTOR(Neq, neq, bool, !=)
BINARY_OPERATOR_FUNCTOR(Lt, lt, bool, >)
BINARY_OPERATOR_FUNCTOR(Lte, lte, bool, >=)
BINARY_OPERATOR_FUNCTOR(Gt, gt, bool, <)
BINARY_OPERATOR_FUNCTOR(Gte, gte, bool, <=)

#define BINARY_FUNCTOR_OPERATOR_FUNCTOR(FUNCTOR, FUNC, R, OP)   \
    template <typename F0, typename F1>                         \
    class FUNCTOR  {                                            \
        public:                                                 \
            FUNCTOR(F0 f0, F1 f1) : m_f0(f0), m_f1(f1) {}       \
            template <typename T0, typename T1>                 \
            R operator ()(const T0& a, const T1& b) const {     \
                return m_f0(a) OP m_f1(b);                      \
            }                                                   \
        private:                                                \
            F0 m_f0;                                            \
            F1 m_f1;                                            \
    };                                                          \
    template <typename F0, typename F1>                         \
    FUNCTOR<F0, F1> FUNC(F0 f0, F1 f1) { return FUNCTOR<F0, F1>(f0, f1); }

BINARY_FUNCTOR_OPERATOR_FUNCTOR(And, and, bool, &&)
BINARY_FUNCTOR_OPERATOR_FUNCTOR(Or, or, bool, ||)

#undef BINARY_FUNCTOR_OPERATOR_FUNCTOR

}

