#pragma once
#include "Vector.h"

template<typename T, size_t N>
class _Matrix {
public:
	typedef T Type;
	typedef _Vector<T, N> ColumnType;
	typedef _Matrix<T, N> ThisType;

private:
	ColumnType column[N];

public:
	_Matrix(const T k = 1) {
		for (size_t i = 0; i < N; ++i) {
			this->column[i][i] = k;
		}
	}

	_Matrix(const ColumnType(&arr)[N]) {
		for (size_t i = 0; i < N; ++i) {
			this->column[i] = arr[i];
		}
	}

	_Matrix(const T(&arr)[N * N]) {
		for (size_t x = 0; x < N; ++x) {
			for (size_t y = 0; y < N; ++y) {
				(*this)[x][y] = arr[x * N + y];
			}
		}
	}

	ColumnType & operator [](const size_t i) {
		return this->column[i];
	}

	const ColumnType & operator [](const size_t i) const {
		return this->column[i];
	}

	const ColumnType col(const size_t x) const {
		return this->column[x];
	}

	const ColumnType row(const size_t y) const {
		ColumnType result;
		for (size_t i = 0; i < N; ++i) {
			result[i] = this->column[i][y];
		}
		return result;
	}

	const ColumnType operator *(const ColumnType & rhs) const {
		ColumnType result;
		for (size_t y = 0; y < N; ++y) {
			result[y] = this->row(y).dot(rhs);
		}

		return result;
	}

	ThisType & operator =(const ThisType & rhs) {
		for (size_t x = 0; x < N; ++x) {
			this->column[x] = rhs.column[x];
		}

		return *this;
	}

	ThisType & operator *=(const T k) {
		for (size_t x = 0; x < N; ++x) {
			this->column[x] *= k;
		}

		return *this;
	}

	ThisType & operator *=(const ThisType & rhs) {
		*this = *this * rhs;
		return *this;
	}

	const ThisType operator *(const ThisType & rhs) const {
		ThisType result;
		for (size_t x = 0; x < N; ++x) {
			result[x] = *this * rhs[x];
		}
		return result;
	}

	const T * data() const {
		return this->column[0].data();
	}

	T * data() {
		return this->column[0].data();
	}

	friend std::ostream & operator <<(std::ostream & os, const ThisType & matrix) {
		/*for (size_t y = 0; y < N; ++y) {
			os << matrix.row(y) << std::endl;
		}*/
		os << "[";
		for (size_t y = 0; y < N - 1; ++y) {
			for (size_t x = 0; x < N - 1; ++x) {
				os << matrix[x][y] << ", ";
			}
			os << matrix[N - 1][y] << "; ";
		}
		for (size_t x = 0; x < N - 1; ++x) {
			os << matrix[x][N - 1] << ", ";
		}
		os << matrix[N - 1][N - 1] << "]";
		return os;
	}
};

template<typename T>
class Matrix4 : public _Matrix<T, 4> {
private:
	static const size_t N = 4;

public:
	Matrix4(const T k = 1) {
		*this = ThisType(k);
	}

	Matrix4(const ColumnType(&arr)[N]) {
		*this = ThisType(arr);
	}

	Matrix4(const ColumnType & c0, const ColumnType & c1, const ColumnType & c2, const ColumnType & c3) {
		*this = ThisType({ x, y, z });
	}

	Matrix4(const ThisType & matrix) {
		*static_cast<ThisType *>(this) = matrix;
	}

	Matrix4(const T(&arr)[N * N]) {
		*this = ThisType(arr);
	}
};

typedef Matrix4<GLfloat> Matrix4f;
typedef Matrix4<GLdouble> Matrix4d;