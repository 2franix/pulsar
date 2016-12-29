#include <iostream>

#include "unistd.h"

#include "Monitor.h"

using namespace std;
using namespace Pulsar;

int main(int argumentCount, char **arguments)
{
	PulseAudio::Pointer<pa_threaded_mainloop> mainloop(pa_threaded_mainloop_new(), pa_threaded_mainloop_free);
	pa_mainloop_api *mainloopApi = pa_threaded_mainloop_get_api(mainloop.get());

	Monitor monitor(mainloopApi, "/var/run/pulse/native");
	pa_threaded_mainloop_start(mainloop.get());

	do
	{
		sleep(1);
	}
	while(true);
	
	cout << "End." << endl;

	return 0;
};
