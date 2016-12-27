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

		private:
			std::string serverName;
			pa_mainloop_api *mainloopApi;

			/** Connection to the PA server. */
			pa_context *context;
	};
}
