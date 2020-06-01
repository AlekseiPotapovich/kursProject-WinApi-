#pragma once


#include <string>


std::string base64_decode(std::string const& s, bool remove_linebreaks = false);

int convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n);
