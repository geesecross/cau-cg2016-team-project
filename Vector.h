#pragma once
#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <array>

template<typename VectorT, typename ElementT, size_t N>
class VectorSpecific {
	/* empty ;) */
};

template<typename ElementT, size_t N>
class VectorBase : public VectorSpecific<VectorBase<ElementT, N>, ElementT, N> {
public:
	using ElementType = ElementT;
protected:
	using ThisType = VectorBase<ElementT, N>;

private:
	ElementT e[N];

public:
	static const size_t size = N;
	
	static const ThisType & zeroVector() {
		static ThisType vector(0);
		return vector;
	}
	
	static const ThisType & oneVector() {
		static ThisType vector(1);
		return vector;
	}

	VectorBase(const ElementT k = 0) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] = k;
		}
	}

	VectorBase(const ElementT(&arr)[N]) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] = arr[i];
		}
	}

	VectorBase(const VectorBase<ElementT, N - 1> & vector, const ElementT k) {
		for (size_t i = 0; i < N - 1; ++i) {
			this->e[i] = vector[i];
		}
		this->e[N - 1] = k;
	}

	ElementT & operator [](const size_t i) {
		return e[i];
	}

	const ElementT & operator [](const size_t i) const {
		return e[i];
	}

	ThisType & operator =(const ThisType & rhs) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] = rhs.e[i];
		}
		return *this;
	}

	ThisType & operator =(const ElementT(&arr)[N]) {
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

	ThisType & operator *=(const ElementT k) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] *= k;
		}

		return *this;
	}

	// elementwise multiplication
	ThisType & operator *=(const ThisType & rhs) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] *= rhs.e[i];
		}

		return *this;
	}

	ThisType & operator /=(const ElementT k) {
		for (size_t i = 0; i < N; ++i) {
			this->e[i] /= k;
		}

		return *this;
	}

	const ThisType operator -() const {
		ThisType result = *this;
		result *= -1;
		return result;
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

	const ThisType operator *(const ElementT k) const {
		ThisType result = *this;
		result *= k;
		return result;
	}

	// elementwise multiplication
	const ThisType & operator *(const ThisType & rhs) const {
		ThisType result = *this;
		result *= rhs;
		return result;
	}

	const ThisType operator /(const ElementT k) const {
		ThisType result = *this;
		result /= k;
		return result;
	}

	const ElementT dot(const ThisType & rhs) const {
		ElementT result = 0;
		for (size_t i = 0; i < N; ++i) {
			result += this->e[i] * rhs.e[i];
		}
		return result;
	}

	bool operator ==(const ThisType & rhs) const {
		if (
			this->e[0] != rhs.e[0]
			|| this->e[1] != rhs.e[1]
			|| this->e[2] != rhs.e[2]
			) return false;
		else return true;
	}

	bool operator != (const ThisType & rhs) const {
		return !((*this) == rhs);
	}

	const ElementT length() const {
		ElementT result = 0;
		for (size_t i = 0; i < N; ++i) {
			result += this->e[i] * this->e[i];
		}
		return (ElementT)sqrt((double)result);
	}

	const ThisType normalized() const {
		return *this / this->length();
	}

	const ThisType absolute() const {
		ThisType result;
		for (size_t i = 0; i < N; ++i) {
			result.e[i] = this->e[i] * (this->e[i] >= 0 ? 1 : -1);
		}

		return result;
	}

	const ElementT * data() const {
		return &this->e[0];
	}

	ElementT * data() {
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

template<typename VectorT, typename ElementT>
class VectorSpecific<VectorT, ElementT, 3> {
public:
	static const VectorT & xVector() {
		static VectorT vector({ 1, 0, 0 });
		return vector;
	}
	static const VectorT & yVector() {
		static VectorT vector({ 0, 1, 0 });
		return vector;
	}
	static const VectorT & zVector() {
		static VectorT vector({ 0, 0, 1 });
		return vector;
	}

	const VectorT cross(const VectorT & rhs) const {
		const VectorT & lhs = *static_cast<const VectorT *>(this);
		return VectorT({ lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2], lhs[0] * rhs[1] - lhs[1] * rhs[0] });
	}
};

template<typename VectorT, typename ElementT>
class VectorSpecific<VectorT, ElementT, 4> {
public:
	const VectorBase<ElementT, 3> xyz() const {
		const VectorT & me = *static_cast<const VectorT *>(this);
		return VectorBase<ElementT, 3>({ me[0], me[1], me[2] });
	}
};

template<typename ElementT, size_t N>
class Vector : public VectorBase<ElementT, N> {
public:
	Vector(const ThisType & vector) {
		*static_cast<ThisType *>(this) = vector;
	}

	Vector(const ElementT k = 0) {
		*static_cast<ThisType *>(this) = ThisType(k);
	}

	Vector(const ElementT(&arr)[N]) {
		*static_cast<ThisType *>(this) = ThisType(arr);
	}

	Vector(const Vector<ElementT, size - 1> & vector, const ElementT k) {
		*static_cast<ThisType *>(this) = ThisType(vector, k);
	}

	operator ThisType() const {
		return *this;
	}
};

template<typename ElementT>
class Vector<ElementT, 3> : public VectorBase<ElementT, 3> {
public:
	Vector(const ThisType & vector) {
		*static_cast<ThisType *>(this) = vector;
	}

	Vector(const ElementT k = 0) {
		*static_cast<ThisType *>(this) = ThisType(k);
	}

	Vector(const ElementT(&arr)[3]) {
		*static_cast<ThisType *>(this) = ThisType(arr);
	}

	Vector(const Vector<ElementT, size - 1> & vector, const ElementT z) {
		*static_cast<ThisType *>(this) = ThisType(vector, z);
	}

	Vector(const ElementT x, const ElementT y, const ElementT z) {
		*static_cast<ThisType *>(this) = ThisType({ x, y, z });
	}

	operator ThisType() const {
		return *this;
	}
};

template<typename ElementT>
class Vector<ElementT, 4> : public VectorBase<ElementT, 4> {
public:
	Vector(const ThisType & vector) {
		*static_cast<ThisType *>(this) = vector;
	}

	Vector(const ElementT k = 0) {
		*static_cast<ThisType *>(this) = ThisType(k);
	}

	Vector(const ElementT(&arr)[4]) {
		*static_cast<ThisType *>(this) = ThisType(arr);
	}

	Vector(const Vector<ElementT, size - 1> & vector, const ElementT w) {
		*static_cast<ThisType *>(this) = ThisType(vector, w);
	}

	Vector(const ElementT x, const ElementT y, const ElementT z, const ElementT w) {
		*static_cast<ThisType *>(this) = ThisType({ x, y, z, w });
	}

	operator ThisType() const {
		return *this;
	}
};

using Vector2f = Vector<GLfloat, 2>;
using Vector2d = Vector<GLdouble, 2>;
using Vector3f = Vector<GLfloat, 3>;
using Vector3d = Vector<GLdouble, 3>;
using Vector4f = Vector<GLfloat, 4>;
using Vector4d = Vector<GLdouble, 4>;