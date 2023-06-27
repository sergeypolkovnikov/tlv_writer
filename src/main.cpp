#include <iostream>
#include <fstream>
#include <array>
#include <set>
#include <string_view>
#include "tlv_field.h"
#include <optional>
#include <algorithm>

#include "json.hpp"

enum class Key {
    Invalid = 0
};

std::basic_ostream<char>& operator << (std::basic_ostream<char>& stream, Key const& key) {
    return stream << static_cast<int>(key);
}

std::unordered_map<std::string, Key> read_map(std::ifstream& file) {
    if (file) {
        std::array<char, 1024> buff;
        file.getline(buff.data(), buff.size());
        const auto json_str = std::string_view{ buff.data() };
        const auto json = nlohmann::json::parse(json_str);
        std::unordered_map<std::string, Key> result;
        result.reserve(json.size());
        for (auto it = json.begin(); it != json.end(); ++it) {
            result[it.key()] = Key{ it.value() };
        }
        return result;
    }
    return std::unordered_map<std::string, Key>{}; // empty map
}

Key get_key(const std::unordered_map<std::string, Key>& map, std::string str) {
    const auto it = map.find(str);
    return it == map.end() ? Key::Invalid : it->second;
}

std::optional<TLVField> get_from_json(const nlohmann::json& json) {
    if (json.is_string()) {
        return TLVField::from_string(json.get<std::string>());
    }
    else if (json.is_number_integer()) {
        return TLVField::from_int(json.get<int>());
    }
    else if (json.is_number_float()) {
        return TLVField::from_float(json.get<float>());
    }
    else if (json.is_boolean()) {
        return TLVField::from_bool(json.get<bool>());
    }
    return std::nullopt;
}

std::string_view trim_left(std::string_view value, char trimmed) {
    const auto start = std::ranges::find_if(value, [&](char symbol) {
        return !std::isspace(symbol) && symbol != trimmed; });
    return std::string_view{ start, value.end() };
}

std::string_view trim_right(std::string_view value, char trimmed) {
    const auto end = std::find_if(value.rbegin(), value.rend(), [&](char symbol) {
        return !std::isspace(symbol) && symbol != trimmed; });
    const auto trim_size = std::distance(value.rbegin(), end);
    return std::string_view{ value.begin(), value.end() - trim_size };
}

std::string_view trim(std::string_view value, char trimmed) {
    return trim_left(trim_right(value, trimmed), trimmed);
}

void save_map_to_bin(const std::unordered_map<std::string, Key>& map, std::ofstream& out) {
    out << TLVField::from_int(static_cast<int>(map.size()));
    for (const auto& [key, value] : map) {
        out << TLVField::from_string(key);
        out << TLVField::from_int(static_cast<int>(value));
    }
}

void parse_and_save_to_bin(std::string_view str, const std::unordered_map<std::string, Key>& map, std::ofstream& out) {
    try {
        const auto json_str = trim(str, ',');
        if (!json_str.empty()) {
            const auto json = nlohmann::json::parse(json_str);
            for (const auto& el : json.items()) {
                const auto key = get_key(map, el.key());
                if (key != Key::Invalid) {
                    const auto field = get_from_json(el.value());
                    if (field) {
                        out << key;
                        out << field.value();
                    }
                }
            }
        }
    }
    catch (std::exception& ex) {
        std::cout << "Parse error: " << ex.what();
    }
}

int main(int argc, char* argv[]) {
    const auto input_file_name = argc >= 2 ? argv[1] : "input.json";
    const auto output_file_name = argc >= 3 ? argv[2] : "output.bin";
    std::ifstream input_file{ input_file_name };
    std::ofstream output_file{ output_file_name };
    const auto map = read_map(input_file);
    save_map_to_bin(map, output_file);
    while (input_file) {
        std::array<char, 1024> buff;
        input_file.getline(buff.data(), buff.size());
        parse_and_save_to_bin(std::string_view{ buff.data() }, map, output_file);
    }
}
