#include "engine.h"
#include "component.h"
#include "gameObject.h"
#include "Rigidbody.h"

namespace engine {


	Rigidbody::Rigidbody(GameObject* gameObject) : Component(gameObject, "Rigidbody") {
		this->gameObject->serializeValue("velx", &velx);
		this->gameObject->serializeValue("vely", &vely);
		x = &this->gameObject->x;
		y = &this->gameObject->y;
	}

	void Rigidbody::addVelocity(float x, float y) {
		velx += x;
		vely += y;
	}

	void applyFriction(float& velocity, float friction, float deltaTime)
	{
		if (velocity > 0.0f)
		{
			velocity -= friction * velocity * deltaTime;
			if (velocity < 0.0f)
			{
				velocity = 0.0f;
			}
		}
		else if (velocity < 0.0f)
		{
			velocity += friction * (-velocity) * deltaTime;
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
		this->vely += 5 * getDeltaTime();

		*x += velx * getDeltaTime() * 0.6f;
		*y += vely * getDeltaTime() * 0.6f;

		applyFriction(velx, 0.3f, getDeltaTime());

		velx = clamp(velx, -20, 20);
		

		if (*y > 500) {
			*y = 500;
			vely = 0;
		}
	}

}