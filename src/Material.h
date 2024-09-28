#pragma once
#include <glm/glm.hpp>

struct Material {
	glm::vec3 ambiant;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};