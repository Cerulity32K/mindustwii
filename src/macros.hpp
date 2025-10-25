#define IMPL_CASTED_BINARY_OPERATOR(type, repr, op)                            \
    inline constexpr type operator op(type lhs, type rhs) {                    \
        return static_cast<type>(static_cast<repr>(lhs)                        \
                                     op static_cast<repr>(rhs));               \
    }

#define IMPL_COMPOUND_ASSIGNMENT_OPERATOR(type, repr, op, compound)            \
    inline constexpr type& operator compound(type & lhs, type rhs) {           \
        lhs = lhs op rhs;                                                      \
        return lhs;                                                            \
    }

#define IMPL_CASTED_UNARY_OPERATOR(type, repr, op)                             \
    inline constexpr type operator op(type x) {                                \
        return static_cast<type>(op static_cast<repr>(x));                     \
    }

#define IMPL_CASTED_NEGATION_OPERATOR(type, repr)                              \
    inline constexpr bool operator!(type x) { return !static_cast<repr>(x); }

#define IMPL_ENUM_CLASS_FLAGS(type, repr)                                      \
    IMPL_CASTED_BINARY_OPERATOR(type, repr, &)                                 \
    IMPL_COMPOUND_ASSIGNMENT_OPERATOR(type, repr, &, &=)                       \
    IMPL_CASTED_BINARY_OPERATOR(type, repr, |)                                 \
    IMPL_COMPOUND_ASSIGNMENT_OPERATOR(type, repr, |, |=)                       \
    IMPL_CASTED_BINARY_OPERATOR(type, repr, ^)                                 \
    IMPL_COMPOUND_ASSIGNMENT_OPERATOR(type, repr, ^, ^=)                       \
    IMPL_CASTED_UNARY_OPERATOR(type, repr, ~)                                  \
    IMPL_CASTED_NEGATION_OPERATOR(type, repr)
