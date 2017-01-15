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
	Monitor::Monitor(pa_mainloop_api *api, const string &serverName, const string &sinkName)
		: serverName(serverName), sinkName(sinkName), mainloopApi(api), context(nullptr), stream(nullptr, pa_stream_unref)
	{
		this->context = pa_context_new(this->mainloopApi, Application::get().getName().c_str());
		pa_context_set_state_callback(this->context, &Monitor::onContextStateChangedCallback, this);
		pa_context_connect(this->context, this->serverName.c_str(), PA_CONTEXT_NOAUTOSPAWN, nullptr);
		//pa_context_connect(this->context, nullptr, (pa_context_flags_t)0, nullptr);
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
					pa_operation *o = pa_context_get_sink_info_by_name(context, this->sinkName.c_str(), &Monitor::onSinkInfoCallback, this);
					if (o != nullptr)
					{
						pa_operation_unref(o);
					}
					else
					{
						cout << "Failed to get sink info." << endl;
					}
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

	void Monitor::onContextStateChangedCallback(pa_context *context, void *monitor)
	{
		static_cast<Monitor*>(monitor)->onContextStateChanged(context);
	}

	void Monitor::onStreamReadCallback(pa_stream *stream, size_t byteCount, void *monitor)
	{
		static_cast<Monitor*>(monitor)->onStreamRead(byteCount);
	}

	void Monitor::onStreamRead(size_t byteCount)
	{
		if (byteCount == 0)
		{
			cout << "Monitor::onStreamRead, no data." << endl;
		}
		else
		{
			cout << "Monitor::onStreamRead, " << byteCount << " bytes." << endl;
			const void *data = nullptr;
			size_t readByteCount;
			pa_stream_peek(this->stream.get(), &data, &readByteCount);
			if (data != nullptr)
			{
				int sum = 0;
				const int16_t *dataChunks = reinterpret_cast<const int16_t*>(data);
				for (size_t byteIx = 0; byteIx < readByteCount; byteIx+=2)
				{
					int16_t chunk = dataChunks[byteIx];				
					sum += abs(chunk);
				}
				sum /= readByteCount / 2; 
				cout << "Level = " << sum / (double)INT16_MAX * 100.0 << endl;
			}
			else
			{
				cout << "No data." << endl;
			}
				
			pa_stream_drop(this->stream.get());
		}
	}

	void Monitor::onSinkInfoCallback(pa_context *context, const pa_sink_info *sinkInfo, int eol, void *monitor)
	{
		static_cast<Monitor*>(monitor)->onSinkInfo(sinkInfo);
	}

	void Monitor::onSinkInfo(const pa_sink_info *info)
	{
		cout << "onSinkInfo" << endl;
		if (info == nullptr) return;
        cout << "index:" << info->index << endl;
		cout << "name" << info->name << endl;
        cout << "description" << info->description << endl;

		// Create stream to listen to the queried sink.
		pa_sample_spec spec;
		spec.channels = 2;
		spec.format = PA_SAMPLE_S16LE;
		spec.rate = 44100;
		this->stream.reset(pa_stream_new(this->context, "Pulsar VU-meter", &spec, nullptr));
		pa_stream_set_read_callback(this->stream.get(), &Monitor::onStreamReadCallback, this);

		pa_stream_connect_record(this->stream.get(), info->monitor_source_name, nullptr, PA_STREAM_PEAK_DETECT);
	}
}
