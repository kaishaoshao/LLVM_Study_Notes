#include <iostream>
using namespace std;
int main()
{
	int m = 1000;
	long n = static_cast<long>(m);  			//宽转换  ，也即范围小的数转换为范围大的数，没有信息丢失
	char ch = static_cast<char>(m); 			//窄转换 ，也即范围大的数转换为范围小的数，可能会丢失信息
	int *p1 = static_cast<int*>(malloc(10 * sizeof(int)));//将void指针转换为具体类型指针
	void *p2 = static_cast<void*>(p1); 		//将具体类型指针，转换为void指针

	//下面的用法是错误的
	//float *p3 = static_cast<float*>(p1); 		//不能在两个具体类型的指针之间转换
	//float * p34 = static_cast<float*>(100); 	//不能将整数转换为指针类型
	
	cout << m << endl;
	cout << ch << endl;
	cout << p1 << endl;
	cout << p2 << endl;

	return 0;
}
