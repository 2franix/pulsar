#pragma once

#include "pulse/context.h"
#include "pulse/thread-mainloop.h"
#include "pulse/introspect.h"

#include <memory>


namespace Pulsar
{
	class PulseAudio
	{
		public:
			/** Alias for std::unique_ptr dedicated to PA objects that require a
			 * specific function for disposal. */
		template <class T> using Pointer = std::unique_ptr<T, std::function<void (T*)>>;
	};
}
