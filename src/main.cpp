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
		("operator,o", po::value<std::string>()->multitoken()->default_value("or"), "Operator to apply on the different sinks for probing to succeed.")
		("timeout,t", po::value<double>()->default_value(0.5), "Probing duration in seconds. It is the maximum amount of time before the program will return a non-zero value if no sample could be probed.")
		("verbosity,v", po::value<string>()->default_value("error"), "Verbose mode. Value must be in [info, warning, error].")
		("version", po::bool_switch()->default_value(false), "Print version of this program.")
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

	if (vm["version"].as<bool>())
	{
		cout << Application::get().getVersion() << endl;
		return 0;
	}

	const string &oper = vm["operator"].as<string>();
	if (oper != "and" && oper != "or")
	{
		Application::get().printError("Invalid operator.");
		return 2;
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

	std::function<bool(const list<shared_ptr<Monitor>>&)> predicate = [&oper](const list<shared_ptr<Monitor>>& monitors)
	{
		std::function<bool(const shared_ptr<Monitor>&)> monitorPredicate = [](const shared_ptr<Monitor> &monitor) {return monitor->hasSamples();};
		if (oper == "or")
		{
			return any_of(monitors.begin(), monitors.end(), monitorPredicate);
		}
		else
		{
			return all_of(monitors.begin(), monitors.end(), monitorPredicate); 
		}
	};

	bool sampleFound = false;
	while(getTime() - startTime < duration && !sampleFound)
	{
		sampleFound = predicate(monitors);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	for (const shared_ptr<Monitor> &monitor : monitors)
	{
		string status = monitor->getSinkName() + ": ";
		status += monitor->hasSamples() ? "is playing samples" : "is not playing samples";
		Application::get().printInfo(status);
	}

	pa_threaded_mainloop_stop(mainloop.get());
	
	return sampleFound ? 0 : 1;
};
