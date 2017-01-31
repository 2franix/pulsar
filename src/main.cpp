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
	// Declare command line options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("server", po::value<std::string>()->default_value(""), "Server to connect to.")
		("sink,s", po::value<std::string>()->default_value(""), "Sink to connect to.")
		("duration,d", po::value<int>()->default_value(1), "Probing duration in seconds. It is the amount of time the sink's samples will be averaged.")
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

	clock_t startTime = clock();

	// Start mainloop.
	PulseAudio::Pointer<pa_threaded_mainloop> mainloop(pa_threaded_mainloop_new(), pa_threaded_mainloop_free);
	pa_mainloop_api *mainloopApi = pa_threaded_mainloop_get_api(mainloop.get());

	//Monitor monitor(mainloopApi, "/var/run/pulse/native", "alsa_output.pci-0000_00_14.2.analog-stereo");
	Monitor monitor(mainloopApi, vm["server"].as<string>(), vm["sink"].as<string>());
	pa_threaded_mainloop_start(mainloop.get());

	int duration = vm["duration"].as<int>();
	do
	{
		sleep(0.01);
	}
	while((clock() - startTime)/CLOCKS_PER_SEC < duration);
	
	cout << "Average level: " << monitor.getAverageLevel() << endl;

	return 0;
};
