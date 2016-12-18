#include "Monitor.h"
#include "Application.h"

#include <memory>
#include <functional>
#include <iostream>

#include <pulse/thread-mainloop.h>
#include <pulse/context.h>

using namespace std;

namespace Pulsar
{
	Monitor::Monitor()
		: mainloop(pa_threaded_mainloop_new(), pa_threaded_mainloop_free), mainloopApi(nullptr), context(nullptr)
	{
		this->mainloopApi = pa_threaded_mainloop_get_api(this->mainloop.get());
		this->context = pa_context_new(this->mainloopApi, Application::get().getName().c_str());
		PulseAudio::ContextNotifyCallback callback = [this](pa_context *context, void *dummy) {this->onContextStateChanged(context);};
		// FIXME: Very unlikely to work, as callback.target returns a pointer to
		// a temporary lambda that is not wrapped in a std::function on PA's
		// end. Means that the closure is no more valid ("this" will no more be
		// valid, for instance).
		pa_context_set_state_callback(this->context, callback.target<void (pa_context*,void*)>(), NULL);
	}

	void Monitor::onContextStateChanged(pa_context *context)
	{
		cout << "onContextStateChanged" << endl;
	}
}
