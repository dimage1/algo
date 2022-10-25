
#include <stdio.h>
#include <ostream>
#include <iostream>

template<class T1, class T2>
class Test 
{
    T1 x;
    T2 y;
  
public:
    Test(const T1 &x_, const T2 &y_) : x(x_), y(y_)  {}
	Test(const Test& other) : x(other.x), y(other.y)  {}
    Test() = delete;
	
	~Test() = default;
    
	Test& operator=(const Test& other)
	{
		x = other.x;
		y = other.y;
		
		return *this;
	}
	
    bool operator==(const Test& brother) const 
    { 
        return brother.x == x && brother.y == y; 
    }
    bool operator!=(const Test& rhs) const
    {
        return !operator==(rhs);
    }

    
    friend std::ostream& operator<<(std::ostream &os, const Test &t)
    {
        os << "(" << t.x << ", " << t.y << ")";
        return os;
    }

    const T1& get1() const {return x;}
    const T2& get2() const {return y;}
};

int main()
{
    auto x = Test(1, 2);
    auto y = Test(3, 4);
    
    Test t(x, y);
    
    auto tmp = y;
    y = x;
    x = tmp;

    std::cout << t << " - " << x << " - " << y << std::endl;

    return 0;
}


--std=c++20 --stdlib=libc++
