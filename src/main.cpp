#include <iostream>

#include "Monitor.h"

using namespace std;
using namespace Pulsar;

int main(int argumentCount, char **arguments)
{
	for (int i = 0; i < argumentCount; i++)
	{
		cout << arguments[i] << endl;
	}
	Monitor monitor;
	return 0;
};
