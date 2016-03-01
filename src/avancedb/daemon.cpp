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

#include <boost/format.hpp>

class DaemonException : public std::exception {
public:
    DaemonException(const char* msg) : msg(msg) {}
    DaemonException(const std::string& msg) : msg(msg) {}
    
    virtual const char* what() const noexcept override { return msg.c_str(); }
        
private:
    std::string msg;
};

bool Daemon::Daemonize(const std::string& stdoutFile, const std::string& stderrFile) {
    auto pid = fork();
    if (pid < 0) {
        auto msg = boost::format("Unable to fork the daemon process, the errno was %1%") % errno;
        throw DaemonException(msg.str());
    } else if (pid == 0) {
        if (setsid() < 0) {
            auto msg = boost::format(
                "Unable to create a new session after the fork, the errno was: %1%") % errno;
            throw DaemonException(msg.str());
        }

        RedirectStdout(stdoutFile.size() > 0 ? stdoutFile : "/dev/null");
        RedirectStderr(stderrFile.size() > 0 ? stderrFile : "/dev/null");
        std::freopen("/dev/null", "r", stdin);
    }
    
    // the pid is zero when we are the child of the fork
    return pid == 0;
}

void Daemon::RedirectStdout(const std::string& filename) {
    if (filename.size() > 0) {
        auto file = std::fopen(filename.c_str(), "w");
        if (file == nullptr) {
            auto msg = boost::format(
                "Unable to open '%1%' for stdout redirect, the errno was: %2%") %
                filename % errno;
            throw DaemonException(msg.str());
        }

        std::fclose(file);

        if (std::freopen(filename.c_str(), "w", stdout) == NULL) {
            auto msg = boost::format(
                "Unable to redirect stdout to '%1%', the errno was: %2%") %
                filename % errno;
            throw DaemonException(msg.str());
        }
    }
}

void Daemon::RedirectStderr(const std::string& filename) {
    if (filename.size() > 0) {
        auto file = std::fopen(filename.c_str(), "w");
        if (file == nullptr) {
            auto msg = boost::format(
                "Unable to open '%1%' for stderr redirect, the errno was: %2%") %
                filename % errno;
            throw DaemonException(msg.str());
        }

        std::fclose(file);

        if (std::freopen(filename.c_str(), "w", stderr) == NULL) {
            auto msg = boost::format(
                "Unable to redirect stderr to '%1%', the errno was: %2%") %
                filename % errno;
            throw DaemonException(msg.str());
        }     
    }
}

void Daemon::WritePid(const std::string& filename) {
    if (filename.size() > 0) {
        try {
            std::ofstream stream;
            stream.exceptions(std::ofstream::failbit | std::ofstream::badbit);
            stream.open(filename, std::ofstream::out | std::ofstream::trunc);
            stream << getpid();
        } catch (const std::exception& ex) {
            auto msg = boost::format(
                "Unable to write the pid to file '%1%', the error reported was: '%2%'") %
                filename % ex.what();
            throw DaemonException(msg.str());
        }
    }
}

void Daemon::ChangeDir(const std::string& dir) {
    if (dir.size() > 0 && chdir(dir.c_str()) != 0) {
        auto msg = boost::format(
            "Unable to change to directory '%1%', the errno was %2%") %
            dir % errno;
        throw DaemonException(msg.str());
    }
}