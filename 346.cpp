#include <iostream>
using namespace std;
class Complex
{
public:
	double shi;
	double xu;
	Complex(double a, double b = 0) : shi(a), xu(b) {};
	void show()
	{
		if (shi != 0 && xu == 0)
			cout << shi << endl;
		else if (shi == 0 && xu != 0)
			cout << xu << "i\n";
		else if (shi == 0 && xu == 0)
			cout << "0\n";
		else if(xu>0)
			cout << shi << "+" << xu << "i\n";
		else if(xu<0)
			cout<<shi<<xu<<"i\n";
	}
	void add(Complex b)
	{
		shi += b.shi;
		xu += b.xu;
	}
	void sub(Complex b)
	{
		shi -= b.shi;
		xu -= b.xu;
	}
};

int main()
{
	double re, im;
	cin >> re >> im;
	Complex c1(re, im);		// 用re, im初始化c1
	cin >> re;
	Complex c2 = re;     		// 用实数re初始化c2

	c1.show();
	c2.show();
	c1.add(c2);         // 将C1与c2相加，结果保存在c1中
	c1.show();          // 将c1输出
	c2.sub(c1);			// c2-c1，结果保存在c2中 
	c2.show();			// 输出c2 
	
    return 0;
}
