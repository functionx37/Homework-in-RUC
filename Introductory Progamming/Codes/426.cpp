#include <iostream>
#include <math.h>

using namespace std;

class Point
{
public:
    Point(double x, double y);
    Point(const Point &p);
    double getX() const;
    double getY() const;
protected:
    double _x, _y;
};

Point::Point(double x, double y):_x(x), _y(y){
}

Point::Point(const Point &p):_x(p._x), _y(p._y){
}

double Point::getX() const
{
    return this->_x;
}

double Point::getY() const
{
    return this->_y;
}

class Rectangle{
public:
    Rectangle(const Point & lb, const Point & rt);
       Point getLB(){
        return _lb;
   };
   Point getRT(){
        return _rt;
   };

 private:
    Point _lb, _rt;
};

Rectangle::Rectangle(const Point & lb, const Point & rt):_lb(lb),_rt(rt){};
Rectangle operator+(Rectangle a,Rectangle b){
    Point x(min(a.getLB().getX(),b.getLB().getX()),min(a.getLB().getY(),b.getLB().getY()));
    Point y(max(a.getRT().getX(),b.getRT().getX()),max(a.getRT().getY(),b.getRT().getY()));
    Rectangle ans(x,y);
    return ans;
}
 
void printRectangle(Rectangle rec){
    cout<<rec.getLB().getX()<<" "<<rec.getLB().getY()<<endl;
    cout<<rec.getRT().getX()<<" "<<rec.getRT().getY()<<endl;
}

int main()
{
    double x1, y1, x2, y2;
    cin>>x1>>y1>>x2>>y2;
    Rectangle r1(Point(x1,y1), Point(x2,y2));
    cin>>x1>>y1>>x2>>y2;
    Rectangle r2(Point(x1,y1), Point(x2,y2));
    Rectangle r3 = r1 + r2;
    printRectangle(r3);
    return 0;
}
