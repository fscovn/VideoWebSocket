/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Config.h
 * Author: ubuntu
 *
 * Created on April 9, 2020, 9:23 AM
 */
#ifndef LOCAL_CONFIG
#define LOCAL_CONFIG
#include <string>
#include <iostream>
#include <vector>
#include "tinyxml2.h"
using namespace std;
using std::vector;

class vs_config {
public:
    vs_config(string xmlPath);
    virtual ~vs_config();    
    
public:
    int listen_port;
    int step;
    float resized;
private:
    
};

#endif