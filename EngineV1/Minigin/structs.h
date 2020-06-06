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

struct Matrix3f
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;
};