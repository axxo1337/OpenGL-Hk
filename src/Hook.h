#ifndef HOOK_H_
#define HOOK_H_

#include <cstdio>

#include <Windows.h>

namespace Hook
{
	//
	// Management functions
	//

	bool init();
	void shutdown();

	//
	// Get functions
	//

	bool getIsInit();
}

#endif