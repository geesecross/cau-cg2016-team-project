#pragma once
#include "Matrix.h"

namespace MatrixFactory {
	enum Axis {
		X,
		Y,
		Z,
	};

	template<typename T> Matrix<T, 4> translation(const Vector<T, 3> xyz) {
		Matrix<T, 4> mat;
		mat[3][0] = xyz[0];
		mat[3][1] = xyz[1];
		mat[3][2] = xyz[2];

		return mat;
	}

	template<typename T> Matrix<T, 4> scale(const T k) {
		Matrix<T, 4> mat;
		mat[0][0] = k;
		mat[1][1] = k;
		mat[2][2] = k;

		return mat;
	}

	template<typename T> Matrix<T, 4> scale(const Vector<T, 3> xyz) {
		Matrix<T, 4> mat;
		mat[0][0] = xyz[0];
		mat[1][1] = xyz[1];
		mat[2][2] = xyz[2];

		return mat;
	}

	template<typename T> Matrix<T, 4> rotation(const T radian, const Axis axis) {
		Matrix<T, 4> mat;
		switch (axis) {
		case X:
			mat[1][1] = (T)cos((double)radian);
			mat[2][1] = (T)-sin((double)radian);
			mat[1][2] = (T)sin((double)radian);
			mat[2][2] = (T)cos((double)radian);
			break;

		case Y:
			mat[0][0] = (T)cos((double)radian);
			mat[2][0] = (T)sin((double)radian);
			mat[0][2] = (T)-sin((double)radian);
			mat[2][2] = (T)cos((double)radian);
			break;

		case Z:
			mat[0][0] = (T)cos((double)radian);
			mat[1][0] = (T)-sin((double)radian);
			mat[0][1] = (T)sin((double)radian);
			mat[1][1] = (T)cos((double)radian);
			break;
		}

		return mat;
	}
}