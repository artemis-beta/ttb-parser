#include "Structure.hxx"

namespace TTBParser
{

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
	
	bool Timetable::sendToFile(const std::string file_name)
	{
		std::ofstream outs(file_name);

		outs << _send_to_string();

		outs.close();

		return true;
	}

	std::string Timetable::_send_to_string()
	{
		std::stringstream strs;
		for(int i{0}; i < entries.size(); ++i)
		{
			if(i > 0)
			{
					strs << char(0);
			}
			std::pair<std::string, Entry> entry = getEntry(i);
			if(entry.first.substr(0,7) == "Comment")
			{
				strs << entry.second.description;
			}
			else if(entry.first == "Start Time")
			{
				strs << ptimeToString(entry.second.start_time);
			}
			else
			{
				strs << boost::algorithm::join(entry.second.asVector(), ",");
			}
		}

		return strs.str();
	}

};
