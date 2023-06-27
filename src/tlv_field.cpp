#include "tlv_field.h"
#include <ostream>

TLVField TLVField::from_string(std::string val) {
    return TLVField::from(std::move(val));
}

TLVField TLVField::from_int(int val) {
    return TLVField::from(val);
}

TLVField TLVField::from_float(float val) {
    return TLVField::from(val);
}

TLVField TLVField::from_bool(bool val) {
    return TLVField::from(val);
}

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

std::ostream& operator << (std::basic_ostream<char>& stream, TLVField const& field) {
    stream << static_cast<char>(field.value.index());
    std::visit(overloaded{
        [&](std::string const& str) { stream << str.size() + 1; stream << str; }
        , [&](const auto val) { stream << val; }
        }, field.value);
    return stream;
}