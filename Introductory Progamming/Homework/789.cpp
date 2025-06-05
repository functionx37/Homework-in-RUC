#include <iostream>
#include <string>

class A;

namespace myns
{
    class COUT
    {
    public:
        COUT() {}
        void operator<<(const char a[]); // 成员函数
        myns::COUT operator<<(A b); // 成员函数
    };
    COUT cout;
}

class A
{
public:
    A(int x, int y) : w(x), h(y) {}
    int getLen() const
    {
        if (len < 0)
            len = 2 * (w + h);
        return len;
    }

private:
        mutable int len = -1;
public:
    int w,h;
};

myns::COUT myns::COUT::operator<<(A b){
    std::cout<<"["<<b.w<<","<<b.h<<"]"<<" -> "<<b.getLen();
    return cout;
}
void myns::COUT::operator<<(const char a[]){
    std::cout<<a;
}


using myns::cout;
using std::cin;
int main()
{
    int x, y;
    cin >> x >> y;
    A a1(x, y);
    cout << a1 << "\n"; // first myns::COUT operator<<, then void operator<<
    cin >> x >> y;
    A a2(x, y);
    cout << a2 << "\n";
    return 0;
}