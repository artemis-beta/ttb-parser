#include "Structure.hxx"

namespace TTBParser
{

	std::vector<std::string> Entry::asVector() const
	{
		std::vector<std::string> out;
		if(mass > 0)
		{
			std::vector<std::string> intro_line = {
				headcode,
				description,
				std::to_string(start_speed),
				std::to_string(max_speed),
				std::to_string(mass),
				std::to_string(brake_force),
				std::to_string(power)};
			if(signaller_speed > 0)
			{
				intro_line.push_back(std::to_string(signaller_speed));
			}

			out.push_back(boost::algorithm::join(intro_line, ";"));
		}

		std::vector<std::string> define = {ptimeToString(start_time),
						   typeToString(type)};

		std::stringstream strs;
		std::string coord_1;
		std::string coord_2;

		switch(type)
		if(type == ServiceType::NewService)
		{
			case ServiceType::NewService:
				strs << (entry.first).toString();
				coord_1 = strs.str();
				strs.str("");
				strs << (entry.second).toString();
				coord_2 = strs.str();
				define.push_back(boost::algorithm::join(std::vector<std::string>({coord_1, coord_2}), " "));
				break;
			case ServiceType::ServiceFromService:
				define.push_back(parent->headcode);
				break;
			default:
				break;
		}

		out.push_back(boost::algorithm::join(define, ";"));

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
