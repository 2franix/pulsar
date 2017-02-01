#include "Application.h"

#include <iostream>

using namespace std;

namespace Pulsar
{
	Application::Application()
		: name("Pulsar"), logLevel(LogLevel::Info)
	{
	}

	Application &Application::get()
	{
		static Application application;
		return application;
	}

	void Application::printMessage(LogLevel level, const std::string &message)
	{
		if (level < this->logLevel) return;
		cout << message << endl;
	}
}
