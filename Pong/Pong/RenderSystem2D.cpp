#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "RenderSystem2D.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "ResourceManager.h"

#include <iostream>

extern ECSCoordinator ECSCoord;

void RenderSystem2D::Init(){
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    unsigned int VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderSystem2D::Update() {
    
    for (auto entity : Entities) {
        auto rigidBody = ECSCoord.GetComponent<C_RigidBody2D>(entity);
        if (rigidBody->toRender) {
            Shader shader = ResourceManager::GetShader(rigidBody->ShaderName);
            shader.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(rigidBody->Position, 0.f));

            model = glm::translate(model, glm::vec3(0.5f * rigidBody->Size.x, 0.5f * rigidBody->Size.y, 0.f));
            model = glm::rotate(model, glm::radians(rigidBody->Rotation), glm::vec3(0.f, 0.f, 1.f));
            model = glm::translate(model, glm::vec3(-0.5f * rigidBody->Size.x, -0.5f * rigidBody->Size.y, 0.f));

            model = glm::scale(model, glm::vec3(rigidBody->Size, 1.f));
            //std::cout << "model stuff: " << glm::to_string(model) << std::endl;
            shader.setUniform<glm::mat4>("model", model);
            shader.setUniform<glm::vec3>("spriteColor", glm::vec3(1.f, 1.f, 1.f));

            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
}