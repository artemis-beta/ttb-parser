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
#ifndef __TYPES_HXX__
#define __TYPES_HXX__

#include <map>
#include <string>

namespace TTBParser
{
    enum ServiceType
    {
        NewService,
        ServiceFromService,
        ServiceFromShuttle,
        ShuttleFromFeeder,
        ShuttleFromStop
    };

    std::string typeToString(const ServiceType& type);

    ServiceType typeFromString(const std::string str);
};

#endif
