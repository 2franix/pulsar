#pragma once

#include <string>

#include "PulseAudio.h"

namespace Pulsar
{
	class Monitor
	{
		public:
			Monitor(pa_mainloop_api *api, const std::string &serverName, const std::string &sinkName);

		public:
			bool hasSamples() const {return this->sampleCount > 0;}

			size_t getSampleCount() const
			{
				return this->sampleCount;
			}

			const std::string &getSinkName() const {return sinkName;}

		private:
			void onContextStateChanged(pa_context *context);
			void onSinkInfo(const pa_sink_info *info);
			void onStreamRead(size_t byteCount);
			static void onContextStateChangedCallback(pa_context *context, void *monitor);
			static void onSinkInfoCallback(pa_context *context, const pa_sink_info *sinkInfo, int eol, void *monitor);
			static void onStreamReadCallback(pa_stream *stream, size_t byteCount, void *monitor);

		private:
			std::string serverName;
			std::string sinkName;
			pa_mainloop_api *mainloopApi;

			/** Connection to the PA server. */
			PulseAudio::Pointer<pa_context> context;

			PulseAudio::Pointer<pa_stream> stream;
			size_t sampleCount;
	};
}
