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
 @date   last modified 2020-08-22
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
            return ptimeToString(time_start) + "\t" + ptimeToString(time_end) + "\t" + label;
        }

        std::string toTTB()
        {
            return boost::algorithm::join(std::vector<std::string>({ptimeToString(time_start), ptimeToString(time_end), label}), ";");
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
            return ptimeToString(time) + "\t"  + label;
        }

        std::string toTTB()
        {
            return boost::algorithm::join(std::vector<std::string>({ptimeToString(time), label}), ";");
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

        std::string toTTB() const
        {
            std::string X_str = (X < 0) ? "N" : "";
            X_str += std::to_string(abs(X));
            std::string Y_str = (Y < 0) ? "N" : "";
            Y_str += std::to_string(abs(Y));
            return X_str+"-"+Y_str;
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

    struct Repeats
    {
        int number = 0;
        int time_interval = 0;
        int headcode_interval = 0;

        std::string toString()
        {
            return "Repeat "+std::to_string(number)+" times with headcode change XX"+\
            ((headcode_interval < 10) ? "0" : "") + std::to_string(headcode_interval)+" every "+std::to_string(time_interval)+" mins";
        }

        std::string toTTB()
        {
            return boost::algorithm::join(std::vector<std::string>({"R", std::to_string(time_interval),
                                                                    std::to_string(headcode_interval),
                                                                    std::to_string(number)}), ";");
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
        int size = 0;                                   /*!< How long the Entry is in terms of components */
        ServiceType type = NewService;                  /*!< Service type, e.g. Sns etc */
        FinishType finish_as = FinishRemainHere;        /*!< Finish service event type */
        std::map<int, duration_event> duration_events;  /*!< Events containing a start and end time */
        std::map<int, single_event> single_events;      /*!< Events with only a single time */
        Repeats repeats;                                /*!< Service repeat information */
        boost::posix_time::ptime start_time;            /*!< Entry start time */
        boost::posix_time::ptime finish_time;           /*!< Entry end time */
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
        Entry* daughter = nullptr;        /*!< Pointer to daughter entry */

        bool operator== (Entry& other)
        {
            return max_speed == other.max_speed && \
            start_speed == other.start_speed && \
            signaller_speed == other.signaller_speed && \
            start_time == other.start_time && \
            description == other.description && \
            headcode == other.headcode && \
            type == other.type && \
            brake_force == other.brake_force && \
            entry == other.entry && \
            exit == other.exit;
        }

        bool operator== (const Entry& other) const
        {
            Entry _temp_1 = Entry(*this);
            Entry _temp_2 = Entry(other);
            return _temp_1 == _temp_2;
        }

        bool operator!= (Entry& other)
        {
            return !(*this == other);
        }

        bool operator!= (const Entry& other) const
        {
            return !(*this == other);
        }

        std::string toString();

        void addDurationEvent(boost::posix_time::ptime start, boost::posix_time::ptime end, std::string label, int index=-1)
        {
            if(duration_events.find(index) != duration_events.end() || single_events.find(index) != single_events.end())
            {
                throw std::invalid_argument("Index already exists!");
            }
            if(index == -1) index = (duration_events.size() > single_events.size()) ? duration_events.size() : single_events.size();
            duration_events[index] = {start, end, label};
            size += 1;
        }

        void addSingleEvent(boost::posix_time::ptime time, std::string label, int index=-1)
        {
            if(duration_events.find(index) != duration_events.end() || single_events.find(index) != single_events.end())
            {
                throw std::invalid_argument("Index already exists!");
            }
            if(index == -1) index = (duration_events.size() > single_events.size()) ? duration_events.size() : single_events.size();
            single_events[index] = {time, label};
            size += 1;
        }

        std::string toString() const
        {
            return toString();
        }

        std::vector<std::string> asVector();
        void TemperalOffset(int nmins);
    };
};

#endif
