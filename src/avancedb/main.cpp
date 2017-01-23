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

#include "http_server.h"
#include "map_reduce_thread_pool.h"
#include "daemon.h"
#include "config.h"

int main(int argc, const char** argv) {
    try {
        Config::Parse(argc, argv);

        if (Config::IsHelp()) {
            std::cout << Config::Description() << std::endl;
        } else if (!Config::IsDaemon() || Daemon::Daemonize(Config::Process::StdOutFile(), Config::Process::StdErrFile())) {
            Daemon::RedirectStdout(Config::Process::StdOutFile());
            Daemon::RedirectStderr(Config::Process::StdErrFile());

            Daemon::WritePid(Config::Process::PidFile());
            Daemon::ChangeDir(Config::Process::RootDirectory());

            MapReduceThreadPoolScope threadPool{Config::SpiderMonkey::HeapSize(), Config::SpiderMonkey::EnableBaselineCompiler(), Config::SpiderMonkey::EnableIonCompiler()};

            HttpServer server(Config::Http::Address().c_str(), Config::Http::Port());
            server.Start();
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        return 1;
    }
}