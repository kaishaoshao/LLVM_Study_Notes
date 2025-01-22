#include<iostream>
#include<memory>
using namespace std;

struct Point {
	int x;
	int y;
	Point(int x, int y) : x(x), y(y) {}
	void print() {
		std::cout << "(" << x << ", " << y << ")";
	}
};

bool is_equal(Point *p1, Point *p2) {
	return p1->x == p2->x and p1->y == p2->y;
}

int main() {
	auto p1 = new Point(2, 3);				// 调用构造函数生成普通的Point指针p1
	p1->print();							// 在p1上调用print方法
	std::cout << std::endl;

	auto p2 = std::make_unique<Point>(4, 5);	// 调用make_unique生成unique_ptr p2
	p2->print();							// 在p2上调用print方法，和普通指针用法相同
	std::cout << std::endl;

	auto res = is_equal(p1, p2.get());			// 用get方法获取unique_ptr管理的Point*类型的指针
	if (res) {
		std::cout << "equal";
	} else {
		std::cout << "not equal";
	}

	delete p1;								// 非智能指针管理的指针需要手动释放，p2则不需要
}
