#include <iostream>
#include <random>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <type_traits>
#include <algorithm>

template <typename T>
T saturate_add(T a, T b) {
	double res = static_cast<double>(a) + static_cast<double>(b);
	if (res > static_cast<double>(std::numeric_limits<T>::max())) {
		return std::numeric_limits<T>::max();
	}
	if (res < static_cast<double>(std::numeric_limits<T>::lowest())) {
		return std::numeric_limits<T>::lowest();
	}
	return static_cast<T>(res);
}

inline bool saturate_add(bool a, bool b) {
	return a || b;
}

template <typename T>
T saturate_sub(T a, T b) {
	double res = static_cast<double>(a) - static_cast<double>(b);
	if (res > static_cast<double>(std::numeric_limits<T>::max())) {
		return std::numeric_limits<T>::max();
	}
	if (res < static_cast<double>(std::numeric_limits<T>::lowest())) {
		return std::numeric_limits<T>::lowest();
	}
	return static_cast<T>(res);
}

inline bool saturate_sub(bool a, bool b) {
	return a && !b;
}

template <typename T>
T saturate_mul(T a, T b) {
	double res = static_cast<double>(a) * static_cast<double>(b);
	if (res > static_cast<double>(std::numeric_limits<T>::max())) {
		return std::numeric_limits<T>::max();
	}
	if (res < static_cast<double>(std::numeric_limits<T>::lowest())) {
		return std::numeric_limits<T>::lowest();
	}
	return static_cast<T>(res);
}

inline bool saturate_mul(bool a, bool b) {
	return a && b;
}

template <typename T>
T saturate_invert(T val) {
	double inv = -static_cast<double>(val);
	double maxlim = static_cast<double>(std::numeric_limits<T>::max());
	double minlim = static_cast<double>(std::numeric_limits<T>::lowest());
	if (inv > maxlim) inv = maxlim;
	if (inv < minlim) inv = minlim;
	return static_cast<T>(inv);
}
inline bool saturate_invert(bool val) {
	return !val;
}

template <typename T>
class Grayscale {
private:
	int rows;
	int cols;
	T* data;

public:
	static const double epsilon;  // статик - одна для всех Grayscale

	Grayscale(int _rows, int _cols, bool choice) : rows(_rows), cols(_cols), data(nullptr) {
		if (rows <= 0 || cols <= 0) {
			throw std::invalid_argument("Sizes should be > 0");
		}

		data = new T[rows * cols];
		if (choice == true) {
			std::random_device rd;
			std::mt19937 gen(rd());
			for (int i = 0; i < rows * cols; ++i) {
				data[i] = generate_random_value<T>(gen);
			}
		}
		else {
			for (int i = 0; i < rows * cols; ++i) {
				data[i] = T{};
			}
		}
	}

	int get_rows() const { return rows; }
	int get_cols() const { return cols; }

	~Grayscale() {
		delete[] data;
	}

	Grayscale(const Grayscale& other) : rows(other.rows), cols(other.cols), data(new T[other.rows * other.cols]) {
		for (int i = 0; i < rows * cols; ++i) {
			data[i] = other.data[i];
		}
	}

	Grayscale& operator = (const Grayscale& other) {
		if (this != &other) {
			T* newdata = new T[other.rows * other.cols];
			for (int i = 0; i < other.rows * other.cols; ++i) {
				newdata[i] = other.data[i];
			}
			delete[] data;
			data = newdata;
			rows = other.rows;
			cols = other.cols;
		}
		return *this;
	}

	T& operator () (int r, int c) {
		if (r >= rows || c >= cols) {
			throw std::out_of_range("Index is out of range");
		}
		return data[r * cols + c];
	}

	const T& operator () (int r, int c) const {
		if (r < 0 || r >= rows || c < 0 || c >= cols) {
			throw std::out_of_range("Index is out of range");
		}
		return data[r * cols + c];
	}

	bool operator == (const Grayscale& other) const {
		if (rows != other.rows || cols != other.cols) {
			return false;
		}
		for (int i = 0; i < rows * cols; ++i) {
			if (std::abs(static_cast<double>(data[i]) - static_cast<double>(other.data[i])) >= epsilon) {
				return false;
			}
		}
		return true;
	}

	bool operator != (const Grayscale& other) const {
		return !(*this == other);
	}

	Grayscale operator+(const Grayscale& other) const {
		if (rows != other.rows || cols != other.cols) {
			throw std::invalid_argument("Matrix sizes must match");
		}
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			result.data[i] = saturate_add(data[i], other.data[i]);
		}
		return result;
	}

	Grayscale operator + (T num) const {
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			result.data[i] = saturate_add(data[i], num);
		}
		return result;
	}

	Grayscale operator-(const Grayscale& other) const {
		if (rows != other.rows || cols != other.cols) {
			throw std::invalid_argument("Matrix sizes must match");
		}
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			result.data[i] = saturate_sub(data[i], other.data[i]);
		}
		return result;
	}

	Grayscale operator - (T num) const {
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			result.data[i] = saturate_sub(data[i], num);
		}
		return result;
	}

	Grayscale operator*(const Grayscale& other) const {
		if (rows != other.rows || cols != other.cols) {
			throw std::invalid_argument("Matrix sizes must match");
		}
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			result.data[i] = saturate_mul(data[i], other.data[i]);
		}
		return result;
	}

	Grayscale operator * (T num) const {
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			result.data[i] = saturate_mul(data[i], num);
		}
		return result;
	}

	Grayscale operator / (T num) const {
		if (num == T{}) {
			throw std::invalid_argument("Error: division by zero");
		}
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			result.data[i] = static_cast<T>(static_cast<double>(data[i]) / static_cast<double>(num));
		}
		return result;
	}

	Grayscale operator!() const {
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			result.data[i] = saturate_invert(data[i]);
		}
		return result;
	}

	// коммутативность
	friend Grayscale operator + (T num, const Grayscale& img) {
		return img + num;
	}

	friend Grayscale operator * (T num, const Grayscale& img) {
		return num * scalar;
	}
	
	double fill_factor() const {
		double sum = 0;
		for (int i = 0; i < rows * cols; ++i) {
			sum += static_cast<double>(data[i]);
		}
		double maxlim = static_cast<double>(std::numeric_limits<T>::max());
		if (std::abs(maxlim) < epsilon) maxlim = 1;

		return sum / (static_cast<double>(rows * cols) * maxlim);
	}
	
	friend std::ostream& operator << (std::ostream& os, const Grayscale& img) {
		for (int i = 0; i < img.rows; ++i) {
			for (int j = 0; j < img.cols; ++j) {
				if (sizeof(T) == 1) {
					os << static_cast<int>(img(i, j)) << " ";
				}
				else {
					os << img(i, j) << " ";
				}
			}
			os << "\n";
		}
		return os;
	}
};

template <typename T>
const double Grayscale<T>::epsilon = 1e-6;

// задача: Нарисуйте прямую с координатами начала x1,y1	
// и координатами конца x2,y2 (значение для заполнения может задаваться пользователем)

template <typename T>
void make_line(Grayscale<T>& img, int x1, int y1, int x2, int y2, T color) {
	int r = img.get_rows();
	int c = img.get_cols();

	int dx = std::abs(x2 - x1);
	int dy = std::abs(y2 - y1);

	int x_way;
	if (x1 < x2) x_way = 1;
	else { x_way = -1; }

	int y_way;
	if (y1 < y2) y_way = 1;
	else { y_way = -1; }

	int err = dx - dy;

	while (true) {
		if (y1 >= 0 && y1 < r && x1 >= 0 && x1 < c) {
			img(y1, x1) = color;
		}

		if (x1 == x2 && y1 == y2) {
			break;
		}

		int next = 2 * err;
		if (next > -dy) {
			err -= dy;
			x1 += x_way;
		}
		if (next < dx) {
			err += dx;
			y1 += y_way;
		}
	}
}
