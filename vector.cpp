#include <iostream>
#include <memory>
#include <utility>

constexpr size_t INIT_CAP{64};

class A {
private:
	int x{};
	double y{};

public:
	A(int a, double b)
		: x{a}, y{b}
	{ std:: cout << "A constructed\n"; };

	void print() const {
		std::cout << "print " << x << " " << y << '\n';
	}
};

template <typename T>
class Vector {
private:
	size_t size{};
	size_t capacity{};
	std::allocator<T> alloc;
	T* ptr{ nullptr };

public:
	Vector() = default;

	Vector(const Vector& v) {
		throw std::runtime_error("Cannot copy vector object.");
	};

	void operator=(const Vector& v) {
		throw std::runtime_error("Cannot copy assign vector object.");
	}

	Vector(Vector&& v)
		: size{v.size}, capacity{v.capacity}, ptr{v.ptr}
	{}

	void operator=(Vector&& v) {
		size = v.size;
		capacity = v.capacity;
		ptr = v.ptr;
	}

	Vector(int s) {
		if (s < 0)
			throw std::runtime_error("Size cannot be negative.");

		if (s > 0) {
			capacity = s;
			ptr = alloc.allocate(s);
		}
	}

	Vector(std::initializer_list<T> init_list) 
		: capacity{init_list.size()}, size{init_list.size()}
	{
		ptr = alloc.allocate(capacity);
		size_t i{};
		for (auto p{init_list.begin()}; p != init_list.end(); ++p) {
			std::construct_at(ptr + i++, *p);
		}
	}

	void push(const T& obj) {
		if (size == capacity) {
			resize_inc();
		}

		std::construct_at(ptr + size, obj);
		++size;
	}

	void push(T&& obj) {
		if (size == capacity) {
			resize_inc();
		}

		std::construct_at(ptr + size, std::move(obj));
		++size;
	}

	T pop() {
		if (size == 0)
			throw std::runtime_error("Cannot pop from empty vector.");

		--size;

		if ((size << 1) <= (capacity >> 1)) {
			resize_dec();
		}

		T ret_elem = ptr[size];

		std::destroy_at(ptr + size);

		return ret_elem;
	}

	template <typename... Args>
	T& emplace(Args&&... args) {
		if (size == capacity) {
			resize_inc();
		}

		std::construct_at(ptr + size, T{std::forward<Args>(args)...});
		return ptr[size++];
	}

	void reserve(int extra_capacity) {
		if (extra_capacity <= 0)
			throw std::runtime_error("Size cannot be non-positive.");

		resize_inc(extra_capacity);
	}

	const T& peek() const {
		if (size == 0)
			throw std::runtime_error("Cannot peek at empty vector.");

		return ptr[size];
	}

	bool isEmpty() const {
		return size == 0;
	}

	size_t getSize() const {
		return size;
	}

	size_t getCapacity() const {
		return capacity;
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector<T>& v);

	void operator[](size_t index) {
		return ptr[index];
	}

private:
	void resize_inc(size_t extra_capacity = 0) {
		if (capacity == 0) {
			ptr = alloc.allocate(INIT_CAP);
			capacity = INIT_CAP;
			return;
		}

		size_t new_capacity{ capacity << 1 };

		if (extra_capacity) {
			new_capacity = capacity + extra_capacity; 
		}

		T* new_ptr = alloc.allocate(new_capacity);

		for (size_t i{}; i < size; ++i) {
			std::construct_at(new_ptr + i, std::move(ptr[i]));
		}

		alloc.deallocate(ptr, capacity);
		ptr = new_ptr;
		new_ptr = nullptr;

		capacity = new_capacity;
	}

	void resize_dec() {
		T* new_ptr = alloc.allocate(capacity >> 1);

		for (size_t i{}; i < size; ++i) {
			std::construct_at(new_ptr + i, std::move(ptr[i]));
		}

		alloc.deallocate(ptr, capacity);
		ptr = new_ptr;
		new_ptr = nullptr;

		capacity >>= 1;
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Vector<T>& v) {
	out << "Vector: { ";
	for (auto i{0uz}; i < v.getSize(); ++i) {
		out << v[i] << " ";
	}

	out << "}";

	return out;
}

int main() {

	Vector<A> v;

	v.emplace(10, 2.0);

	Vector<int> v2; 

	return 0;
}
