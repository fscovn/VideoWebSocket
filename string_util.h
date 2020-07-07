/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string_util.h
 * Author: ubuntu
 *
 * Created on May 14, 2020, 3:16 PM
 */

#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <iostream>
#include <string>

using namespace std;


class string_util {
public:
    static bool start_width(string str, string prefix) {
        return str.size() >= prefix.size() &&
                str.compare(0, prefix.size(), prefix) == 0;
    }
    static bool has_prefix(string str, string prefix) {
        return str.size() >= prefix.size() &&
                str.compare(str.size() - prefix.size(), prefix.size(), prefix) == 0;
    }


private:

};

#endif /* STRING_UTIL_H */

