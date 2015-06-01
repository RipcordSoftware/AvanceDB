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