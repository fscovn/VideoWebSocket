/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vs_processing.cpp
 * Author: ubuntu
 * 
 * Created on May 25, 2020, 11:13 AM
 */

#include "vs_processing.h"

vs_processing::vs_processing() {
}
vs_processing::~vs_processing() {
}

string vs_processing::to_base64(Mat img) {
    vector<uchar> buf;
    imencode(".jpg", img, buf);
    uchar *enc_msg = new uchar[buf.size()];
    std::copy(buf.begin(), buf.end(), enc_msg);
    string encoded = base64_encode(enc_msg, buf.size());
    buf.clear();
    delete enc_msg;
    return encoded;
}