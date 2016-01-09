#ifndef _vrConfigureParser_h_
#define _vrConfigureParser_h_
#include "vrTypes.h"

namespace VR
{
	namespace ConfigureParser
	{
		extern void parser_configurefile(const vrString& strConf, std::map< vrString, vrString >& propertyMap);
		
	}//namespace ConfigureParser
}//namespace VR

#endif//_vrConfigureParser_h_