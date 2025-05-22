#include <iostream>

using std::cout;
using std::endl;
using std::cin;

namespace {
    int g = 10;
}

namespace myns {

    int g = 20;

    int add(int i){
        return g + ::g+ i;
    }

    int sub(int i){
        return g + ::g - i;
    }

}

int main() {
        int x, y;
        cin >> x >> y;
        int a = myns::add(x);
        cout << a << endl;
        int b = myns::sub(y);
        cout << b << endl; 
        return 0;
}
