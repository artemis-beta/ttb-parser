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
 @date   last modified Mon Jun 15 2020
*/
#ifndef __STRUCTURE_TTB_HXX__
#define __STRUCTURE_TTB_HXX__

#include <ctime>
#include <vector>
#include <tuple>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>

namespace TTBParser
{
    struct duration_event
    {
        boost::posix_time::ptime time_start;
        boost::posix_time::ptime time_end;
        std::string label;
    };

    struct single_event
    {
        boost::posix_time::ptime time;
        std::string label;
    };

    struct Service
    {
        std::vector<duration_event> duration_events;
        std::vector<single_event> single_events;
        boost::posix_time::ptime start_time;
        std::string headcode;
        std::string description;
        int mass = -1;
        int power = -1;
        int brake_force = -1;
        int max_speed = -1;
        int start_speed = -1;
    };
};

#endif