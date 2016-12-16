#include <iostream>
#include <memory>

#include <pulse/thread-mainloop.h>

using namespace std;

class Monitor
{
	private:
		/** Alias for std::unique_ptr dedicated to PA objects that require a
		 * specific function for disposal. */
		template <class T> using Ptr = std::unique_ptr<T, std::function<void (T*)>>;

	public:
		Monitor()
			: mainloop(pa_threaded_mainloop_new(), pa_threaded_mainloop_free), mainloopApi(nullptr)
			{
				this->mainloopApi = pa_threaded_mainloop_get_api(this->mainloop.get());
			}

	private:
		Ptr<pa_threaded_mainloop> mainloop;
		pa_mainloop_api *mainloopApi;
};

int main(int argumentCount, char **arguments)
{
	for (int i = 0; i < argumentCount; i++)
	{
		cout << arguments[i] << endl;
	}
	Monitor monitor;
	return 0;
};
