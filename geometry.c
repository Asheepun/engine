#include "stdbool.h"
#include "math.h"
#include "stdio.h"
#include "geometry.h"

int Number_normalize(float x){
	if(x > 0){
		return 1;
	}
	if(x < 0){
		return -1;
	}
	return 0;
}

Vec2f getVec2f(float x, float y){
	Vec2f v = { x, y };
	return v;
}

void Vec2f_set(Vec2f *v, float x, float y){
	v->x = x;
	v->y = y;
}

void Vec2f_add(Vec2f *v1, Vec2f *v2){
	v1->x += v2->x;
	v1->y += v2->y;
}

void Vec2f_sub(Vec2f *v1, Vec2f *v2){
	v1->x -= v2->x;
	v1->y -= v2->y;
}

void Vec2f_mul(Vec2f *v1, Vec2f *v2){
	v1->x *= v2->x;
	v1->y *= v2->y;
}

void Vec2f_mulByFactor(Vec2f *v1, float a){
	v1->x *= a;
	v1->y *= a;
}

void Vec2f_div(Vec2f *v1, Vec2f *v2){
	v1->x /= v2->x;
	v1->y /= v2->y;
}

void Vec2f_divByFactor(Vec2f *v1, float a){
	v1->x /= a;
	v1->y /= a;
}

float Vec2f_getMag(Vec2f v){
	return sqrt(v.x * v.x + v.y * v.y);
}

float getDistanceBetweenVec2fAndVec2f(Vec2f v1, Vec2f v2){
	return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

Vec2f Vec2f_getInverse(Vec2f v){
	return getVec2f(1 / v.x, 1 / v.y);
}

void Vec2f_normalize(Vec2f *v){
	Vec2f_divByFactor(v, Vec2f_getMag(*v));
}

void Vec2f_log(Vec2f v){
	printf("{ x: %f, y: %f }\n", v.x, v.y);
}

void Vec3f_set(Vec3f *v, float x, float y, float z){
	v->x = x;
	v->y = y;
	v->z = z;
}

void Vec3f_add(Vec3f *v1, Vec3f *v2){
	v1->x += v2->x;
	v1->y += v2->y;
	v1->z += v2->z;
}

void Vec3f_sub(Vec3f *v1, Vec3f *v2){
	v1->x -= v2->x;
	v1->y -= v2->y;
	v1->z -= v2->z;
}

void Vec3f_mul(Vec3f *v, float f){
	v->x *= f;
	v->y *= f;
	v->z *= f;
}

void Vec3f_div(Vec3f *v, float x){
	v->x /= x;
	v->y /= x;
	v->z /= x;
}

void Vec3f_normalize(Vec3f *v){
	Vec3f_div(v, Vec3f_getMag(*v));
}

void Vec3f_log(Vec3f v){
	printf("{ x: %f, y: %f, z: %f }\n", v.x, v.y, v.z);
}

Vec3f Vec3f_getSub(Vec3f v1, Vec3f v2){
	Vec3f result = v1;
	Vec3f_sub(&result, &v2);
	return result;
}

Vec3f Vec3f_getMul(Vec3f v, float x){
	Vec3f result = v;
	Vec3f_mul(&result, x);
	return result;
}

float Vec3f_getMag(Vec3f v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Vec3f_getDot(Vec3f v1, Vec3f v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3f Vec3f_getCross(Vec3f v1, Vec3f v2){
	Vec3f result = {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x,
	};

	return result;
}

Vec4f getVec4f(float x, float y, float z, float w){
	Vec4f v = { x, y, z, w };
	return v;
}

Vec4f Vec3f_getVec4f(Vec3f v){
	Vec4f newVector = { v.x, v.y, v.z, 1 };

	return newVector;
}

void Vec3f_rotate(Vec3f *v, float x, float y, float z){
	Vec4f newVector = Vec3f_getVec4f(*v);

	Vec4f_log(newVector);

	Vec4f_rotate(&newVector, x, y, z);

	//printf("%f\n", z);

	v->x = newVector.x;
	v->y = newVector.y;
	v->z = newVector.z;

}

void Vec4f_set(Vec4f *v, float x, float y, float z, float w){
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

float Vec4f_getDot(Vec4f v1, Vec4f v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

void Vec4f_mulByMat4f(Vec4f *v, Mat4f *m){
	float newVector[4] = { 0, 0, 0, 0 };

	for(int y = 0; y < 4; y++){
		newVector[y] += v->x * m->values[y * 4 + 0] +
			v->y * m->values[y * 4 + 1] +
			v->z * m->values[y * 4 + 2] +
			v->w * m->values[y * 4 + 3];
	}

	v->x = newVector[0];
	v->y = newVector[1];
	v->z = newVector[2];
	v->w = newVector[3];
}

void Vec4f_rotate(Vec4f *v, float x, float y, float z){
	Mat4f rotationMatrixX = {
		1, 0, 0, 0,
		0, cos(x), -sin(x), 0,
		0, sin(x), cos(x), 0,
		0, 0, 0, 1,
	};

	Mat4f rotationMatrixY = {
		cos(y), 0, sin(y), 0,
		0, 1, 0, 0,
		-sin(y), 0, cos(y), 0,
		0, 0, 0, 1,
	};

	Mat4f rotationMatrixZ = {
		cos(z), -sin(z), 0, 0,
		sin(z), cos(z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

	Vec4f_mulByMat4f(v, &rotationMatrixX);

	Vec4f_mulByMat4f(v, &rotationMatrixY);

	Vec4f_mulByMat4f(v, &rotationMatrixZ);
}

void Vec4f_log(Vec4f v){
	printf("x: %f, y: %f, z: %f, w: %f\n", v.x, v.y, v.z, v.w);
}

void Mat4f_mul(Mat4f *m1, Mat4f *m2){
	Mat4f newMatrix;

	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			newMatrix.values[y * 4 + x] = 
				m1->values[y * 4 + 0] * m2->values[0 + x] +
				m1->values[y * 4 + 1] * m2->values[4 + x] +
				m1->values[y * 4 + 2] * m2->values[8 + x] +
				m1->values[y * 4 + 3] * m2->values[12 + x];
		}
	}
	
	for(int i = 0; i < 16; i++){
		m1->values[i] = newMatrix.values[i];
	}
}

void Mat4f_rotate(Mat4f *m, float x, float y, float z){
	Mat4f rotationMatrixX = {
		1, 0, 0, 0,
		0, cos(x), -sin(x), 0,
		0, sin(x), cos(x), 0,
		0, 0, 0, 1,
	};

	Mat4f rotationMatrixY = {
		cos(y), 0, sin(y), 0,
		0, 1, 0, 0,
		-sin(y), 0, cos(y), 0,
		0, 0, 0, 1,
	};

	Mat4f rotationMatrixZ = {
		cos(z), -sin(z), 0, 0,
		sin(z), cos(z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

	Mat4f_mul(m, &rotationMatrixX);
	Mat4f_mul(m, &rotationMatrixY);
	Mat4f_mul(m, &rotationMatrixZ);
	/*
	return mat4(
		c + x*x*(1-c),            x*y*(1-c) - z*s,      x*z*(1-c) + y*s,  0,
		    y*x*(1-c) + z*s,  c + y*y*(1-c),            y*z*(1-c) - x*s,  0,
		    z*x*(1-c) - y*s,      z*y*(1-c) + x*s,  c + z*z*(1-c),        0,
		    0,                        0,                    0,            1
	*/
}

void Mat4f_scale(Mat4f *m, float x, float y, float z){
	Mat4f scaleMatrix = {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1,
	};

	Mat4f_mul(m, &scaleMatrix);
}

void Mat4f_translate(Mat4f *m, float x, float y, float z){
	Mat4f translationMatrix = {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1,
	};

	Mat4f_mul(m,  &translationMatrix);
	/*
	m->values[3] += pos->x;
	m->values[7] += pos->y;
	m->values[11] += pos->z;
	*/
}

void Mat4f_perspective(Mat4f *m, float fov, float aspectRatio, float nearViewDistance, float farViewDistance){

	float f = 1.0f / tanf(fov / 2.0f);
	float ar = aspectRatio;
	float nd = nearViewDistance, fd = farViewDistance;
	
	Mat4f perspectiveMatrix = {
		 f / ar,           0,                0,                0,
		 0,                f,                0,                0,
		 0,                0,               (fd+nd)/(nd-fd),  (2*fd*nd)/(nd-fd),
		 0,                0,               -1,                0
	};

	Mat4f_mul(m, &perspectiveMatrix);

}

/*
int x = 1;
int a = 65521;
int c = 0;
int m = 17364;

float getRandom(){

	x = (a * x + c) % m;
	float r = (float)x;
	r /= m;

	return r;
	
}
*/
