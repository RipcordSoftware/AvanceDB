/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015-2017 Ripcord Software
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

#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "http_server.h"
#include "map_reduce_thread_pool.h"
#include "daemon.h"
#include "config.h"

int main(int argc, char** argv) {
    try {
        std::string addr = "0.0.0.0";
        unsigned port = 5994;
        std::string pidFile;
        std::string stdoutFile;
        std::string stderrFile;
        std::string rootDirectory;
        auto isUserProcess = false;

        boost::program_options::options_description desc("Program options");
        desc.add_options()
            ("help,h", "shows the program options")
            ("address,a", boost::program_options::value(&addr)->default_value(addr), "the IP address to listen on")
            ("port,p", boost::program_options::value(&port)->default_value(port), "the TCP/IP port to listen on")
            ("daemon,d", "daemonize the process")
            ("pid", boost::program_options::value(&pidFile), "writes the process id to a file")
            ("out,o", boost::program_options::value(&stdoutFile), "writes stdout to a file")
            ("err,e", boost::program_options::value(&stderrFile), "writes stderr to a file")
            ("dir", boost::program_options::value(&rootDirectory), "sets the working directory")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
        } else if ((isUserProcess = vm.count("daemon") == 0) || Daemon::Daemonize(stdoutFile, stderrFile)) {
            Daemon::WritePid(pidFile);
            Daemon::ChangeDir(rootDirectory);

            if (isUserProcess) {
                Daemon::RedirectStdout(stdoutFile);
                Daemon::RedirectStderr(stderrFile);
            }

            MapReduceThreadPoolScope threadPool{Config::SpiderMonkey::GetHeapSize(), Config::SpiderMonkey::GetEnableBaselineCompiler(), Config::SpiderMonkey::GetEnableIonCompiler()};

            HttpServer server(addr.c_str(), port);
            server.Start();
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        return 1;
    }
}