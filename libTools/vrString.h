#ifndef _vrString_h_
#define _vrString_h_


#include <string>
#include <boost/format.hpp>
namespace VR
{
	class vrString :public std::string
	{
	public:

	};
	

	template<class TFirst>
	void string_format(boost::format& fmt, TFirst&& first)
	{
		fmt % first;
	}

	template<class TFirst, class... TOther>
	void string_format(boost::format& fmt, TFirst&& first, TOther&&... other)
	{
		fmt % first;
		string_format(fmt, other...);
	}

	template<class TFirst, class... TOther>
	vrString string_format(const char* format, TFirst&& first, TOther&&... other)
	{
		boost::format fmt(format);
		string_format(fmt, first, other...);
		return fmt.str();
	}
}//namespace VR

#endif//_vrString_h_