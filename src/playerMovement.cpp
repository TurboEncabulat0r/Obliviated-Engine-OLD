#include "engine.h"
#include "component.h"


namespace engine {

	class movement : public Component {
		movement(GameObject* gameObject, std::string name) : Component(gameObject, name) {

		}
	};

}