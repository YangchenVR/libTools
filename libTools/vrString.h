#ifndef _vrString_h_
#define _vrString_h_


#include <string>
#include <boost/format.hpp>
namespace VR
{
	/*class vrString :public std::string
	{
	public:

	};*/
	typedef std::string vrString;

	/*template<class TFirst>
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
	}*/

	vrString awesome_printf_helper(boost::format& f){
		return boost::str(f);
	}

	template<class T, class... Args>
	vrString awesome_printf_helper(boost::format& f, T&& t, Args&&... args){
		return awesome_printf_helper(f % std::forward<T>(t), std::forward<Args>(args)...);
	}

	template<typename... Arguments>
	vrString string_format(std::string const& fmt, Arguments&&... args)
	{
		boost::format f(fmt);
		return awesome_printf_helper(f, std::forward<Arguments>(args)...);
		//auto result = awesome_printf_helper(f, std::forward<Arguments>(args)...);
		//std::cout << result;
	}
}//namespace VR

#endif//_vrString_h_