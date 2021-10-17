#include "Overlay.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;

Overlay::Overlay()
{

}

void Overlay::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Component::draw();
}
