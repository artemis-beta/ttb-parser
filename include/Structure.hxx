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
#ifndef __STRUCTURE_TTB_HXX__
#define __STRUCTURE_TTB_HXX__

#include <ctime>
#include <vector>
#include <tuple>
#include <fstream>

#include "Types.hxx"
#include "Utilities.hxx"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/date_time.hpp>

namespace TTBParser
{
    /**
     * @brief Structure for events with start and end time
     * 
     * This structure covers events that last a duration such
     * as arrival/departure from a station
     * 
     */
    struct duration_event
    {
        boost::posix_time::ptime time_start;    /*!< Start time */
        boost::posix_time::ptime time_end;      /*!< End time */
        std::string label;                      /*!< Event label/location */

        /**
         * @brief Allows duration_event to be sent to std::cout
         * 
         * @param os output stream
         * @param de duration_event instance
         * @return std::ostream& 
         */
        friend std::ostream& operator<<(std::ostream& os, duration_event& de)
        {
            os << de.time_start << "\t" << de.time_end << "\t" << de.label;
            return os;
        }
    };

    /**
     * @brief Structure for single time events
     * 
     * This is a structure for events containing only a single time
     */
    struct single_event
    {
        boost::posix_time::ptime time;  /*!< Event time */
        std::string label;              /*!< Event label/location */

        /**
         * @brief Allows single_event to be sent to std::cout
         * 
         * @param os output stream
         * @param se the single_event object
         * @return std::ostream& 
         */
        friend std::ostream& operator<<(std::ostream& os, single_event& se)
        {
            os << se.time << "\t" << "\t" << se.label;
            return os;
        }
    };

    /**
     * @brief Structure describing the coordinates
     * 
     */
    struct Coordinate
    {
        int X;  /*!< X Coordinate on map */
        int Y;  /*!< Y Coordinate on map */

        /**
         * @brief Allows coordinate to be sent to std::cout
         * 
         * @param os output stream
         * @param coord the coordinate instance
         * @return std::ostream& 
         */
        friend std::ostream& operator<<(std::ostream& os, Coordinate& coord)
        {
            os << coord.X << "-" << coord.Y;
            return os;
        }

        /**
         * @brief compares one coordinate with another
         * 
         * @param other coordinate to compare with
         * @return true 
         * @return false 
         */
        bool operator==(const Coordinate other) const
        {
            return X == other.X && Y == other.Y;
        }

    };

    /**
     * @brief Structure describing a single entry
     * 
     * This struct contains the information for a single entry, when
     * only 'description' is initialised it also acts a placeholder
     * for any comments within the timetable
     */
    struct Entry
    {
        int index = 0;                                  /*!< Identifier to maintain ordering in file */
        ServiceType type;                               /*!< Service type, e.g. Sns etc */
        std::vector<duration_event> duration_events;    /*!< Events containing a start and end time */
        std::vector<single_event> single_events;        /*!< Events with only a single time */
        boost::posix_time::ptime start_time;            /*!< Entry start time */
        std::string headcode;                           /*!< Entry headcode */
        std::string description;                        /*!< Description of entry */
        int mass = -1;                                  /*!< Mass in T */
        int power = -1;                                 /*!< Power in kW */
        int brake_force = -1;                           /*!< Brake force in T */
        int max_speed = -1;                             /*!< Max speed in km/h */
        int start_speed = -1;                           /*!< Start speed in km/h */
        int signaller_speed = -1;                       /*!< Start speed from signaller control */
        std::pair<Coordinate, Coordinate> entry = {{-9999, -9999}, {-9999, -9999}}; /*!< Entry point coordinates */
        Coordinate exit = {-9999, -9999};   /*!< Exit coordinate */
        Entry* parent = nullptr;          /*!< Pointer to parent entry */

        friend std::ostream& operator<<(std::ostream& os, Entry& srv)
        {
            os << "[" << srv.headcode << "]" << std::endl;
            os << "\t" << "Description: " << srv.description << std::endl;
            os << "\t" << "Start Time: ";
            os << srv.start_time.time_of_day() << std::endl;
            if(srv.parent){os << "\t" << "Formed From: " << srv.parent->headcode << std::endl;}
            if(srv.entry.first.X != -9999){os << "\t" << "Entry: " << srv.entry.first;}
            if(srv.entry.second.X != -9999){os << " " << srv.entry.second << std::endl;}
            if(srv.mass != -1){os << "\t" << "Mass: " << srv.mass << " T" <<  std::endl;}
            if(srv.start_speed != -1){os << "\t" << "Start Speed: " << srv.start_speed << " km/h" << std::endl;}
            if(srv.max_speed != -1){os << "\t" << "Max Speed: " << srv.max_speed << " km/h" << std::endl;}
            if(srv.power != -1){os << "\t" << "Power: " << srv.power << " kW" << std::endl;}
            if(srv.brake_force != -1){os << "\t" << "Brake Force: " << srv.brake_force << " T" << std::endl;}

            return os;
        }

	std::vector<std::string> asVector() const;
    };

    struct Timetable
    {
	    boost::posix_time::ptime start;
	    std::map<std::string, Entry> entries;
	    std::string route_name;
	    std::string name;

	    std::pair<std::string, Entry> getEntry(const int i) const
	    {
		    for(auto& entry : entries)
		    {
			    if(entry.second.index == i)
			    {
				return {entry.first, entry.second};
			    }
	            }

		    return {"",{}};
	    }
	    bool sendToFile(const std::string file_name);
    };
};

#endif
