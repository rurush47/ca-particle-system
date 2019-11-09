#pragma once
#include "Rigidbodies/Rigidbody.h"

class Sphere :
	public Rigidbody
{
public:
	float radius;

	void render() override;
};

