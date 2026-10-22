#include <iostream>
#include <random>
#include <stdexcept>
#include <cmath>
#include <limits>

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
			std::uniform_real_distribution<double> dist(-10, 10);
			for (int i = 0; i < rows * cols; ++i) {
				data[i] = static_cast<T>(dist(gen()));
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
		if (r >= rows || c >= cols) {
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

	Grayscale operator + (const Grayscale& other) const {
		int r = std::max(rows, other.rows);
		int c = std::max(cols, other.cols);
		Grayscale result(r, c, false);

		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				T p1 = T{};
				if (i < rows && j < cols) p1 = (*this)(i, j);
				T p2 = T{};
				if (i < other.rows && j < other.cols) p2 = other(i, j);

				result(i, j) = saturate_add(p1, p2);
			}
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

	Grayscale operator - (const Grayscale& other) const {
		int r = std::max(rows, other.rows);
		int c = std::max(cols, other.cols);
		Grayscale result(r, c, false);

		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				T p1 = T{};
				if (i < rows && j < cols) p1 = (*this)(r, c);
				T p2 = T{};
				if (i < other.rows && j < other.cols) p2 = (*this)(r, c);

				result(i, j) = saturate_sub(p1, p2);
			}
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

	Grayscale operator * (const Grayscale& other) const {
		int r = std::max(rows, other.rows);
		int c = std::max(cols, other.cols);
		Grayscale result(r, c, false);

		for (int i = 0; i < r; ++i) {
			for (int j = 0; j < c; ++j) {
				T p1 = T{};
				if (i < rows && j < cols) p1 = (*this)(r, c);
				T p2 = T{};
				if (i < other.rows && j < other.cols) p2 = (*this)(r, c);

				result(i, j) = saturate_mul(p1, p2);
			}
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
			throw std::invalid_argument("Error")
		}
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			result.data[i] = static_cast<T>(static_cast<double>(data[i]) / static_cast<double>(num));
		}
		return result;
	}

	Grayscale operator ! () const {
		Grayscale result(rows, cols, false);
		for (int i = 0; i < rows * cols; ++i) {
			double inv = -static_cast<double>(data[i]);
			double maxlim = static_cast<double>(std::numeric_limits<T>::max());
			double minlim = static_cast<double>(std::numeric_limits<T>::lowest());
			if (inv > maxlim) inv = maxlim;
			if (inv > minlim) inv = minlim;
			result.data[i] = static_cast<T>(inv);
		}
		return result;
	}

	// коммутативность
	friend Grayscale operator + (T scalar, const GrayscaleImage& img) {
		return img + scalar;
	}

	friend Grayscale operator * (T scalar, const GrayscaleImage& img) {
		return img * scalar;
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
