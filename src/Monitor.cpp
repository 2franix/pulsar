#include "Monitor.h"
#include "Application.h"

#include <memory>
#include <functional>
#include <iostream>

#include <pulse/thread-mainloop.h>
#include <pulse/context.h>
#include <pulse/introspect.h>
#include <pulse/operation.h>

using namespace std;

namespace Pulsar
{
	Monitor::Monitor(pa_mainloop_api *api, const string &serverName)
		: serverName(serverName), mainloopApi(api), context(nullptr)
	{
		this->context = pa_context_new(this->mainloopApi, Application::get().getName().c_str());
		//pa_context_set_state_callback(this->context, &Monitor::contextStateCallback, this);
		pa_context_set_state_callback(this->context, [](pa_context *context, void *monitor) {
				static_cast<Monitor*>(monitor)->onContextStateChanged(context);
				}, this);
		//pa_context_connect(this->context, this->serverName.c_str(), PA_CONTEXT_NOAUTOSPAWN, nullptr);
		pa_context_connect(this->context, nullptr, (pa_context_flags_t)0, nullptr);
	}

	void Monitor::onContextStateChanged(pa_context *context)
	{
		cout << "onContextStateChanged" << endl;
		pa_context_state_t state = pa_context_get_state(context);
		switch (state)
		{
			case PA_CONTEXT_READY:
				{
					cout << "Pulseaudio connection ready..." << endl;
					// Connected to Pulseaudio. Now request that sink_info_cb
					// be called with information about the available sinks.
					pa_operation *o = pa_context_get_sink_info_by_name(context, "", [](pa_context *context, const pa_sink_info *info, int eol, void *monitor) {
							static_cast<Monitor*>(monitor)->onSinkInfo(info);
							}, this);
					pa_operation_unref(o);
				}
				break;

			case PA_CONTEXT_FAILED:
				cout << "Connection failed" << endl;
				break;

			case PA_CONTEXT_UNCONNECTED:
				cout << "Context unconnected" << endl;
				break;

			case PA_CONTEXT_CONNECTING:
				cout << "Context connecting" << endl;
				break;

			case PA_CONTEXT_AUTHORIZING:
				cout << "Context authorizing" << endl;
				break;

			case PA_CONTEXT_TERMINATED:
				cout << "Connection terminated" << endl;
				break;

			case PA_CONTEXT_SETTING_NAME:
				cout << "Connection setting name" << endl;
				break;

			default:
				cout << "Unhandled state for context." << endl;
				break;
		}
	}

	void Monitor::onSinkInfo(const pa_sink_info *info)
	{
		cout << "onSinkInfo" << endl;
	}
}
