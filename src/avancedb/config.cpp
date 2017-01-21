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

#include "config.h"

#include <algorithm>

#include <boost/thread.hpp>

const char Config::Http::DefaultAddress[] = "0.0.0.0";
const unsigned Config::Http::DefaultPort = 5994;

std::string Config::Http::address_ = Config::Http::DefaultAddress;
unsigned Config::Http::port_ = Config::Http::DefaultPort;

std::string Config::Process::pidFile_;
std::string Config::Process::stdOutFile_;
std::string Config::Process::stdErrFile_;
std::string Config::Process::rootDirectory_;

boost::program_options::options_description Config::desc_("Program options");
boost::program_options::variables_map Config::vm_;

void Config::Parse(int argc, const char** argv) {
    if (desc_.options().size() == 0) {
        desc_.add_options()
            ("help,h", "shows the program options")
            ("address,a", boost::program_options::value(&Http::address_)->default_value(Http::address_), "the IP address to listen on")
            ("port,p", boost::program_options::value(&Http::port_)->default_value(Http::port_), "the TCP/IP port to listen on")
            ("daemon,d", "daemonize the process")
            ("pid", boost::program_options::value(&Process::pidFile_), "writes the process id to a file")
            ("out,o", boost::program_options::value(&Process::stdOutFile_), "writes stdout to a file")
            ("err,e", boost::program_options::value(&Process::stdErrFile_), "writes stderr to a file")
            ("dir", boost::program_options::value(&Process::rootDirectory_), "sets the working directory")
        ;
    }

    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc_), vm_);
    boost::program_options::notify(vm_);
}

const boost::program_options::options_description& Config::Description() noexcept {
    return desc_;
}

bool Config::IsHelp() noexcept {
    return vm_.count("help") > 0;
}

bool Config::IsDaemon() noexcept {
    return Process::Daemonize();
}

const std::string& Config::Process::PidFile() noexcept {
    return pidFile_;
}

const std::string& Config::Process::StdOutFile() noexcept {
    return stdOutFile_;
}

const std::string& Config::Process::StdErrFile() noexcept {
    return stdErrFile_;
}

const std::string& Config::Process::RootDirectory() noexcept {
    return rootDirectory_;
}

bool Config::Process::Daemonize() noexcept {
    return vm_.count("daemon") > 0;
}

unsigned Config::Environment::CpuCount() {
    auto cores = std::max(2u, boost::thread::hardware_concurrency());
    return cores;
}

const std::string& Config::Http::Address() noexcept {
    return address_;
}

unsigned Config::Http::Port() noexcept {
    return port_;
}

std::uint32_t Config::SpiderMonkey::GetHeapSize() {
    return 64 * 1024 * 1024;
}

bool Config::SpiderMonkey::GetEnableBaselineCompiler() {
    return true;
}

bool Config::SpiderMonkey::GetEnableIonCompiler() {
    return true;
}

double Config::MapReduce::GetCPUMultiplier() {
    return 0.5;
}

unsigned Config::Data::GetDatabaseDeleteDelay() {
    return 5;
}