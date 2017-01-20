#pragma once

#include "PulseAudio.h"

namespace Pulsar
{
	class Monitor
	{
		public:
			Monitor(pa_mainloop_api *api, const std::string &serverName, const std::string &sinkName);

		public:
			int16_t getAverageLevel() const
			{
				return (this->chunkCount > 0 ? this->chunkSum / (double)this->chunkCount : 0) / (double)INT16_MAX;}

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
			pa_context *context;

			PulseAudio::Pointer<pa_stream> stream;
			size_t chunkCount;
			int32_t chunkSum;
	};
}
