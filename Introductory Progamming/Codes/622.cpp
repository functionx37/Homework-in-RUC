#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

namespace {
	int CountGet = 0;
}

// put your code below for template functions Get and PutAndGet

template <typename T>
void Get(T &a)
{
	cin >> a;
	::CountGet++;
}
template <typename T>
void GetAndPut(int i)
{
	for (int j = 0; j < i; j++)
	{
		T b;
		cin >> b;
		cout << b<<" ";
		::CountGet++;
	}
	cout<<endl;
}


int main() {
	int m, n;
	string tag;
	Get(m);
	Get(n);

	for(int i = 0; i < m; i++) {
		Get(tag);
		if (tag == "int") {
			GetAndPut<int>(n);
		} else if (tag == "double") {
			GetAndPut<double>(n);
		} else if (tag == "string") {
			GetAndPut<string>(n);
		}
	}
	cout << "Count of Get() = " << CountGet << endl;

	return 0;
}

