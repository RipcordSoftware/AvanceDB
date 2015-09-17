/*
 *  AvanceDB - an in-memory database similar to Apache CouchDB
 *  Copyright (C) 2015 Ripcord Software
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

#include "http_server.h"

#include <string>

#include <boost/program_options.hpp>

int main(int argc, char** argv) {
    std::string addr = "0.0.0.0";
    unsigned port = 5994;
    
    boost::program_options::options_description desc("Program options");
    desc.add_options()
        ("help,h", "shows the program options")
        ("address,a", boost::program_options::value<std::string>(&addr)->default_value(addr), "the IP address to listen on")
        ("port,p", boost::program_options::value<unsigned>(&port)->default_value(port), "the TCP/IP port to listen on")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    } else {    
        HttpServer server(addr.c_str(), port);
        server.Start();
        return 0;
    }
}