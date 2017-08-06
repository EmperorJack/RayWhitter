//
// Created by Playtech on 6/08/2017.
//

#ifndef RAYWHITTER_MATERIAL_HPP
#define RAYWHITTER_MATERIAL_HPP

#include <glm/glm.hpp>

class Material {

public:
    virtual glm::vec3 getBSDF();

};

#endif //RAYWHITTER_MATERIAL_HPP
