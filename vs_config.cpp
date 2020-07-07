/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Config.cpp
 * Author: ubuntu
 * 
 * Created on April 9, 2020, 9:23 AM
 */

#include <sys/socket.h>

#include "vs_config.h"
using namespace tinyxml2;

vs_config::vs_config(string xmlPath) {
    XMLDocument xmlDoc;
    xmlDoc.LoadFile(xmlPath.c_str());
    if (xmlDoc.ErrorID() != 0)
        return;
    XMLElement *root = xmlDoc.FirstChildElement();
    if (root != NULL) {
        const char *stepStr = root->FirstChildElement("step")->GetText();
        step = atoi(stepStr);
        const char *fresizeStr = root->FirstChildElement("resize")->GetText();
        resized = atof(fresizeStr);
        const char *lportStr = root->FirstChildElement("port")->GetText();
        listen_port = atoi(lportStr);
    }
}

vs_config::~vs_config() {
}