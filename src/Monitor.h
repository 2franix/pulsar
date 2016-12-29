#pragma once

#include "PulseAudio.h"

namespace Pulsar
{
	class Monitor
	{
		public:
			Monitor(pa_mainloop_api *api, const std::string &serverName);

		private:
			void onContextStateChanged(pa_context *context);
			void onSinkInfo(const pa_sink_info *info);
			static void onContextStateChangedCallback(pa_context *context, void *monitor);
			static void onSinkInfoCallback(pa_context *context, const pa_sink_info *sinkInfo, int eol, void *monitor);

		private:
			std::string serverName;
			pa_mainloop_api *mainloopApi;

			/** Connection to the PA server. */
			pa_context *context;
	};
}
