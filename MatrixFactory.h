#pragma once
#include "Matrix.h"

namespace MatrixFactory {
	enum Axis {
		X,
		Y,
		Z,
	};

	template<typename T> Matrix<T, 4> translation(const Vector<T, 3> & xyz) {
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

	template<typename T> Matrix<T, 4> scale(const Vector<T, 3> & xyz) {
		Matrix<T, 4> mat;
		mat[0][0] = xyz[0];
		mat[1][1] = xyz[1];
		mat[2][2] = xyz[2];

		return mat;
	}

	template<typename T> Matrix<T, 4> rotation(const T degree, const Axis axis) {
		T cosValue;
		T sinValue;

		// 특수근 처리 (정확도 향상용)
		if (90 == degree) {
			cosValue = 0;
			sinValue = 1;
		}
		else if (-90 == degree) {
			cosValue = 0;
			sinValue = -1;
		}
		else {
			const double radian = (degree * M_PI) / 180.0;
			cosValue = (T)cos(radian);
			sinValue = (T)sin(radian);
		}

		Matrix<T, 4> mat;
		switch (axis) {
		case X:
			mat[1][1] = cosValue;
			mat[2][1] = -sinValue;
			mat[1][2] = sinValue;
			mat[2][2] = cosValue;
			break;

		case Y:
			mat[0][0] = cosValue;
			mat[2][0] = sinValue;
			mat[0][2] = -sinValue;
			mat[2][2] = cosValue;
			break;

		case Z:
			mat[0][0] = cosValue;
			mat[1][0] = -sinValue;
			mat[0][1] = sinValue;
			mat[1][1] = cosValue;
			break;
		}

		return mat;
	}
}