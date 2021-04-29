#include "stdbool.h"
#include "math.h"
#include "stdio.h"
#include "geometry.h"

//GENERAL MATH FUNCTIONS
float normalize(float x){
	return x / fabs(x);
}

int ___x = 1;
int ___a = 65521;
int ___c = 0;
int ___m = 17364;

float getRandom(){

	___x = (___a * ___x + ___c) % ___m;
	float r = (float)___x;
	r /= ___m;

	return r;
	
}

//VEC2F FUNCTIONS

Vec2f getVec2f(float x, float y){
	Vec2f v = { x, y };
	return v;
}

void Vec2f_log(Vec2f v){
	printf("{ x: %f, y: %f }\n", v.x, v.y);
}

void Vec2f_add(Vec2f *v1, Vec2f v2){
	v1->x += v2.x;
	v1->y += v2.y;
}

void Vec2f_sub(Vec2f *v1, Vec2f v2){
	v1->x -= v2.x;
	v1->y -= v2.y;
}

void Vec2f_mul(Vec2f *v1, Vec2f v2){
	v1->x *= v2.x;
	v1->y *= v2.y;
}

void Vec2f_div(Vec2f *v1, Vec2f v2){
	v1->x /= v2.x;
	v1->y /= v2.y;
}

void Vec2f_mulByFloat(Vec2f *v1, float a){
	v1->x *= a;
	v1->y *= a;
}

void Vec2f_divByFloat(Vec2f *v1, float a){
	v1->x /= a;
	v1->y /= a;
}

void Vec2f_normalize(Vec2f *v){
	Vec2f_divByFloat(v, getMagVec2f(*v));
}

float getMagVec2f(Vec2f v){
	return sqrt(v.x * v.x + v.y * v.y);
}

Vec2f getAddVec2f(Vec2f v1, Vec2f v2){
	return getVec2f(v1.x + v2.x, v1.y + v2.y);
}

Vec2f getSubVec2f(Vec2f v1, Vec2f v2){
	return getVec2f(v1.x - v2.x, v1.y - v2.y);
}

Vec2f getMulVec2fFloat(Vec2f v, float a){
	return getVec2f(v.x * a, v.y * a);
}

Vec2f getDivVec2fFloat(Vec2f v, float a){
	return getVec2f(v.x / a, v.y / a);
}

float getDistanceVec2f(Vec2f v1, Vec2f v2){
	return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

Vec2f getInverseVec2f(Vec2f v){
	return getVec2f(1 / v.x, 1 / v.y);
}

//VEC3F FUNCTIONS
Vec3f getVec3f(float x, float y, float z){
	Vec3f v = { x, y, z };
	return v;
}

void Vec3f_log(Vec3f v){
	printf("{ x: %f, y: %f, z: %f }\n", v.x, v.y, v.z);
}

void Vec3f_add(Vec3f *v1, Vec3f v2){
	v1->x += v2.x;
	v1->y += v2.y;
	v1->z += v2.z;
}

void Vec3f_sub(Vec3f *v1, Vec3f v2){
	v1->x -= v2.x;
	v1->y -= v2.y;
	v1->z -= v2.z;
}

void Vec3f_mulByFloat(Vec3f *v, float f){
	v->x *= f;
	v->y *= f;
	v->z *= f;
}

void Vec3f_divByFloat(Vec3f *v, float x){
	v->x /= x;
	v->y /= x;
	v->z /= x;
}

void Vec3f_normalize(Vec3f *v){
	Vec3f_divByFloat(v, getMagVec3f(*v));
}

void Vec3f_rotate(Vec3f *point, float rotX, float rotY, float rotZ){

	//x axis
	float mag = sqrt(point->y * point->y + point->z * point->z);

	float deg = atan2(point->z, point->y);

	deg += rotX;

	point->y = mag * cos(deg);
	point->z = mag * sin(deg);

	//y axis
	mag = sqrt(point->x * point->x + point->z * point->z);

	deg = atan2(point->x, point->z);

	deg += rotY;

	point->x = mag * sin(deg);
	point->z = mag * cos(deg);

	//z axis
	mag = sqrt(point->x * point->x + point->y * point->y);

	deg = atan2(point->y, point->x);

	deg += rotZ;

	point->x = mag * cos(deg);
	point->y = mag * sin(deg);

}

float getMagVec3f(Vec3f v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3f getAddVec3f(Vec3f v1, Vec3f v2){
	return getVec3f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3f getSubVec3f(Vec3f v1, Vec3f v2){
	return getVec3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vec3f getMulVec3fFloat(Vec3f v, float a){
	return getVec3f(v.x * a, v.y * a, v.z * a);
}

Vec3f getDivVec3fFloat(Vec3f v, float a){
	return getVec3f(v.x / a, v.y / a, v.z / a);
}

Vec3f getCrossVec3f(Vec3f v1, Vec3f v2){
	Vec3f v = {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x,
	};

	return v;
}

float getAngleBetweenVec3f(Vec3f v1, Vec3f v2){

	float a = getMagVec3f(getSubVec3f(v1, v2));
	float b = getMagVec3f(v1);
	float c = getMagVec3f(v2);

	return acos((a * a - b * b - c * c) / -2 / b / c);

}
