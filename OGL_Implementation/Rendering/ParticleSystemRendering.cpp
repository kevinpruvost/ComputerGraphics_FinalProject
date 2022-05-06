/*****************************************************************//**
 * \file   ParticleSystemRendering.cpp
 * \brief  ParticleSystem Rendering source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 15 2022
 *********************************************************************/
#include "ParticleSystemRendering.hpp"

#include <glm/gtx/string_cast.hpp>

// C++ includes
#include <memory>
#include <stdexcept>

static std::unique_ptr<ParticleSystemRendering> s_particleSystemRendering;

ParticleSystemRendering::ParticleSystemRendering()
{
    glGenVertexArrays(1, &__vao);
    glGenBuffers(1, &__vbo);

    constexpr const GLfloat particle_quad[] = {
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };
    glBindVertexArray(__vao);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, __vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(sizeof(GLfloat) * 3));
    glBindVertexArray(0);
}

ParticleSystemRendering::~ParticleSystemRendering()
{
    glDeleteVertexArrays(1, &__vao);
    glDeleteBuffers(1, &__vbo);
}

void ParticleSystemRendering::Init()
{
    s_particleSystemRendering.reset(new ParticleSystemRendering());
}

const ParticleSystemRendering & ParticleSystemRendering::Get()
{
    if (s_particleSystemRendering) return *s_particleSystemRendering;
    throw std::runtime_error("Trying to get ParticleSystemRendering although it has not been initialized.\n");
}

GLuint ParticleSystemRendering::GetVAO()
{
    return __vao;
}

GLuint ParticleSystemRendering::GetVBO()
{
    return __vbo;
}

GLuint ParticleSystemRendering::GetParticleSystemRenderingVAO()
{
    return s_particleSystemRendering->GetVAO();
}

GLuint ParticleSystemRendering::GetParticleSystemRenderingVBO()
{
    return s_particleSystemRendering->GetVBO();
}

void ParticleSystemRendering::DrawParticleSystem(ParticleSystem_Base * particleSystem)
{
    const auto & particles = particleSystem->GetParticles();

    //glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_DST_ALPHA);
    //glDepthMask(GL_FALSE);

    const glm::mat4 & parentMatrix = particleSystem->GetModelMatrix();

    for (const std::unique_ptr<Particle_Base> & particle : particles)
    {
        if (particleSystem->displayMode & RenderingMode::FacesMode)
        {
            const Texture & texture = particleSystem->texture;
            Shader & shader = particleSystem->shaderParticleSystem;
            shader.Use();

            shader.SetUniformMatrix4f("model", parentMatrix * particle->GetModelMatrix());
            shader.SetUniformInt("_texture", 0);

            glActiveTexture(GL_TEXTURE0);
            if (texture.GetWidth() != 0)
                glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
            else
                glBindTexture(GL_TEXTURE_2D, 0);
            switch (particleSystem->GetParticlePropertiesStyle())
            {
                case Particle_Base::ParticlePropertiesStyle::Color:
                    shader.SetUniformFloat("ParticleColor", particle->color);
                    break;
            }
            glBindVertexArray(s_particleSystemRendering->GetVAO());
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
        if (particleSystem->displayMode & RenderingMode::WireframeMode)
        {
            Shader & shader = particleSystem->shaderWireframe;
            shader.Use();
            shader.SetUniformMatrix4f("model", parentMatrix * particle->GetModelMatrix());
            // use the same color for all points
            shader.SetUniformFloat("ourColor", WireframeColors[0]);

            glBindVertexArray(s_particleSystemRendering->GetVAO());
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(0);
        }
        if (particleSystem->displayMode & RenderingMode::VerticesMode)
        {
            Shader & shader = particleSystem->shaderPoint;
            shader.Use();
            shader.SetUniformMatrix4f("model", parentMatrix * particle->GetModelMatrix());
            // use the same color for all points
            shader.SetUniformFloat("ourColor", WireframeColors[0]);

            glBindVertexArray(s_particleSystemRendering->GetVAO());
            glDrawArrays(GL_POINTS, 0, 6);
            glBindVertexArray(0);
        }
    }

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Blending options
    //glDepthMask(GL_TRUE);
}
