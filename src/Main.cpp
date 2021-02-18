#include <iostream>
#include <fstream>
#include <string>
#include "vec3.h"
#include "ray.h"
#include "futils.h"

#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define MAX_COLOR 255
#define P3 "P3"
#define MP 255.999
#define SB 0.25
#define EXPORT_IMAGE_PATH "tracer.ppm"

/*RayTrace a Simple Sphere and prepare normals 
	Equation: (P-C).(P-C) = r * r
*/

double isHitSphere(const Point3D& center, double radius, const Ray& r) {
	Vec3 oc = r.getOrigin() - center;
	double a = r.getDirection().length_squared();
	double half_b = dot(oc, r.getDirection());
	double c = oc.length_squared() - radius * radius;
	double discriminant = half_b * half_b - a * c;

	//Quadratic Equation
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-half_b - sqrt(discriminant)) / a;
	}
}

/*Blend the color using LERP*/

Color rayColor(const Ray& r) {
	double t = isHitSphere(Point3D(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		Vec3 N = unit_vector(r.at(t) - Vec3(0, 0, -1));
		return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	Vec3 unit_direction = unit_vector(r.getDirection());
	t = 0.5 * (unit_direction.y() + 1.0);

	//Blending or LERP
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

//Image settings
double getAspectRatio()  { return 16.0 / 9.0; }
int getImageWidth() { return 400; }
int getImageHeight() { return static_cast<int>(getImageWidth() / getAspectRatio()); }

//Camera Settings
double getViewportHeight() { return 2.0; }
double getViewportWidth() { return getAspectRatio() * getViewportHeight(); }
double getFocalLength() { return 1.0; }
Point3D getOrigin() { return Point3D(0.0, 0.0, 0.0); }
Vec3 getHorizontal() { return Vec3(getViewportWidth(), 0, 0); }
Vec3 getVertical() { return Vec3(0, getViewportHeight(), 0); }
Vec3 getLowerLeftCorner() { return getOrigin() - getHorizontal() / 2 - getVertical() / 2 - Vec3(0.0, 0.0, getFocalLength());  }

//Render to an Image ppm file
void renderToImage(FUtils &fWriter)
{
	for (int j = IMAGE_HEIGHT - 1; j >= 0; j--)
	{
		std::cerr << "\r Lines remaining: " << j << std::flush;
		for (int i = 0; i < IMAGE_WIDTH; i++)
		{
			double u = double(i) / (IMAGE_WIDTH - 1);
			double v = double(j) / (IMAGE_HEIGHT - 1);
			Ray r(getOrigin(), getLowerLeftCorner() + u * getHorizontal() + v * getVertical() - getOrigin());
			fWriter.writeColor(rayColor(r));
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
int main()
{
	FUtils fWriter;
	fWriter.init(EXPORT_IMAGE_PATH, MP);
	fWriter.writeToFile(P3);
	fWriter.writeToFile(std::to_string(IMAGE_WIDTH) + ' ' + std::to_string(IMAGE_HEIGHT));
	fWriter.writeToFile(std::to_string(MAX_COLOR));

	//Render
	renderToImage(fWriter);

	//cleanup file exporter
	fWriter.close();
	return EXIT_SUCCESS;
}