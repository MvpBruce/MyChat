#pragma once
#include <iomanip>
#include <sstream>
#include <string>

std::string url_encode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
    }

    return escaped.str();
}
