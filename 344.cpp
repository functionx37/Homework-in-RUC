#include <iostream>
#include <math.h>
#include<iomanip>

using namespace std;


class Triangle {
public:
    Triangle(double x1, double y1, double x2, double y2, double x3, double y3);
    double getArea();
    double getPerimeter();


private:
    double _x1;
    double _y1;
    double _x2;
    double _y2;
    double _x3;
    double _y3;
    //以下为三条边的长度：
    //l1 对应 (x1,y1) -- (x2, y2)之间的边长
    //l2 对应 (x2,y2) -- (x3, y3)之间的边长
    //l3 对应 (x3,y3) -- (x1, y1)之间的边长
    double _l1;
    double _l2;
    double _l3;

};
//在下面的空格实现Triangle类的成员函数
Triangle::Triangle(double x1, double y1, double x2, double y2, double x3, double y3){
    _x1 = x1;
    _x2 = x2;
    _x3 = x3;
    _y1 = y1;
    _y2 = y2;
    _y3 = y3;
    _l1 = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    _l2 = sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2));
    _l3 = sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
}

double Triangle::getArea(){
    double s,p;
    p = (_l1+_l2+_l3)/2;
    s = sqrt(p*(p-_l1)*(p-_l2)*(p-_l3));
    return s;
}

double Triangle::getPerimeter(){
    double a;
    a = _l1+_l2+_l3;
    return a;
}


int main()
{
    double x1, x2, x3, y1, y2, y3;
    cin >>x1>>y1>>x2>>y2>>x3>>y3;
    Triangle t(x1,y1,x2,y2,x3,y3);

//在下面的空格按题目要求输出结果
double c = t.getPerimeter();
    double s = t.getArea();
    cout<<fixed<<setprecision(2)<<c<<"\n"<<fixed<<setprecision(2)<<s;
    
    return 0;
}
