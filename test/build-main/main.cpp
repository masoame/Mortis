#include<PE.h>
#include<iostream>

template<class Object>
class CrtpBase {
	protected:
	CrtpBase() = default;


};
class A : public CrtpBase<A>
{
public:
	A() = default;
};


class C : public Mortis::Singleton<C> {

};
class D : public Mortis::Singleton<D> {

};

int main() {
	
	A a;
	a;
	return 0;
}
