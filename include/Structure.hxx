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
 @date   last modified Tue Jun 16 2020
*/
#ifndef __STRUCTURE_TTB_HXX__
#define __STRUCTURE_TTB_HXX__

#include <ctime>
#include <vector>
#include <tuple>

#include "Types.hxx"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>

namespace TTBParser
{
    struct duration_event
    {
        boost::posix_time::ptime time_start;
        boost::posix_time::ptime time_end;
        std::string label;

        friend std::ostream& operator<<(std::ostream& os, duration_event& de)
        {
            os << de.time_start << "\t" << de.time_end << "\t" << de.label;
            return os;
        }
    };

    struct single_event
    {
        boost::posix_time::ptime time;
        std::string label;

        friend std::ostream& operator<<(std::ostream& os, single_event& se)
        {
            os << se.time << "\t" << "\t" << se.label;
            return os;
        }
    };

    struct Service
    {
        ServiceType type;
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
        int signaller_speed = -1;

        friend std::ostream& operator<<(std::ostream& os, Service& srv)
        {
            os << "[" << srv.headcode << "]" << std::endl;
            os << "\t" << "Description: " << srv.description << std::endl;
            os << "\t" << "Start Time: ";
            os << srv.start_time.time_of_day() << std::endl;
            if(srv.mass != -1){os << "\t" << "Mass: " << srv.mass << " T" <<  std::endl;}
            if(srv.start_speed != -1){os << "\t" << "Start Speed: " << srv.start_speed << " km/h" << std::endl;}
            if(srv.max_speed != -1){os << "\t" << "Max Speed: " << srv.max_speed << " km/h" << std::endl;}
            if(srv.power != -1){os << "\t" << "Power: " << srv.power << " kW" << std::endl;}
            if(srv.brake_force != -1){os << "\t" << "Brake Force: " << srv.brake_force << " T" << std::endl;}

            return os;
        }
    };
};

#endif