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
#include <map>
#include <tuple>
#include <string>
#include <sstream>
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

        std::string toString()
        {
            return to_simple_string(time_start.time_of_day()) + "\t" + to_simple_string(time_end.time_of_day()) + "\t" + label;
        }

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

        std::string toString()
        {
            return to_simple_string(time.time_of_day()) + "\t"  + label;
        }

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

        std::string toString() const
        {
            return std::to_string(X) + ", " + std::to_string(Y);
        }

        /**
         * @brief Allows coordinate to be sent to std::cout
         * 
         * @param os output stream
         * @param coord the coordinate instance
         * @return std::ostream& 
         */
        friend std::ostream& operator<<(std::ostream& os, Coordinate& coord)
        {
            os << coord.toString();
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

        friend std::string operator+(const std::string str, Coordinate& coord)
        {
            return str+coord.toString();
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
        int size = -1;                                  /*!< How long the Entry is in terms of components */
        ServiceType type;                               /*!< Service type, e.g. Sns etc */
        std::map<int, duration_event> duration_events;    /*!< Events containing a start and end time */
        std::map<int, single_event> single_events;        /*!< Events with only a single time */
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

        std::string toString()
        {
            std::string out;
            out += ("[" + headcode + "]" + "\n");
            out += ("\tDescription: " + description + "\n");
            out += "\tStart Time: ";
            out +=  to_simple_string(start_time.time_of_day());
            out += "\n";
            if(parent){out += "\tFormed From: "; out += parent->headcode; out += "\n";}
            if(entry.first.X != -9999){out += "\tEntry: "; out = out + entry.first;}
            if(entry.second.X != -9999){out += " " + entry.second + "\n";}
            if(mass != -1){out += "\tMass: " + std::to_string(mass) + " T" +  "\n";}
            if(start_speed != -1){out += "\tStart Speed: "; out += std::to_string(start_speed); out += " km/h\n";}
            if(max_speed != -1){out += "\tMax Speed: "; out += std::to_string(max_speed); out += " km/h\n";}
            if(power != -1){out += "\tPower: "; out += std::to_string(power); out += " kW\n";}
            if(brake_force != -1){out += "\tBrake Force: "; out += std::to_string(brake_force); out += " T\n";}
            
            for(int i{0}; i < size; ++i)
            {
                if(duration_events.find(i) != duration_events.end())
                {
                    out += "\t"+duration_events[i].toString() + "\n";
                }
                else if(single_events.find(i) != single_events.end())
                {
                    out += "\t"+single_events[i].toString() + "\n";
                }
            }

            return out;
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
        std::string _send_to_string();
	    bool sendToFile(const std::string file_name);
    };
};

#endif
