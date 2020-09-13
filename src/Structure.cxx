#include "Structure.hxx"

namespace TTBParser
{

    std::string Entry::toString()
    {
        std::string out;
        out += ("[" + headcode + "]" + "\n");
        out += ("\tDescription: " + description + "\n");
        out += "\tStart Time: ";
        out +=  ptimeToString(start_time);
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
        out += "\tFinish Time: ";
        out += ptimeToString(finish_time);
        if(repeats.number > 0)
        {
            out += "\t"+repeats.toString() + "\n";
        }

        return out;
    }

    std::vector<std::string> Entry::asVector()
    {
        std::vector<std::string> out;
        std::vector<std::string> intro_line;
        if(mass > 0)
        {
            intro_line = {
                headcode,
                description,
                std::to_string(start_speed),
                std::to_string(max_speed),
                std::to_string(mass),
                std::to_string(brake_force),
                std::to_string(power)
            };

            if(signaller_speed > 0)
            {
                intro_line.push_back(std::to_string(signaller_speed));
            }
        }
        else
        {
            intro_line = {
                headcode,
                description
            };
        }
        out.push_back(boost::algorithm::join(intro_line, ";"));

        std::vector<std::string> define = {ptimeToString(start_time),
                           typeToString(type)};

        std::string coord_1;
        std::string coord_2;

        switch(type)
        {
            case ServiceType::NewService:
                coord_1 = entry.first.toTTB();
                coord_2 = entry.second.toTTB();
                define.push_back(boost::algorithm::join(std::vector<std::string>({coord_1, coord_2}), " "));
                break;
            case ServiceType::ServiceFromService:
                define.push_back(parent->headcode);
                break;
            case ServiceType::ShuttleFromStop:
                coord_1 = entry.first.toTTB();
                coord_2 = entry.second.toTTB();
                define.push_back(boost::algorithm::join(std::vector<std::string>({coord_1, coord_2}), " "));
                define.push_back(daughter->headcode);
                break;
            default:
                break;
        }
        out.push_back(boost::algorithm::join(define, ";"));

        for(int i{0}; i < size; ++i)
        {
            if(duration_events.find(i) != duration_events.end())
            {
                out.push_back(duration_events[i].toTTB());
            }
            else if(single_events.find(i) != single_events.end())
            {
                out.push_back(single_events[i].toTTB());
            }
        }

        switch(finish_as)
        {
            case FinishType::FinishRemainHere:
                out.push_back(boost::algorithm::join(std::vector<std::string>({ptimeToString(finish_time), "Frh"}), ";"));
                break;
            case FinishType::FinishRemainHereShuttle:
                out.push_back(boost::algorithm::join(std::vector<std::string>({ptimeToString(finish_time), "Frh-sh", parent->headcode}), ";"));
                break;
            case FinishType::FinishNewService:
                out.push_back(boost::algorithm::join(std::vector<std::string>({ptimeToString(finish_time), "Fns", daughter->headcode}), ";"));
            default:
                break;
        }

        if(repeats.number > 0)
        {
            out.push_back(repeats.toTTB());
        }

        return out;
    }

    void Entry::TemperalOffset(int nmins)
    {
        start_time  += boost::posix_time::minutes(nmins);
        finish_time += boost::posix_time::minutes(nmins);

        for(auto& s : single_events)
        {
            s.second.time += boost::posix_time::minutes(nmins);
        }

        for(auto& d : duration_events)
        {
            d.second.time_start += boost::posix_time::minutes(nmins);
            d.second.time_end   += boost::posix_time::minutes(nmins);
        }
    }

    void Entry::_add_duration_event(boost::posix_time::ptime start, boost::posix_time::ptime end, std::string label, int index)
    {
        index = _shimmy_events(index);
        duration_events[index] = {start, end, label};
        size += 1;
    }

    void Entry::_add_single_event(boost::posix_time::ptime time, std::string label, int index)
    {
        index = _shimmy_events(index);
        single_events[index] = {time, label};
        size += 1;
    }

    int Entry::_shimmy_events(int index)
    {
        if(index == -1)
        {
            return size;
        }
        else
        {
            if(duration_events.find(index) == duration_events.end() && single_events.find(index) == single_events.end())
            {
                throw std::invalid_argument("Index '"+std::to_string(index)+"' is outside scope of event lists");
            }
            for(int i{0}; i < index; ++i)
            {
                if(duration_events.find(size-i) != duration_events.end())
                {
                    duration_events[size-i+1] = duration_events[size-i];
                }
                else if(single_events.find(size-i) != single_events.end())
                {
                    single_events[size-i+1] = single_events[size-i];
                }
            }
        }

        return index;
    }

};
