#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

namespace {
	int CountGet = 0;
}

class Node {
public:
	int x;
	double y;
};

std::istream& operator>>(std::istream& in, Node& node) {
	cout << "Should Not Enter Here" << endl;
	return in;
}

template <typename T>
void Get(T &a)
{
	cin >> a;
	::CountGet++;
}

template <typename T>
void GetAndPut(int k)
{
	for(int i=0;i<k;i++){
		T a;
		cin>>a;
		cout<<a<<" ";
		::CountGet++;
	}
	cout<<endl;
}
template <>
void GetAndPut<Node>(int k)
{
	for(int i=0;i<k;i++){
		Node a;
		cin>>a.x>>a.y;
		cout<<a.x<<" "<<a.y<<" ";
		::CountGet++;
		::CountGet++;
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
		} else if (tag == "node") {
			GetAndPut<Node>(n);
		}
	}
	cout << "Count of Get() = " << CountGet << endl;

	return 0;
}

