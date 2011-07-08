#ifndef _AKUMA_SIMULATOR_H_
#define _AKUMA_SIMULATOR_H_

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>

namespace ExitReason
{
	enum Enum
	{
		UserAction,
		Error,
		Restart,

		Count
	};
}

bool initSimulator(const boost::filesystem::path& profilePath, const char* profile);
ExitReason::Enum startSimulator(const boost::filesystem::path& pathToMain);

#endif