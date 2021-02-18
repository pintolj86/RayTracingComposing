#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
public:
	Ray() {}
	Ray(const Point3D& origin, const Vec3& direction):origin(origin), direction(direction){}

	Point3D getOrigin() const { return origin; }
	Vec3 getDirection() const { return direction; }

	Point3D at(double t) const
	{
		return origin + t * direction;
	}

public:
	Point3D origin;
	Vec3 direction;
};
#endif
