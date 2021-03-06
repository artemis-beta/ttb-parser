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
            return false;
        }
        
        if(!boost::filesystem::exists(file_name))
        {
           return false;
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


    bool Parser::ParseEntries(const std::string& file_name)
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
        std::vector<std::pair<std::string, std::string>> linked_entries;

        for(auto& serv : _ttb_data)
        {
            Entry entry;

            if(std::find(serv.begin(), serv.end(), ';') == serv.end())
            {
                if(std::isdigit(serv[0]) && std::isdigit(serv[1]) && serv[2] == ':')
                {
		    entry.start_time = getTimeFromString(serv);
		    _impl->_entries["Start Time"] = entry;
                    continue;
                }

                else
                {
                    entry.description = serv;
                    entry.index = ++index;
                    _impl->_entries["Comment "+std::to_string(entry.index)] = entry;
		    continue;
                }
            }

            std::vector<std::string> _components = _impl->_split(serv, ',');
            entry.size = _components.size();

            for(int i{0}; i < _components.size(); ++i)
            {
                if(i == 0)
                {
                    std::vector<std::string> init_components = _impl->_split(_components[i], ';');

                    entry.headcode = init_components[0];
                    entry.description = init_components[1];

                    if(init_components.size() > 2)
                    {
                        entry.start_speed = static_cast<int>(std::stoi(init_components[2]));
                        entry.max_speed = static_cast<int>(std::stoi(init_components[3]));
                        entry.mass = static_cast<int>(std::stoi(init_components[4]));
                        entry.brake_force = static_cast<int>(std::stoi(init_components[5]));
                        entry.power = static_cast<int>(std::stoi(init_components[6]));
                    }

                    if(init_components.size() > 7)
                    {
                        entry.signaller_speed = static_cast<int>(std::stoi(init_components[7]));
                    }

                    continue;

                }

                if(i == 1)
                {
                    std::vector<std::string> start_components = _impl->_split(_components[i], ';');

                    entry.start_time = getTimeFromString(start_components[0]);

                    if(start_components[1] == "Snt")
                    {
                        entry.type = ServiceType::NewService;
                    }
                    else if(start_components[1] == "Sns")
                    {
                        entry.type = ServiceType::ServiceFromService;
                    }
                    else if(start_components[1] == "Sns-sh")
                    {
                        entry.type = ServiceType::ServiceFromShuttle;
                    }
                    else if(start_components[1] == "Snt-sh")
                    {
                        entry.type = ServiceType::ShuttleFromStop;
                    }

                    if(start_components.size() > 2)
                    {
                        std::vector<std::string> _coords;
                        switch(entry.type)
                        {
                            case ServiceType::NewService:
                                _coords = _impl->_split(start_components[2], ' ');
                                entry.entry = {_impl->_parse_coordinate(_coords[0]), _impl->_parse_coordinate(_coords[1])};
                                break;
                            case ServiceType::ShuttleFromStop:
                                _coords = _impl->_split(start_components[2], ' ');
                                entry.entry = {_impl->_parse_coordinate(_coords[0]), _impl->_parse_coordinate(_coords[1])};
                                break;
                            case ServiceType::ServiceFromService:
                                linked_entries.push_back({start_components[2], entry.headcode});
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
                        if(getTimeFromString(_time_components[1]).is_not_a_date_time())
                        {
                            entry.finish_time = getTimeFromString(_time_components[0]);
                            if(_time_components[1] == "Frh")
                            {
                                entry.finish_as = FinishType::FinishRemainHere;
                            }
                            else if(_time_components[1] == "Fns")
                            {
                                entry.finish_as = FinishType::FinishNewService;
                            }
                            else if(_time_components[1] == "Frh-sh")
                            {
                                entry.finish_as = FinishType::FinishRemainHereShuttle;
                            }
                        }
                        else 
                        {
                            entry.duration_events[i] = {getTimeFromString(_time_components[0]),
                                                                getTimeFromString(_time_components[1]),
                                                                _time_components[2]};
                        }
                    }

                    else if(_time_components.size() == 2)
                    {
                        
                        entry.single_events[i] = {getTimeFromString(_time_components[0]), 
                                                        _time_components[1]};
                    }

                }

                else
                {
                    std::vector<std::string> _components = _impl->_split(comp, ';');
                    if(_components[0] == "R")
                    {
                        entry.repeats.number = stoi(_components[3]);
                        entry.repeats.headcode_interval = stoi(_components[2]);
                        entry.repeats.time_interval = stoi(_components[1]);
                    }
                }

            }
            
            entry.index = ++index;
            _impl->_entries[entry.headcode] = entry;
        }

        for(auto& pair : linked_entries)
        {
            _impl->_entries[pair.second].parent = &(_impl->_entries[pair.first]);
            _impl->_entries[pair.first].daughter = &(_impl->_entries[pair.second]);
        }

        return true;
    }

    Timetable Parser::getTimetableObject(const std::string name) const
    {
        Timetable timetable;
        timetable.entries = _impl->_entries;
        timetable.start = _impl->_entries["Start Time"].start_time;
        timetable.name = name;

        return timetable;
    }
};
