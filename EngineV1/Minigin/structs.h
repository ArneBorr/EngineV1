#pragma once

struct Vector2f
{
public:
	Vector2f() : x{ 0 }, y{ 0 }{};
	Vector2f(float _x, float _y) : x{ _x }, y{ _y }{};
	float x, y;
};

struct Vector3f
{
public:
	Vector3f(float _x, float _y, float _z) : x{ _x }, y{ _y }, z{ _z } {};
	float x, y, z;
};

struct Vector4f
{
public:
	float x, y, z, w;
};

template <class T>
struct Matrix3
{
	Matrix3<T>() {};
	Matrix3<T>(T _00, T _01, T _02, T _10, T _11, T _12, T _20, T _21, T _22)
	{
		values[0][0] = _00; values[0][1] = _01; values[0][2] = _02;
		values[1][0] = _10; values[1][1] = _11; values[1][2] = _12;
		values[2][0] = _20; values[2][1] = _21, values[2][2] = _22;
	}	

	T values[3][3];

	inline Matrix3<T> operator*(const Matrix3<T>& rh)
	{
		auto rhValues = rh.values;
		return Matrix3<T>(
			values[0][0] * rhValues[0][0] + rhValues[0][1] * rhValues[1][0] + values[0][2] * rhValues[2][0],
			values[0][0] * rhValues[0][1] + rhValues[0][1] * rhValues[1][1] + values[0][2] * rhValues[2][1],
			values[0][0] * rhValues[0][2] + rhValues[0][1] * rhValues[1][2] + values[0][2] * rhValues[2][2],
				 	   			  			  				  								 
			values[1][0] * rhValues[0][0] + rhValues[1][1] * rhValues[1][0] + values[1][2] * rhValues[2][0],
			values[1][0] * rhValues[0][1] + rhValues[1][1] * rhValues[1][1] + values[1][2] * rhValues[2][1],
			values[1][0] * rhValues[0][2] + rhValues[1][1] * rhValues[1][2] + values[1][2] * rhValues[2][2],
				  	   			  			 				 								
			values[2][0] * rhValues[0][0] + rhValues[2][1] * rhValues[1][0] + values[2][2] * rhValues[2][0],
			values[2][0] * rhValues[0][1] + rhValues[2][1] * rhValues[1][1] + values[2][2] * rhValues[2][1],
			values[2][0] * rhValues[0][2] + rhValues[2][1] * rhValues[1][2] + values[2][2] * rhValues[2][2]);
	}
};