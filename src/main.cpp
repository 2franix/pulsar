#include <iostream>

using namespace std;

int main(int argumentCount, char **arguments)
{
	for (int i = 0; i < argumentCount; i++)
	{
		cout << arguments[i] << endl;
	}
	return 0;
};
