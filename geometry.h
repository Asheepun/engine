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

//GENERAL MATH FUNCTIONS
float normalize(float);

float getRandom();

//VEC2F FUNCTIONS
Vec2f getVec2f(float, float);

void Vec2f_log(Vec2f);

void Vec2f_add(Vec2f *, Vec2f);

void Vec2f_sub(Vec2f *, Vec2f);

void Vec2f_mul(Vec2f *, Vec2f);

void Vec2f_div(Vec2f *, Vec2f);

void Vec2f_mulByFloat(Vec2f *, float);

void Vec2f_divByFloat(Vec2f *, float);

float getMagVec2f(Vec2f);

Vec2f getAddVec2f(Vec2f, Vec2f);

Vec2f getSubVec2f(Vec2f, Vec2f);

Vec2f getMulVec2fFloat(Vec2f, float);

Vec2f getDivVec2fFloat(Vec2f, float);

float getDistanceVec2f(Vec2f, Vec2f);

Vec2f getInverseVec2f(Vec2f);

//VEC3F FUNCTIONS

Vec3f getVec3f(float, float, float);

void Vec3f_log(Vec3f);

void Vec2f_normalize(Vec2f *);

void Vec3f_set(Vec3f *, float, float, float);

void Vec3f_add(Vec3f *, Vec3f);

void Vec3f_sub(Vec3f *, Vec3f);

void Vec3f_mulByFloat(Vec3f *, float);

void Vec3f_divByFloat(Vec3f *, float);

void Vec3f_normalize(Vec3f *);

void Vec3f_rotate(Vec3f *, float, float, float);

float getMagVec3f(Vec3f);

Vec3f getAddVec3f(Vec3f, Vec3f);

Vec3f getSubVec3f(Vec3f, Vec3f);

Vec3f getMulVec3fFloat(Vec3f, float);

Vec3f getDivVec3fFloat(Vec3f, float);

#endif
