/*
 Parser to read in and interpret .ttb files from Railway Operation Simulator
 Copyright (C) 2020 Kristian Zarebski
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>
 
 @author K. Zarebski
 @date   last modified Mon Jun 16 2020
*/
#ifndef __TTBPARSER_HXX__
#define __TTBPARSER_HXX__

#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <map>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/range.hpp>

#include "Structure.hxx"
#include "Types.hxx"

namespace TTBParser
{
    class _parser_impl
    {
        public:
            std::map<std::string, Service> _services;
            bool _file_check(const std::string& file_name);
            boost::posix_time::ptime _get_time(const std::string& time_str);
            std::vector<std::string> _split(const std::string& str, const char delimiter=char(0));
    };

    class Parser
    {
        private:
            _parser_impl* _impl;
        public:
            Parser(char separator=char(0)) : _impl(new _parser_impl) {}
            bool ParseServices(const std::string& file_name);
    };
};

#endif