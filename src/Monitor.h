#pragma once

#include "PulseAudio.h"

namespace Pulsar
{
	class Monitor
	{
		public:
			Monitor();

		private:
			void onContextStateChanged(pa_context *context);
			//static contextStateChangedCallback(pa_context *context, void *monitor);

		private:
			PulseAudio::Pointer<pa_threaded_mainloop> mainloop;
			pa_mainloop_api *mainloopApi;

			/** Connection to the PA server. */
			pa_context *context;
	};
}
