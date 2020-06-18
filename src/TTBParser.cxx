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
 @date   last modified Wed Jun 17 2020
*/

#include "TTBParser.hxx"

namespace TTBParser
{
    bool _parser_impl::_file_check(const std::string& file_name)
    {
        if(file_name.empty())
        {
            std::cerr << "File name is empty string" << std::endl;
            return false;
        }

        if(!boost::filesystem::exists(file_name))
        {
            throw std::runtime_error("File '"+file_name+"' not found");
        }

        return  !boost::filesystem::is_empty(file_name);
    }

    Coordinate _parser_impl::_parse_coordinate(const std::string& coordinate, const char delimiter)
    {
        std::vector<std::string> _components = _split(coordinate, delimiter);
        if(_components[0][0] == 'N')
        {
            _components[0][0] = '-';
        }

        if(_components[1][0] == 'N')
        {
            _components[1][0] = '-';
        }

        return  {std::stoi(_components[0]), std::stoi(_components[1])};
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

        _out.push_back(_temp);

        return _out;

    }

    boost::posix_time::ptime TTBParser::_parser_impl::_get_time(const std::string& time_str)
    {
        const std::locale loc = std::locale(std::locale::classic(),
                                             new boost::posix_time::time_input_facet("%H:%M"));
        std::istringstream is(time_str);
        is.imbue(loc);
        boost::posix_time::ptime t;
        is >> t;

        return t;
    }

    bool Parser::ParseServices(const std::string& file_name)
    {
        int index = 0;

        if(!_impl->_file_check(file_name)) return false;

        boost::filesystem::ifstream  file(file_name);

        std::vector<std::string> _file_data;
        
        std::string _data_str;

        bool is_intro_str = true;

        while(getline(file, _data_str))
        {
            _file_data.push_back(_data_str);
        }

        std::vector<std::string> _ttb_data = _impl->_split(_file_data[0]);
        std::vector<std::pair<std::string, std::string>> linked_services;

        for(auto& serv : _ttb_data)
        {
            Service service;

            if(std::find(serv.begin(), serv.end(), ';') == serv.end())
            {
                if(std::isdigit(serv[0]) && std::isdigit(serv[1]) && serv[2] == ':')
                {
                    _impl->_start_time = _impl->_get_time(serv);
                    continue;
                }

                else
                {
                    service.description = serv;
                    service.index = ++index;
                    _impl->_services["Comment "+std::to_string(service.index)] = service;
		    continue;
                }
            }

            std::vector<std::string> _components = _impl->_split(serv, ',');
            
            


            for(int i{0}; i < _components.size(); ++i)
            {
		std::cout << _components[i] << std::endl;
                if(i == 0)
                {
                    std::vector<std::string> init_components = _impl->_split(_components[i], ';');

                    service.headcode = init_components[0];
                    service.description = init_components[1];

                    if(init_components.size() > 2)
                    {
                        service.start_speed = static_cast<int>(std::stoi(init_components[2]));
                        service.max_speed = static_cast<int>(std::stoi(init_components[3]));
                        service.mass = static_cast<int>(std::stoi(init_components[4]));
                        service.brake_force = static_cast<int>(std::stoi(init_components[5]));
                        service.power = static_cast<int>(std::stoi(init_components[6]));
                    }

                    if(init_components.size() > 7)
                    {
                        service.signaller_speed = static_cast<int>(std::stoi(init_components[7]));
                    }

                    continue;

                }

                if(i == 1)
                {
                    std::vector<std::string> start_components = _impl->_split(_components[i], ';');

                    service.start_time = _impl->_get_time(start_components[0]);

                    if(start_components[1] == "Snt")
                    {
                        service.type = ServiceType::NewService;
                    }
                    else if(start_components[1] == "Sns")
                    {
                        service.type = ServiceType::ServiceFromService;
                    }
                    else if(start_components[1] == "Sns-sh")
                    {
                        service.type = ServiceType::ServiceFromShuttle;
                    }
                    else if(start_components[1] == "Snt-sh")
                    {
                        service.type = ServiceType::ShuttleFromStop;
                    }

                    if(start_components.size() > 2)
                    {
                        std::vector<std::string> _coords;
                        switch(service.type)
                        {
                            case ServiceType::NewService:
                                _coords = _impl->_split(start_components[2], ' ');
                                service.entry = {_impl->_parse_coordinate(_coords[0]), _impl->_parse_coordinate(_coords[1])};
                                break;
                            case ServiceType::ShuttleFromStop:
                                service.entry = {_impl->_parse_coordinate(start_components[2]), {-9999, -9999}};
                                break;
                            case ServiceType::ServiceFromService:
                                linked_services.push_back({start_components[2], service.headcode});
                                break;
                            default:
                                break;
                        }
                        
                    }

                }

                const std::string comp = _components[i];
                if(std::find(comp.begin(), comp.end(), ':') != comp.end())
                {
                    std::vector<std::string> _time_components = _impl->_split(comp, ';');
                
                    if(_time_components.size() == 3)
                    {
                        service.duration_events.push_back({_impl->_get_time(_time_components[0]),
                                                                _impl->_get_time(_time_components[1]),
                                                                _time_components[2]});
                    }

                    else if(_time_components.size() == 2)
                    {
                        
                        service.single_events.push_back({_impl->_get_time(_time_components[0]), 
                                                        _time_components[1]});
                    }

                }

            }
            
            service.index = ++index;
            _impl->_services[service.headcode] = service;

        }

        for(auto& pair : linked_services)
        {
            _impl->_services[pair.second].parent = &(_impl->_services[pair.first]);
        }


        return true;
    }
};
