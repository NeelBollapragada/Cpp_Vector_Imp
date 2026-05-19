#include <iostream>
#include <vector>
#include "vector.h"

struct Counted {
	static inline int constructed{};
	static inline int destroyed{};

	int value{};

	Counted(int v) : value{v} {
		++constructed;
	}

	Counted(const Counted& other) : value{other.value} {
		++constructed;
	}

	Counted(Counted&& other) noexcept : value{other.value} {
		++constructed;
	}

	~Counted() {
		++destroyed;
	}
};

int main() {
	{
		Vector<Counted> v;

		for (int i = 0; i < 65; ++i) {
			v.emplace(i);
		}

		std::cout << "constructed: " << Counted::constructed << '\n';
		std::cout << "destroyed before scope end: " << Counted::destroyed << '\n';
	}

	std::cout << "destroyed after scope end: " << Counted::destroyed << '\n';
}
