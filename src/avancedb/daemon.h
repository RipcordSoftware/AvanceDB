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

#ifndef DAEMON_H
#define	DAEMON_H

#include <string>

class Daemon {
public:
    Daemon() = delete;
    Daemon(const Daemon&) = delete;
    
    static bool Daemonize(const std::string& stdoutFile, const std::string& stderrFile);
    
    static void RedirectStdout(const std::string& file);
    static void RedirectStderr(const std::string& file);
    
    static void WritePid(const std::string& file);
    
    static void ChangeDir(const std::string& dir);
    
private:

};

#endif	/* DAEMON_H */

