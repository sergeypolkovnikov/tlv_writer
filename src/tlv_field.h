#pragma once
#include <variant>
#include <string>

struct TLVField {
    static TLVField from_string(std::string val);
    static TLVField from_int(int val);
    static TLVField from_float(float val);
    static TLVField from_bool(bool val);

    friend std::basic_ostream<char>& operator << (std::basic_ostream<char>& stream, TLVField const& field);

private:
    template<typename T>
    static TLVField from(T&& val) {
        TLVField res;
        res.value = std::forward<T>(val);
        return res;
    }

    std::variant<std::string, int, float, bool> value;
};
