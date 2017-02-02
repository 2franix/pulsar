#include <iostream>
#include <ctime>

#include "unistd.h"

#include "boost/program_options.hpp" 

#include "Monitor.h"

using namespace std;
using namespace Pulsar;
namespace po = boost::program_options;

double getTime()
{
	timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return time.tv_sec + (double)time.tv_nsec/1e9;
}

int main(int argumentCount, char **arguments)
{
	// Declare command line options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("server", po::value<std::string>()->default_value(""), "Server to connect to.")
		("sink,s", po::value<std::string>()->default_value(""), "Sink to connect to.")
		("timeout,t", po::value<double>()->default_value(0.5), "Probing duration in seconds. It is the maximum amount of time before the program will return a non-zero value if no sample could be probed.")
		("help,h", "Print this message.");

	// Parse command line.
	po::variables_map vm;
	po::store(po::parse_command_line(argumentCount, arguments, desc), vm);
	po::notify(vm); 
	if (vm.count("help") > 0)
	{
		cout << desc << endl;
		return 0;
	}

	double startTime = getTime();

	// Start mainloop.
	PulseAudio::Pointer<pa_threaded_mainloop> mainloop(pa_threaded_mainloop_new(), pa_threaded_mainloop_free);
	pa_mainloop_api *mainloopApi = pa_threaded_mainloop_get_api(mainloop.get());

	//Monitor monitor(mainloopApi, "/var/run/pulse/native", "alsa_output.pci-0000_00_14.2.analog-stereo");
	Monitor monitor(mainloopApi, vm["server"].as<string>(), vm["sink"].as<string>());
	pa_threaded_mainloop_start(mainloop.get());

	double duration = vm["timeout"].as<double>();
	do
	{
		sleep(0.1);
	}
	while(getTime() - startTime < duration && !monitor.hasSamples());

	pa_threaded_mainloop_stop(mainloop.get());
	
	return monitor.hasSamples() ? 0 : 1;
};
