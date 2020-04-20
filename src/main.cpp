#include <iostream>
#include <chrono>
#include <thread>

#include "unistd.h"

#include "boost/program_options.hpp" 

#include "Application.h"
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
		("sinks,s", po::value<std::vector<std::string>>()->multitoken(), "Sinks to connect to.")
		("timeout,t", po::value<double>()->default_value(0.5), "Probing duration in seconds. It is the maximum amount of time before the program will return a non-zero value if no sample could be probed.")
		("verbosity,v", po::value<string>()->default_value("error"), "Verbose mode. Value must be in [info, warning, error].")
		("version", "Print version of this program.")
		("help,h", "Print this message.");

	// Parse command line.
	po::variables_map vm;
	po::store(po::parse_command_line(argumentCount, arguments, desc), vm);
	po::notify(vm); 
	Application::LogLevel logLevel;
	if (vm["verbosity"].as<string>() == "error")
	{
		logLevel = Application::LogLevel::Error;
	}
	else if (vm["verbosity"].as<string>() == "warning")
	{
		logLevel = Application::LogLevel::Warning;
	}
	else if (vm["verbosity"].as<string>() == "info")
	{
		logLevel = Application::LogLevel::Info;
	}
	else
	{
		throw "Invalid verbosity level.";
	}
	Application::get().setLogLevel(logLevel);

	if (vm.count("version") > 0)
	{
		cout << Application::get().getVersion() << endl;
	}

	if (vm.count("help") > 0)
	{
		cout << desc << endl;
		return 0;
	}

	// Start mainloop.
	PulseAudio::Pointer<pa_threaded_mainloop> mainloop(pa_threaded_mainloop_new(), pa_threaded_mainloop_free);
	pa_mainloop_api *mainloopApi = pa_threaded_mainloop_get_api(mainloop.get());

	std::vector<string> sinkNames;
	if (vm.count("sinks"))
	{
		sinkNames = vm["sinks"].as<vector<string>>();
	}
	else
	{
		sinkNames.push_back("");
	}

	list<shared_ptr<Monitor>> monitors;
	for(const string &sinkName : sinkNames)
	{
		monitors.push_back(make_shared<Monitor>(mainloopApi, vm["server"].as<string>(), sinkName));
	}
	pa_threaded_mainloop_start(mainloop.get());

	double duration = vm["timeout"].as<double>();
	double startTime = getTime();

	while(getTime() - startTime < duration &&
		std::none_of(monitors.begin(), monitors.end(), [](const shared_ptr<Monitor> &monitor) {return monitor->hasSamples();}))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	for (const shared_ptr<Monitor> &monitor : monitors)
	{
		string status = monitor->getSinkName() + ": ";
		status += monitor->hasSamples() ? "is playing samples" : "is not playing samples";
		Application::get().printInfo(status);
	}

	pa_threaded_mainloop_stop(mainloop.get());
	
	return std::any_of(monitors.begin(), monitors.end(), [](const shared_ptr<Monitor> &monitor) {return monitor->hasSamples();}) ? 0 : 1;
};
