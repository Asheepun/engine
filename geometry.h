#ifndef GEOMETRY_H_
#define GEOMETRY_H_

typedef struct Vec2f{
	float x;
	float y;
}Vec2f;

typedef struct Vec3f{
	float x;
	float y;
	float z;
}Vec3f;

typedef struct Vec4f{
	float x;
	float y;
	float z;
	float w;
}Vec4f;

typedef struct Mat4f{
	float values[16];
}Mat4f;

int Number_normalize(float);

Vec2f getVec2f(float, float);

void Vec2f_set(Vec2f *, float, float);

void Vec2f_add(Vec2f *, Vec2f *);

void Vec2f_sub(Vec2f *, Vec2f *);

void Vec2f_mul(Vec2f *, Vec2f *);

void Vec2f_mulByFactor(Vec2f *, float);

void Vec2f_div(Vec2f *, Vec2f *);

void Vec2f_divByFactor(Vec2f *, float);

float Vec2f_getMag(Vec2f);

Vec2f Vec2f_getInverse(Vec2f);

void Vec2f_normalize(Vec2f *);

void Vec2f_log(Vec2f);

void Vec3f_set(Vec3f *, float, float, float);

void Vec3f_add(Vec3f *, Vec3f *);

void Vec3f_sub(Vec3f *, Vec3f *);

void Vec3f_mul(Vec3f *, float);

void Vec3f_div(Vec3f *, float);

void Vec3f_normalize(Vec3f *);

void Vec3f_log(Vec3f);

Vec3f Vec3f_getSub(Vec3f, Vec3f);

Vec3f Vec3f_getMul(Vec3f, float);

float Vec3f_getMag(Vec3f);

float Vec3f_getDot(Vec3f, Vec3f);

Vec3f Vec3f_getCross(Vec3f, Vec3f);

Vec4f Vec3f_getVec4f(Vec3f);

void Vec3f_rotate(Vec3f *, float, float, float);

Vec4f getVec4f(float, float, float, float);

void Vec4f_set(Vec4f *, float, float, float, float);

void Vec4f_mulByMat4f(Vec4f *, Mat4f *);

float Vec4f_getDot(Vec4f, Vec4f);

void Vec4f_dot(Vec4f);

void Vec4f_rotate(Vec4f *, float, float, float);

void Vec4f_log(Vec4f);

void Mat4f_mul(Mat4f *, Mat4f *);

void Mat4f_rotate(Mat4f *, float, float, float);

void Mat4f_scale(Mat4f *, float, float, float);

void Mat4f_translate(Mat4f *, float, float, float);

void Mat4f_perspective(Mat4f *m, float fov, float aspectRatio, float n, float f);

float getRandom();

#endif
