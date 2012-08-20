#ifndef _AKUMA_SIMULATOR_H_
#define _AKUMA_SIMULATOR_H_

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

ExitReason::Enum startSimulator(
	const boost::filesystem::path& profilePath,
	const char* profile,
	const boost::filesystem::path& pathToMain
);

#endif