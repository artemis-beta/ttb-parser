#include "Types.hxx"

namespace TTBParser
{
	std::map<std::string, ServiceType> types = {{"Snt", ServiceType::NewService},
	    								{"Sns", ServiceType::ServiceFromService},
										{"Snt-sh", ServiceType::ShuttleFromStop}};
    std::string typeToString(const ServiceType& type)
    {
	    for(auto& t : types)
	    {
		    if(t.second == type)
		    {
			    return t.first;
	            }
	    }

	    return "";
    }

	ServiceType typeFromString(const std::string str)
    {
	    return types[str];
    } 
};
