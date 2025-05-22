#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

template<typename T>
class Player {
protected:
	T data;
public:
	Player(T& t) : data(t) {}

	void shuffle(){
		cout<<"Player shuffles with "<<data<<",";
	}
	template<typename Q>
	void picker(Q b){
		cout<<" picks "<<b<<endl;
	}
};

// other declarations and implementations
// In Winner, use this->data, instead of "data".
template<typename t1,typename t2>
class Winner{
public:
	t1 data1;
	t2 data2;
	Winner(t1 a,t2 b):data1(a),data2(b){
		cout<<"Winner shuffles with "<<data1<<" and "<<data2<<", picks ";
	};
};
template<typename t1,typename t2>
void dump(Winner<t1,t2> winner){
	string tag;
	cin>>tag;
	if (tag == "string") {
		string a;
		cin>>a;
		cout<<a<<endl;
	} else if (tag == "int") {
		int a;
		cin>>a;
		cout<<a<<endl;
	} else if (tag == "double") {
		double a;
		cin>>a;
		cout<<a<<endl;
	}
}

//put the declaration of "dump" below
template <typename T>
void dump(Player<T> player) {
	string tag;
	player.shuffle();
	cin >> tag;
	if (tag == "string") {
		cin >> tag;
		player.picker(tag);
	} else if (tag == "int") {
		int i32;
		cin >> i32;
		player.picker(i32);
	} else if (tag == "double") {
		double d64;
		cin >> d64;
		player.picker(d64);
	}
}

int main() {
	int n;
	string tag;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> tag;
		if (tag == "player") {
			cin >> tag;
			if (tag == "int") {
				int data1;
				cin >> data1;
				Player<int> player(data1);
				dump(player);
			} else if (tag == "string") {
				string data1;
				cin >> data1;
				Player<string> player(data1);
				dump(player);
			} else if (tag == "double") {
				double data1;
				cin >> data1;
				Player<double> player(data1);
				dump(player);
			}
		} else if (tag == "winner") {
			string tag2;
			cin >> tag >> tag2;
			if (tag == "int" && tag2 == "int") {
				int data1, data2;
				cin >> data1 >> data2;
				Winner<int, int> winner(data1, data2);
				dump(winner);
			} else if (tag == "int" && tag2 == "string") {
				int data1;
				string data2;
				cin >> data1 >> data2;
				Winner<decltype(data1), decltype(data2)> winner(data1, data2);
				dump(winner);
			} else if (tag == "int" && tag2 == "double") {
				int data1;
				double data2;
				cin >> data1 >> data2;
				Winner<decltype(data1), decltype(data2)> winner(data1, data2);
				dump(winner);
			} else if (tag == "string" && tag2 == "string") {
				string data1;
				string data2;
				cin >> data1 >> data2;
				Winner<decltype(data1), decltype(data2)> winner(data1, data2);
				dump(winner);
			} else if (tag == "string" && tag2 == "int") {
				string data1;
				int data2;
				cin >> data1 >> data2;
				Winner<decltype(data1), decltype(data2)> winner(data1, data2);
				dump(winner);
			} else if (tag == "string" && tag2 == "double") {
				string data1;
				double data2;
				cin >> data1 >> data2;
				Winner<decltype(data1), decltype(data2)> winner(data1, data2);
				dump(winner);
			} else if (tag == "double" && tag2 == "double") {
				double data1;
				double data2;
				cin >> data1 >> data2;
				Winner<decltype(data1), decltype(data2)> winner(data1, data2);
				dump(winner);
			} else if (tag == "double" && tag2 == "string") {
				double data1;
				string data2;
				cin >> data1 >> data2;
				Winner<decltype(data1), decltype(data2)> winner(data1, data2);
				dump(winner);
			} else if (tag == "double" && tag2 == "int") {
				double data1;
				int data2;
				cin >> data1 >> data2;
				Winner<decltype(data1), decltype(data2)> winner(data1, data2);
				dump(winner);
			}
		}
	}
}

