#include <iostream>
#include <sstream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ostringstream;
using std::istringstream;
using std::ios_base;



string str2hexint(string &str) {
	ostringstream outstr;

		const char *p = str.c_str();
	int begin = 0;
	for(int i=0;i<str.size();i++){
		if(p[i]==' '||p[i]=='	'){
			string ans = str.substr(begin,i-begin);
			int a = stoi(ans);
			outstr<<std::hex<<std::showbase<<a<<" ";
			while(p[i]==' '||p[i]=='	'){
				i++;
			}
			begin = i;
		}
		if(i==str.size()-1){
			string ans = str.substr(begin,i-begin+1);
			int a = stoi(ans);
			outstr<<std::hex<<std::showbase<<a<<" ";
			begin = i;
		}
	}

	return outstr.str();
}

int main() {
	int n;
	string str;
	cin >> n;
	cin.get();
	for (int i = 0; i < n; i++) {
		std::getline(cin, str);
		cout << str2hexint(str) << endl;
	}
	return 0;
}


