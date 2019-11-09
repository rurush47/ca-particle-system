#include "Sphere.h"

void Sphere::render()
{
	DrawSphere(Geometry::glmToRaylibVec3(position), radius, BLUE);
}
