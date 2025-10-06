#include<utils.hpp>

#include<iostream>

struct foo {
	// Notice how we accept a double as first argument here.
	// This is fine because float is convertible to double.
	// 's' is a reference and can thus be modified.
	void bar(double d, int i, bool b, std::string& s) {
		s = b ? std::to_string(i) : std::to_string(d);
	}
};

// Function objects can cope with default arguments and overloading.
// It does not work with static and member functions.
struct obj {
	void operator()(float, int, bool, std::string&, int = 0) {
		std::cout << "I was here\n";
	}

	void operator()() {}
};

int main() {
	// declare a signal with float, int, bool and string& arguments
	sigslot::signal<float, int, bool, std::string&> sig;

	// a generic lambda that prints its arguments to stdout
	auto printer = [](auto a, auto && ...args) {
		std::cout << a;

		//(void)std::initializer_list<int>{
		//	((void)(std::cout << " " << args), 1)...
		//};

		((std::cout << " " << args), ...);

		std::cout << "\n";
		};

	// connect the slots
	foo ff;
	sig.connect(printer,1);
	sig.connect(&foo::bar, &ff,1);
	sig.connect(obj());

	float f = 1.f;
	short i = 2;  // convertible to int
	std::string s = "0";

	// emit a signal
	sig(f, i, false, s);
	sig(f, i, true, s);
}