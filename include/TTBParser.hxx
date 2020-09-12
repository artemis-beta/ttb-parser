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
 @date   last modified Tue Jun 17 2020
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
#include <boost/range.hpp>

#include "Structure.hxx"
#include "Types.hxx"
#include "Utilities.hxx"
#include "Timetable.hxx"

namespace TTBParser
{
    /**
     * @brief Internal parser class
     * 
     * The class contains the protected functions used
     * within the main parser class. By placing them inside
     * a separate object they can be tested in unit tests
     * 
     */
    class _parser_impl
    {
        public:
            std::map<std::string, Entry> _entries;   /*!< map of entries with headcodes/other as keys */

            /**
             * @brief Check selected timetable file is valid
             * 
             * This function determines if the file exists and whether it
             * is parsible by the parser.
             * 
             * @param file_name name of ttb file to be read
             * @return true 
             * @return false 
             */
            bool _file_check(const std::string& file_name);


            /**
             * @brief Split string based on a selected delimiter
             * 
             * @param str  input string to parse
             * @param delimiter char to split by (defaults to NULL char)
             * @return std::vector<std::string> 
             */
            std::vector<std::string> _split(const std::string& str, const char delimiter=char(0));

            /**
             * @brief 
             * 
             * @param str string to parse
             * @param delimiter char to split by
             * @return Coordinate 
             */
            Coordinate _parse_coordinate(const std::string& str, const char delimiter='-');

    };

    /**
     * @brief Main parser class for timetable files
     * 
     * This is the public accessible parser class used to read
     * in and parse ttb files
     * 
     */
    class Parser
    {
        private:
            _parser_impl* _impl;    /*!< Instance of IMPL for internal functions */
        public:
            Parser(char separator=char(0)) : _impl(new _parser_impl) {}

            /**
             * @brief Parse timetable file and return true if successful
             * 
             * @param file_name input file name as string
             * @return true 
             * @return false 
             */
            bool ParseEntries(const std::string& file_name);

            /**
             * @brief Get the entries
             * 
             * @return std::map<std::string, Entry> 
             */
            std::map<std::string, Entry> getEntries() const {return _impl->_entries;}

	    Timetable getTimetableObject(const std::string name) const;
    };
};

#endif
