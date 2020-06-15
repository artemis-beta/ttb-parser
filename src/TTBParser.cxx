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
 @date   last modified Sun Jun 07 2020
*/

#include "TTBParser.hxx"

namespace TTBParser
{
    bool _parser_impl::_file_check(const std::string& file_name)
    {
        if(!boost::filesystem::exists(file_name))
        {
            throw std::runtime_error("File '"+file_name+"' not found");
        }

        return  !boost::filesystem::is_empty(file_name);
    }

    std::vector<std::string> _parser_impl::_split(const std::string& in_string, const char delimiter)
    {
        std::vector<std::string> _out;
        std::string _temp;

        for(const char& c : in_string)
        {
            if(c == delimiter || (delimiter == char(0) && static_cast<int>(c) == 0))
            {
                _out.push_back(_temp);
                _temp = "";
                continue;
            }

            _temp += c;
        }

        return _out;

    }

    bool Parser::ParseServices(const std::string& file_name)
    {
        try
        {
            if(!_impl->_file_check(file_name))
            {
                throw std::runtime_error("Could not find file '"+file_name+"'");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }

        boost::filesystem::ifstream  file(file_name);

        std::vector<std::string> _file_data;
        
        std::string _data_str;

        std::cout << char(0) << std::endl;

        while(getline(file, _data_str))
        {
            _file_data.push_back(_data_str);
        }

        std::vector<std::string> _ttb_data = _impl->_split(_file_data[0]);

        std::map<std::string, Service> services;

        for(auto& serv : _ttb_data)
        {
            Service service;
            std::vector<std::string> _components = _impl->_split(serv, ',');
            for(auto& comp : _components)
            {
                if(std::find(comp.begin(), comp.end(), ':') != comp.end())
                {
                    std::vector<std::string> _time_components = _impl->_split(comp, ';');
                    
                    if(_time_components.size() == 2)
                    {
                        const std::locale loc = std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%H:%M"));
                        std::istringstream is(_time_components[0]);
                        is.imbue(loc);
                        boost::posix_time::ptime t;
                        is >> t;
                        service.single_events.push_back({t, _time_components[1]});
                    }
                }
            }
        }
    }
};