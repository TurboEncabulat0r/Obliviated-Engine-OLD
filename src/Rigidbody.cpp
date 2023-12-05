#include "engine.h"
#include "component.h"
#include "gameObject.h"
#include "Rigidbody.h"

namespace engine {


	Rigidbody::Rigidbody(GameObject* gameObject) : Component(gameObject, "Rigidbody") {
		this->serializeValue("velx", &velx);
		this->serializeValue("vely", &vely);
		this->serializeValue("grounded", &grounded);
		x = &this->gameObject->x;
		y = &this->gameObject->y;
	}

	void Rigidbody::addVelocity(float x, float y) {
		velx += x;
		vely += y;
	}

	void applyFriction(float& velocity, float friction)
	{
		if (velocity > 0.0f)
		{
			velocity -= friction * velocity;
			if (velocity < 0.0f)
			{
				velocity = 0.0f;
			}
		}
		else if (velocity < 0.0f)
		{
			velocity += friction * (-velocity);
			if (velocity > 0.0f)
			{
				velocity = 0.0f;
			}
		}
	}

	float clamp(float value, float min, float max) {
		if (value < min) {
			return min;
		}
		else if (value > max) {
			return max;
		}
		else {
			return value;
		}
	}
	

	void Rigidbody::update() {
		this->vely -= 2.81;

		*x += velx * getDeltaTime();
		*y += vely * getDeltaTime();

		

		velx = clamp(velx, -200, 200);

		applyFriction(velx, 0.1f);

		if (*y < -50) {
			*y = -50;
			vely = 0;
			grounded = true;
		}
		else {
			grounded = false;
		}
	}

}