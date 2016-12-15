#include <iostream>

#include <pulse/thread-mainloop.h>

using namespace std;

int main(int argumentCount, char **arguments)
{
	for (int i = 0; i < argumentCount; i++)
	{
		cout << arguments[i] << endl;
	}
	pa_threaded_mainloop_new();
	return 0;
};
