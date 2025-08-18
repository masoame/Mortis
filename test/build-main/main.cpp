#include<PE.h>
#include<iostream>
//using namespace Mortis;
//using namespace Mortis::PE;

class base {
private:
	base() = default;
	~base() = default;
	base(const base&) = delete;
	base& operator=(const base&) = delete;
	base(base&&) = delete;
	base& operator=(base&&) = delete;
public:
	static base& Instance() {
		static base instance{};
		return instance;
	}
};

class A : public base{

};
class B : public base {

};

class C : public Mortis::Singleton<C> {

};
class D : public Mortis::Singleton<D> {

};

int main() {
	std::cout << std::hex << &A::Instance() << std::endl;
	std::cout << std::hex << &B::Instance() << std::endl;
	std::cout << std::hex << &C::Instance() << std::endl;
	std::cout << std::hex << &D::Instance() << std::endl;

	return 0;
}
