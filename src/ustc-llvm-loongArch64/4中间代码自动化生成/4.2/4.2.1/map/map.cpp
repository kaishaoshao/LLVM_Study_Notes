#include <iostream>
#include <map>							// 头文件
#include <string>
using namespace std;
 
int main() {
	map<int, string> node; 					// 定义变量
	map<int, string>::iterator iter; 			// 定义迭代器 iter
	node[16] = "张三"; 					// 以数组下标的形式
	node.insert(pair<int, string>(28, "李四"));	// 直接插入键值对，pair定义了一个键值
										// 对，对应map的key和value。
	node[78] = "陆七";
	node[58] = "陈二";
	node[39] = "王五";

	for(iter = node.begin(); iter != node.end(); ++iter) {
		cout<<"工号"<<iter->first<<"："<<iter->second<<"，"<<endl;
	} 
	// 输出“工号16：张三，工号28：李四，39：王五，工号58：陈二，工号78：陆七”

	node.erase(58); 						// 使用key删除key=58的节点

	iter = node.find(78); 					// 使用迭代器查找key=78的节点
	node.erase(iter); 						// 删除key=78的节点

	node[28] = "赵四";						// 仅能修改 value 的值

	for(iter = node.begin(); iter != node.end(); ++iter) {
		cout<<"工号"<<iter->first<<"："<<iter->second<<"，"<<endl;
	}
	// 输出“工号16：张三，工号28：赵四，39：王五”
}
