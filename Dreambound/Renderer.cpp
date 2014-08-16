#include "Renderer.hpp"

// Basic Renderer constructor
Renderer::Renderer() {

}

// Renderer constructor
Renderer::Renderer(sf::RenderWindow* canvas) {
	this->canvas = canvas;
	if (init() == true) {
		// dunno, is init even necessary for this thing? I guess it will be, but it isn't yet.
	} else {
		// moeitemeter = infinity;
	}
}

bool Renderer::init() {
	return true;
}

void Renderer::render() {
	for (std::list<Object*>::iterator iterator = renderList.begin(), end = renderList.end(); iterator != end; ++iterator) {
		(*iterator)->render();
	}
}

void Renderer::cleanup() {

}

void Renderer::registerRender(Object* object) {
	renderList.push_back(object);
}

void Renderer::unregisterRender(Object* object) {
	renderList.remove(object);
}