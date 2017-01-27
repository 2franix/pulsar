#include <iostream>
#include <ctime>

#include "unistd.h"

#include "boost/program_options.hpp" 

#include "Monitor.h"

using namespace std;
using namespace Pulsar;
namespace po = boost::program_options;

int main(int argumentCount, char **arguments)
{
	po::options_description desc("Allowed options");
	boost::shared_ptr<po::option_description> helpOption(new po::option_description("help", po::value<int>(), "essai"));
	desc.add(helpOption);
	clock_t startTime = clock();

	PulseAudio::Pointer<pa_threaded_mainloop> mainloop(pa_threaded_mainloop_new(), pa_threaded_mainloop_free);
	pa_mainloop_api *mainloopApi = pa_threaded_mainloop_get_api(mainloop.get());

	Monitor monitor(mainloopApi, "/var/run/pulse/native", "alsa_output.pci-0000_00_14.2.analog-stereo");
	pa_threaded_mainloop_start(mainloop.get());

	int duration = 1;
	do
	{
		sleep(0.01);
	}
	while((clock() - startTime)/CLOCKS_PER_SEC < duration);
	
	cout << "Average level: " << monitor.getAverageLevel() << endl;

	return 0;
};
