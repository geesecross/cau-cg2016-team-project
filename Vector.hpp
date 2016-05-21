#pragma once
#include <iostream>
#include <array>

template<typename T, size_t N>
class _Vector {
private:
	T e[N];

public:
	typedef _Vector<T, N> ThisType;
	typedef T Type;

	_Vector(const T k = 0) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] = k;
		}
	}

	_Vector(const T(&arr)[N]) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] = arr[i];
		}
	}

	T & operator [](const size_t i) {
		return e[i];
	}

	const T & operator [](const size_t i) const {
		return e[i];
	}

	ThisType & operator =(const ThisType & rhs) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] = rhs.e[i];
		}
		return *this;
	}

	ThisType & operator =(const T(&arr)[N]) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] = arr[i];
		}
		return *this;
	}

	ThisType & operator +=(const ThisType & rhs) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] += rhs.e[i];
		}
		return *this;
	}

	ThisType & operator -=(const ThisType & rhs) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] -= rhs.e[i];
		}
		return *this;
	}

	ThisType & operator *=(const T k) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] *= k;
		}

		return *this;
	}

	const ThisType operator +(const ThisType & rhs) const {
		ThisType result = *this;
		result += rhs;
		return result;
	}

	const ThisType operator -(const ThisType & rhs) const {
		ThisType result = *this;
		result -= rhs;
		return result;
	}

	const ThisType operator *(const T k) const {
		ThisType result = *this;
		result *= k;
		return result;
	}

	const T dot(const ThisType & rhs) const {
		T result = 0;
		for (size_t i = 0; i < N; ++i) {
			result += this->e[i] * rhs.e[i];
		}
		return result;
	}

	const T length() const {
		T result = 0;
		for (size_t i = 0; i < N; ++i) {
			result += this->e[i] * this->e[i];
		}
		return (T)sqrt((double)result);
	}

	const ThisType normalized() const {
		return *this * (1 / this->length());
	}

	const T * data() const {
		return &this->e[0];
	}

	T * data() {
		return &this->e[0];
	}

	friend std::ostream & operator <<(std::ostream & os, const ThisType & vector) {
		os << "(";
		for (size_t i = 0; i < N - 1; ++i) {
			os << vector.e[i] << ", ";
		}
		os << vector.e[N - 1] << ")";
		return os;
	}
};

template<typename T>
class Vector3 : public _Vector<T, 3> {
public:
	Vector3(const T k = 0) {
		*this = ThisType(k);
	}

	Vector3(const T(&arr)[3]) {
		*this = ThisType(arr);
	}

	Vector3(const T x, const T y, const T z) {
		*this = ThisType({ x, y, z });
	}

	Vector3(const ThisType & vector) {
		*static_cast<ThisType *>(this) = vector;
	}

	const ThisType cross(const ThisType & rhs) {
		return ThisType({ (*this)[1] * rhs[2] - (*this)[2] * rhs[1], (*this)[2] * rhs[0] - (*this)[0] * rhs[2], (*this)[0] * rhs[1] - (*this)[1] * rhs[0] });
	}

	operator ThisType() const {
		return *this;
	}

	const _Vector<T, 4> toHomogeneous() const {
		return Vector<T, 4>({ (*this)[0], (*this)[1], (*this)[2], 1 });
	}
};

template<typename T>
class Vector4 : public _Vector<T, 4> {
public:
	Vector4(const T k = 0) {
		*this = ThisType(k);
	}

	Vector4(const T(&arr)[4]) {
		*this = ThisType(arr);
	}

	Vector4(const T x, const T y, const T z, const T w) {
		*this = ThisType({ x, y, z, w });
	}

	Vector4(const ThisType & vector) {
		*static_cast<ThisType *>(this) = vector;
	}

	Vector4(const _Vector<T, 3> vector) {
		*this = ThisType({ vector[0], vector[1], vector[2] });
	}

	operator ThisType() const {
		return *this;
	}

	const _Vector<T, 3> fromHomogeneous() const {
		return _Vector<T, 3>({ (*this)[0] / (*this)[3], (*this)[1] / (*this)[3], (*this)[2] / (*this)[3] });
	}
};

typedef Vector3<GLfloat> Vector3f;
typedef Vector3<GLdouble> Vector3d;

typedef Vector4<GLfloat> Vector4f;
typedef Vector4<GLdouble> Vector4d;