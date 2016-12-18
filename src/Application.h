#pragma once

#include <string>

namespace Pulsar
{
	class Application
	{
		public:
			/** Singleton application object. */
			static Application &get();
			
			const std::string &getName() const {return this->name;}

		private:
			std::string name;
	};
};
