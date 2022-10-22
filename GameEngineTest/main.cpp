#include <cstdio>

#include "Application.h"

void window_key_callback(const int key, int scan_code, const int action, int mods)
{
	if (action == 1)
	{
		printf("%c pressed\n", key);
	}
}

int main(int argc, char* argv[])
{
	Application application;

	if(!application.initialize())
	{
		return -1;
	}

	const int result = application.run();

	application.shutdown();

	return result;
}
