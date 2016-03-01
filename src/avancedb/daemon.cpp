/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2016 Ripcord Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "daemon.h"

#include <iostream>
#include <fstream>
#include <exception>

#include <cstdlib>
#include <cstdio>

#include <unistd.h>

void Daemon::Daemonize() {
    auto pid = fork();
    if (pid < 0) {
        std::cerr << "ERROR: unable to fork the daemon process" << std::endl;
    } else if (pid > 0) {
        exit(0);
    }
    
    setsid();   
    
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
    freopen("/dev/null", "r", stdin);
}

void Daemon::RedirectStdout(const char* file) {
    freopen(file, "w", stdout);
}

void Daemon::RedirectStderr(const char* file) {
    freopen(file, "w", stderr);   
}

void Daemon::WritePid(const char* file) { 
    
    std::ofstream stream;
    stream.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    stream.open(file, std::ofstream::out | std::ofstream::trunc);
    stream << getpid();
}

void Daemon::ChangeDir(const char* dir) {
    chdir(dir);
}