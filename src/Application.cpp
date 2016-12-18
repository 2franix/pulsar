#include "Application.h"

namespace Pulsar
{
	Application &Application::get()
	{
		static Application application;
		return application;
	}
}
