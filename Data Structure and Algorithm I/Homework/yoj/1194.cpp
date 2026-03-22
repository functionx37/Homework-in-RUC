#include <bits/stdc++.h>
using namespace std;

class que
{
public:
    int cap = 0;
    int siz = 0;
    int *vec;
    int front = 0;
    int rear = 0;
    que(int n)
    {
        cap = n;
        vec = new int[n];
    }
    ~que()
    {
        delete[] vec;
    }
    int Front()
    {
        return isEmpty() ? -1 : vec[front];
    }
    int Rear()
    {
        return isEmpty() ? -1 : vec[(rear - 1 + cap) % cap];
    }
    bool isEmpty()
    {
        return siz == 0;
    }
    bool isFull()
    {
        return siz == cap;
    }
    bool enQueue(int value)
    {
        if (isFull())
            return 0;
        vec[rear] = value;
        ++siz;
        rear = (rear + 1) % cap;
        return 1;
    }
    bool deQueue()
    {
        if (isEmpty())
            return 0;
        --siz;
        front = (front + 1) % cap;
        return 1;
    }
};

int main()
{
    int n, m;
    cin >> n >> m;
    que a = que(n);
    while (m--)
    {
        int k;
        cin >> k;
        switch (k)
        {
        case 0:
        {
            cout << a.Front() << endl;
            break;
        }
        case 1:
        {
            cout << a.Rear() << endl;
            break;
        }
        case 2:
        {
            int v;
            cin >> v;
            cout << a.enQueue(v) << endl;
            break;
        }
        case 3:
        {
            cout << a.deQueue() << endl;
            break;
        }
        case 4:
        {
            cout << a.isEmpty() << endl;
            break;
        }
        case 5:
        {
            cout << a.isFull() << endl;
            break;
        }
        }
    }
    return 0;
}