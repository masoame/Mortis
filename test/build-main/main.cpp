#include<PE.h>

#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <thread>


template<typename T>
class A
{
protected:
	const T a = 0;
public:
	void print() {
		std::cout << a << std::endl;
	}
};

template<typename T>
class B : public A<T>
{
public:
	void test_change_a() {
		A<T>::a = 80;
	}
};
int main()
{
	std::jthread out;

	B<int> b;

	b.print();
	//b.test_change_a();
	b.print();

}