#include "vrConfigureParser.h"
#include "vrLog.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
namespace VR
{
	namespace ConfigureParser
	{
		void parser_configurefile(const vrString& strConf, std::map< vrString, vrString >& propertyMap)
		{
			boost::property_tree::ptree pt;
			boost::property_tree::ini_parser::read_ini(strConf, pt);
			for (auto& itr : propertyMap)
			{
				itr.second = pt.get<vrString>(itr.first);
				//infoLog << (itr.first) << " " << (itr.second);
			}
		}
	}//namespace ConfigureParser
}//namespace VR