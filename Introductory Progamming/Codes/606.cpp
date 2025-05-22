#include <iostream>
#include <string>

namespace myns {
        class COUT {
                public:
                        COUT() {}
        };
        COUT cout;
}


class A {
        public:
                A(int x, int y) : w(x), h(y) {}
                int getLen() const {
                        if (len < 0) len = 2 * (w + h);
                        return len;
                }
        private:
public:
        mutable int len = -1;
        int w, h;
};

myns::COUT operator<<(myns::COUT cout,A b){
    std::cout<<"["<<b.w<<","<<b.h<<"]"<<" -> "<<b.getLen();
    return cout;
}
void operator<<(myns::COUT cout ,const char a[]){
    std::cout<<a;
}

using myns::cout;
using std::cin;
int main() {
        int x, y;
        cin>>x>>y;
        A a1(x, y);
        cout << a1 << "\n";
        cin>>x>>y;
        A a2(x, y);
        cout << a2 << "\n";
        return 0;
}