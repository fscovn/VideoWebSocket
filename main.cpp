/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: ubuntu
 *
 * Created on May 7, 2020, 10:57 AM
 */
#include <cstdlib>
#include "vs_server.h"

int main(int argc, char** argv) {
    vs_config *config = new vs_config("config.xml");
    vs_server *server = new vs_server(config);
    server->start_listen();
    return 0;
}