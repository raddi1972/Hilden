
#ifndef  PARSER_HH
#define  PARSER_HH

#include <string>
#include <assert.h>

enum ValueType{
        INTEGER, 
        FLOAT,
        CHAR,
        STRING,
        VOID
    };
class Number {
public:
    Number() : m_type(ValueType::VOID) {}
    Number(const std::string& lit, ValueType type);

public:
    int get_int() const {
        assert(m_type == ValueType::INTEGER);
        return int_val;
    }

    float get_float() const {
        assert(m_type == ValueType::FLOAT);
        return  float_val;
    }

    char get_char() const {
        assert(m_type == ValueType::CHAR);
        return char_val;
    }

    const std::string& get_string() const {
        assert(m_type == ValueType::STRING);
        return string_val;
    }

    ValueType get_type() const {
        return m_type;
    }

    ~Number() { destroy();}
    Number(const Number& val) {
        copy(val);
    }
    Number& operator=(const Number& val) {
        if(&val != this) {
            destroy();
            copy(val);
        }
        return *this;
    }
private:
    template<typename T>
    void init(T& member, const T& val);

    void copy(const Number& val);
    void destroy();

private:
    ValueType m_type;
    union {
        int int_val;
        float float_val;
        char char_val;
        std::string string_val;
    };
};

#endif