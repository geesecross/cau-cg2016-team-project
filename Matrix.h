#pragma once
#include "Vector.h"

template<typename ElementT, size_t N>
class Matrix {
protected:
	using ThisType = Matrix<ElementT, N>;

public:
	typedef Vector<ElementT, N> ColumnType;
	typedef typename ColumnType::ElementType ElementType;
	static const size_t columnSize = N;

private:
	ColumnType column[N];

public:
	static const ThisType zeroMatrix() {
		static ThisType mat(0);
		return mat;
	}

	static const ThisType identity() {
		static ThisType mat(1);
		return mat;
	}

	Matrix(const ElementType k = 1) {
		for (size_t i = 0; i < N; ++i) {
			this->column[i][i] = k;
		}
	}

	Matrix(const ColumnType(&arr)[N]) {
		for (size_t i = 0; i < N; ++i) {
			this->column[i] = arr[i];
		}
	}

	Matrix(const ElementType(&arr)[N * N]) {
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

	ThisType & operator *=(const ElementType k) {
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

	const ThisType transposed() const {
		ThisType result
		for (size_t x = 0; x < N; ++x) {
			for (size_t y = 0; y < N; ++y) {
				result[x][y] = (*this)[y][x];
			}
		}
		return result;
	}

	const ElementType * data() const {
		return this->column[0].data();
	}

	ElementType * data() {
		return this->column[0].data();
	}

	friend std::ostream & operator <<(std::ostream & os, const ThisType & matrix) {
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

using Matrix4f = Matrix<GLfloat, 4>;
using Matrix4d = Matrix<GLdouble, 4>;