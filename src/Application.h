#pragma once

#include <string>

namespace Pulsar
{
	class Application
	{
		public:
			enum LogLevel
			{
				Info = 1,
				Warning = 2,
				Error = 3
			};

		private:
			Application();

		public:
			/** Singleton application object. */
			static Application &get();
			
			const std::string &getName() const {return this->name;}

			const std::string &getVersion() const {static std::string version="1.0.0b"; return version;}

			void setLogLevel(LogLevel level) {this->logLevel = level;}
	 		void printInfo(const std::string &message) {this->printMessage(LogLevel::Info, message);}
	 		void printError(const std::string &message) {this->printMessage(LogLevel::Error, message);}
	 		void printWarning(const std::string &message) {this->printMessage(LogLevel::Warning, message);}

		private:
			void printMessage(LogLevel level, const std::string &message);

		private:
			std::string name;
			LogLevel logLevel;
	};
};
