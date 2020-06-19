#include "Types.hxx"

namespace TTBParser
{

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
};
