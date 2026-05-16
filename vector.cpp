#include <iostream>
#include <memory>

constexpr size_t INIT_CAP{64};

template <typename T>
class Vector {
private:
	size_t size{};
	size_t capacity{};
	std::unique_ptr<T[]> ptr{ nullptr };

public:
	Vector() = default;

	Vector(const Vector& v) {
		throw std::runtime_error("Cannot copy vector object.");
	};

	void operator=(const Vector& v) {
		throw std::runtime_error("Cannot copy assign vector object.");
	}

	Vector(Vector&& v)
		: size{v.size}, capacity{v.capacity}, ptr{std::move(v.ptr)}
	{}

	void operator=(Vector&& v) {
		size = v.size;
		capacity = v.capacity;
		ptr = std::move(v.ptr);
	}

	Vector(int s) {
		if (s < 0)
			throw std::runtime_error("Size cannot be negative.");

		if (s > 0) {
			capacity = s;
			ptr = std::make_unique<T[]>(s);
		}
	}

	void push(const T& obj) {
		if (size == capacity) {
			std::cout << "here\n";
			resize_inc();
		}

		ptr[size] = obj;
		++size;
	}

	T pop() {
		if (size == 0)
			throw std::runtime_error("Cannot pop from empty vector.");

		--size;

		if ((size << 1) <= (capacity >> 1)) {
			resize_dec();
		}

		return ptr[size];
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
			ptr = std::make_unique<T[]>(INIT_CAP);
			capacity = INIT_CAP;
			return;
		}

		size_t new_capacity{ capacity << 1 };

		if (extra_capacity) {
			new_capacity = capacity + extra_capacity; 
		}

		std::unique_ptr<T[]> new_ptr = std::make_unique<T[]>(new_capacity);

		for (size_t i{}; i < size; ++i) {
			new_ptr[i] = ptr[i];
		}

		ptr = std::move(new_ptr);
		new_ptr = nullptr;

		capacity = new_capacity;
	}

	void resize_dec() {
		std::unique_ptr<T[]> new_ptr = std::make_unique<T[]>(capacity >> 1);

		for (size_t i{}; i < size; ++i) {
			new_ptr[i] = ptr[i];
		}

		ptr = std::move(new_ptr);
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

	Vector<int> v;

	for (int i{}; i < 64; ++i) {
		v.push(i);
	}

	std::cout << v.getCapacity() << '\n';

	std::cout << v.getSize() << '\n';

	v.push(64);

	std::cout << v.getCapacity() << '\n';

	for (int i{}; i < 32; ++i) {
		v.pop();
	}

	std::cout << v.getCapacity() << '\n';
    
	std::cout << "size " << v.getSize() << '\n';

	std::cout << v.pop() << '\n';

	std::cout << v.getCapacity() << '\n';
	
	Vector<int> v2(10);

	std::cout << v2.getCapacity() << '\n';

	v2.reserve(30);

	std::cout << v2.getCapacity() << '\n';

	return 0;
}
