#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

class Node {
public:
	int x;
	double y;
};

// put the declaration of Triplet below
/*

std::istream& operator>>(std::istream& in, Node& node) {
	cout << "Should Not Enter Here" << endl;
	return in;
}

template<typename T1, typename T2, typename T3>
std::istream& operator>>(std::istream& in, Triplet<T1, T2, T3>& triplet) {
	cout << "Should Not Enter Here" << endl;
	return in;
}

// put the other code below
*/
template <typename T>
void Get(T &a)
{
	cin >> a;
}
template <typename T>
void GetAndPut(int k)
{
	for (int i = 0; i < k; i++)
	{
		T a;
		cin >> a;
		cout << a << " ";
	}
	cout << endl;
}
template <>
void GetAndPut<Node>(int k)
{
	for (int i = 0; i < k; i++)
	{
		Node a;
		cin >> a.x >> a.y;
		cout << a.x << " " << a.y << " ";
	}
	cout << endl;
}
template<typename T1,typename T2>
void out(int k){}
template<>
void out<int,Node>(int k){
	for(int i=0;i<k;i++){
		int a;
		Node b;
		cin>>a>>b.x>>b.y;
		cout<<a<<" "<<b.x<<" "<<b.y<<" ";
	}
	cout<<endl;
}
template<>
void out<Node,int>(int k){
	for(int i=0;i<k;i++){
		int a;
		Node b;
		cin>>b.x>>b.y>>a;
		cout<<b.x<<" "<<b.y<<" "<<a<<" ";
	}
	cout<<endl;
}
template<>
void out<int,string>(int k){
	for(int i=0;i<k;i++){
		int a;
		string b;
		string c;
		cin>>a>>b>>c;
		cout<<a<<" "<<b<<" "<<c<<" ";
	}
	cout<<endl;
}
template<>
void out<string,string>(int k){
	for(int i=0;i<k;i++){
		string a;
		string b;
		string c;
		cin>>a>>b>>c;
		cout<<a<<" "<<b<<" "<<c<<" ";
	}
	cout<<endl;
}
template<>
void out<Node,string>(int k){
	for(int i=0;i<k;i++){
		Node a;
		string b;
		cin>>a.x>>a.y>>b;
		cout<<a.x<<" "<<a.y<<" "<<b<<" ";
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
		} else if (tag == "triplet") {
			string tr1, tr2;
			Get(tr1);
			Get(tr2);
			if (tr1 == "int" && tr2 == "node") {
				// call GetAndPut below
				out<int,Node>(n);
			} else if (tr1 == "int" && tr2 == "any") {
				// call GetAndPut below
				out<int,string>(n);
			} else if (tr1 == "node" && tr2 == "int") {
				// call GetAndPut below
				out<Node,int>(n);
			} else if (tr1 == "any" && tr2 == "any") {
				// call GetAndPut below
				out<string,string>(n);
			} else if (tr1 == "node" && tr2 == "any") {
				// call GetAndPut below
				out<Node,string>(n);
			}
		}
	}

	return 0;
}

