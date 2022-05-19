/*****************************************************************//**
 * \file   Rendering.cpp
 * \brief  Rendering source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 04 2022
 *********************************************************************/
#include "Rendering.hpp"

// C++ includes
#include "OGL_Implementation\OpenGL_Timer.hpp"
#include "OGL_Implementation\Camera.hpp"

// Wireframe/Points Color
static constexpr const glm::vec3 color = glm::vec3(0.1f, 0.95f, 0.1f);
static constexpr const glm::vec3 color1 = glm::vec3(1.0f, 0.95f, 0.1f);
static constexpr const glm::vec3 color2 = glm::vec3(0.1f, 0.95f, 1.0f);
static constexpr const glm::vec3 color3 = glm::vec3(0.5f, 0.2f, 0.3f);

static std::unique_ptr<Rendering> s_Rendering(nullptr);

static std::vector<Font> fonts;

std::unordered_map<std::string, std::unique_ptr<Shader>> Rendering::shaders;

// Display Mode
std::array<glm::vec3, 4> WireframeColors = { color, color1, color2, color3 };
RenderingMode DisplayMode = RenderingMode::FacesMode;

Rendering::Rendering()
	: __textVAO{ 0 }
	, __textVBO{ 0 }
	, __cubeVAO{ 0 }
	, __cubeVBO{ 0 }
{
	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &__textVAO);
	glGenBuffers(1, &__textVBO);
	glBindVertexArray(__textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, __textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	constexpr const float vertices[] = {
            // back face
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            // front face
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            // left face
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            // right face
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            // bottom face
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            // top face
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,  // bottom-left        
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        };
	glGenVertexArrays(1, &__cubeVAO);
	glGenBuffers(1, &__cubeVBO);
	// fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, __cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// link vertex attributes
	glBindVertexArray(__cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Rendering::~Rendering()
{
	glDeleteVertexArrays(1, &__textVAO);
	glDeleteBuffers(1, &__textVBO);
	glDeleteVertexArrays(1, &__cubeVAO);
	glDeleteBuffers(1, &__cubeVBO);
}

GLuint Rendering::GetTextVAO() { return __textVAO; }
GLuint Rendering::GetTextVBO() { return __textVBO; }
GLuint Rendering::GetCubeVAO() { return __cubeVAO; }
GLuint Rendering::GetCubeVBO() { return __cubeVBO; }

void Rendering::Init()
{
	s_Rendering.reset(new Rendering());
	LightRendering::Init();
	ParticleSystemRendering::Init();
	LoadShadersAndFonts();
}

void Rendering::Refresh()
{
	LightRendering::RefreshUbo();
}

void Rendering::DrawFaces(Entity & entity)
{
	Shader & shader = entity.GetFaceShader();
	// Current Heaviest Line
	const glm::mat4 & model = entity.GetModelMatrix();

	shader.Use();

	// Heaviest line (~40% time passed here in the function)
	//auto id = glGetUniformLocation(shader.Program(), "model");
	//glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(model));
	shader.SetUniformMatrix4f("model", model);

	if (entity.GetPbrMaterial() && s_cubemap)
	{
		shader.SetUniformInt("irradianceMap", 0);
		shader.SetUniformInt("prefilterMap", 1);
		shader.SetUniformInt("brdfLUT", 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, s_cubemap->irradianceMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, s_cubemap->prefilterMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, s_cubemap->brdfLUTTexture);
	}
	else if ((*entity.GetMesh())->HasTextureCoordinates() && entity.GetTexture().GetWidth() != 0)
	{
		shader.SetUniformInt("_texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entity.GetTexture().GetTexture());
	}
	else
	{
		shader.SetUniformInt("useTexture", 0);
	}

	for (const auto & pair : entity.shaderAttributes)
		pair.second->Render(pair.first, shader);

	for (const auto & pair : entity.attributes)
	{
		auto & attribute = *pair.second;
		attribute.Render(shader);
	}

	glBindVertexArray(entity.GetMesh().facesVAO());

	GLenum primitiveMode = (*shader)->GetPrimitiveMode();
	if (primitiveMode == GL_PATCHES) glPatchParameteri(GL_PATCH_VERTICES, 25);

	switch (entity.GetMesh().GetDrawMode())
	{
		case Mesh_Base::DrawMode::DrawElements:
			glDrawElements(primitiveMode, entity.GetMesh().facesNVert(), GL_UNSIGNED_INT, 0);
			break;
		case Mesh_Base::DrawMode::DrawArrays:
			glDrawArrays(primitiveMode, 0, entity.GetMesh().facesNVert());
			break;
	}
	glBindVertexArray(0);
}

void Rendering::DrawWireframe(Entity & entity)
{
	Shader & shader = entity.GetWireframeShader();
	const glm::mat4 & model = entity.GetModelMatrix();
	
	shader.Use();

	shader.SetUniformMatrix4f("model", model);

	// use the same color for all points
	shader.SetUniformFloat("ourColor", WireframeColors[0]);

	for(const auto & pair : entity.shaderAttributes)
		pair.second->Render(pair.first, shader);

	glBindVertexArray(entity.GetMesh().facesVAO());
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLenum primitiveMode = (*shader)->GetPrimitiveMode();
	if (primitiveMode == GL_PATCHES) glPatchParameteri(GL_PATCH_VERTICES, 25);

	switch (entity.GetMesh().GetDrawMode())
	{
		case Mesh_Base::DrawMode::DrawElements:
			glDrawElements(primitiveMode, entity.GetMesh().facesNVert(), GL_UNSIGNED_INT, 0);
			break;
		case Mesh_Base::DrawMode::DrawArrays:
			glDrawArrays(primitiveMode, 0, entity.GetMesh().facesNVert());
			break;
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void Rendering::DrawVertices(Entity & entity)
{
	Shader & shader = entity.GetPointShader();
	const glm::mat4 & model = entity.GetModelMatrix();
	
	shader.Use();

	shader.SetUniformMatrix4f("model", model);

	// use the same color for all points
	shader.SetUniformFloat("ourColor", WireframeColors[0]);

	glBindVertexArray(entity.GetMesh().verticesVAO());

	glDrawArrays(GL_POINTS, 0, entity.GetMesh().verticesNVert());
	glBindVertexArray(0);
}

void Rendering::DrawEntity(Entity & entity)
{
	if (DisplayMode & RenderingMode::VerticesMode)  DrawVertices(entity);
	if (DisplayMode & RenderingMode::WireframeMode) DrawWireframe(entity);
	if (DisplayMode & RenderingMode::FacesMode)     DrawFaces(entity);
}

void Rendering::RotateWireframeColor()
{
	std::rotate(WireframeColors.begin(), WireframeColors.begin() + 1, WireframeColors.end());
}

void Rendering::DrawImage(Image2D & image)
{
	image.shaderFace.Use();

	glDepthMask(GL_FALSE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, image.texture.GetTexture());
	glBindVertexArray(image.mesh.facesVAO());

	const glm::vec2 & wDimensions = mainCamera->GetWindowDimensions();

	const GLfloat vertices[4][4] = {
		{ 0.0, 0.0,   0.0, 0.0 },
		{ wDimensions.x, 0.0,   1.0, 0.0 },
		{ 0.0, wDimensions.y,   0.0, 1.0 },
		{ wDimensions.x, wDimensions.y,   1.0, 1.0 }
	};

	glBindBuffer(GL_ARRAY_BUFFER, image.mesh.facesVBO());
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
}

void Rendering::DrawText(Text2D & text)
{
	text.shader.Use();
	text.shader.SetUniformFloat("textColor", text.color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(s_Rendering->GetTextVAO());

	const glm::vec2 & wDimensions = mainCamera->GetWindowDimensions();

	// Iterate through all characters
	const auto & characters = text.font.GetCharacters();
	GLfloat x = text.pos.x * wDimensions.x, y = text.pos.y * wDimensions.y;
	GLfloat scale = text.scale / text.font.GetFontSize();

	// Centering text
	if (text.centered)
	{
		GLfloat totalW = 0.0f;
		int i = 0;
		for (const auto & c : text.str)
		{
			const Character & ch = *characters.at(c);

			if (i + 1 != text.str.size())
				totalW += ((ch.GetAdvance() >> 6) * scale);
			else
				totalW += (ch.GetSize().x * scale);
			++i;
		}
		x -= totalW / 2.0f;
	}

	for (const auto & c : text.str)
	{
		const Character & ch = *characters.at(c);

		GLfloat xpos = x + ch.GetBearing().x * scale;
		GLfloat ypos = y - (ch.GetSize().y - ch.GetBearing().y) * scale;

		GLfloat w = ch.GetSize().x * scale;
		GLfloat h = ch.GetSize().y * scale;
		// Update VBO for each character
		GLfloat vertices[4][4] = {
			{ xpos,     ypos,       0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.GetTextureID());
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, s_Rendering->GetTextVBO());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // GL_TRIANGLE_STRIP vs. GL_TRIANGLES, STRIP has more performance
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.GetAdvance() >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Rendering::DrawText(Text3D & text)
{
	text.shader.Use();
	text.shader.SetUniformFloat("textColor", text.color);

	text.shader.SetUniformMatrix4f("model", text.GetModelMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(s_Rendering->GetTextVAO());

	// Iterate through all characters
	const auto & characters = text.font.GetCharacters();
	GLfloat x = 0.0f, y = 0.0f;
	GLfloat scale = text.scale / text.font.GetFontSize();

	// Centering text
	if (text.centered)
	{
		GLfloat totalW = 0.0f;
		int i = 0;
		for (const auto & c : text.str)
		{
			const Character & ch = *characters.at(c);

			if (i + 1 != text.str.size())
				totalW += ((ch.GetAdvance() >> 6) * scale);
			else
				totalW += (ch.GetSize().x * scale);
			++i;
		}
		x -= totalW / 2.0f;
	}

	for (const auto & c : text.str)
	{
		const Character & ch = *characters.at(c);

		GLfloat xpos = x + ch.GetBearing().x * scale;
		GLfloat ypos = y - (ch.GetSize().y - ch.GetBearing().y) * scale;

		GLfloat w = ch.GetSize().x * scale;
		GLfloat h = ch.GetSize().y * scale;
		// Update VBO for each character
		GLfloat vertices[4][4] = {
			{ xpos,     ypos,       0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.GetTextureID());
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, s_Rendering->GetTextVBO());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // GL_TRIANGLE_STRIP vs. GL_TRIANGLES, STRIP has more performance
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.GetAdvance() >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Rendering::RotateFonts()
{
	std::rotate(fonts.begin(), fonts.begin() + 1, fonts.end());
}

void Rendering::DrawBrdfCubemap(Brdf_Cubemap & cubemap)
{
	cubemap.shader.Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.cubemapTexture);
	RenderCube();
}

void Rendering::DrawParticleSystem(ParticleSystem_Base * particleSystem)
{
	ParticleSystemRendering::DrawParticleSystem(particleSystem);
}

void Rendering::LoadShadersAndFonts()
{
	Font font1 = GenerateFont(Constants::Paths::arialFont);
	Font font2 = GenerateFont(Constants::Paths::starFont);
	Font font3 = GenerateFont(Constants::Paths::notesFont);

	// Setting default font
	SetDefaultFont(font1);

	fonts.insert(fonts.begin(), { font1, font2, font3 });

	// Build and compile our shader program
	Shader lightShader     = GenerateShader(Constants::Paths::lightShaderVertex, Constants::Paths::lightShaderFrag);
	Shader pointShader     = GenerateShader(Constants::Paths::pointShaderVertex, Constants::Paths::pointShaderFrag);
	Shader faceShader      = GenerateShader(Constants::Paths::faceShaderVertex, Constants::Paths::faceShaderFrag);
	Shader face2DShader    = GenerateShader(Constants::Paths::face2DShaderVertex, Constants::Paths::face2DShaderFrag);
	Shader wireframeShader = GenerateShader(Constants::Paths::wireframeShaderVertex, Constants::Paths::wireframeShaderFrag);
	Shader text2DShader    = GenerateShader(Constants::Paths::text2DShaderVertex, Constants::Paths::text2DShaderFrag);
	Shader text3DShader    = GenerateShader(Constants::Paths::text3DShaderVertex, Constants::Paths::text3DShaderFrag);
	Shader particleShader  = GenerateShader(Constants::Paths::particleShaderVertex, Constants::Paths::particleShaderFrag);
	Shader snowShader      = GenerateShader(Constants::Paths::snowShaderVertex, Constants::Paths::snowShaderFrag);
	Shader bezierShader    = GenerateShader(Constants::Paths::bezierShaderVertex, Constants::Paths::bezierShaderFrag, Constants::Paths::bezierShaderTcs, Constants::Paths::bezierShaderTes);

	Shader bezierWireframeShader = GenerateShader(Constants::Paths::bezierWireframeShaderVertex, Constants::Paths::bezierWireframeShaderFrag, Constants::Paths::bezierWireframeShaderTcs, Constants::Paths::bezierWireframeShaderTes);

	Shader axisDisplayerShader = GenerateShader(Constants::Paths::axisDisplayerShaderVertex, Constants::Paths::axisDisplayerShaderFrag);

	Shader backgroundShader = GenerateShader(Constants::Paths::backgroundVertex, Constants::Paths::backgroundFrag);
	backgroundShader.SetUniformInt("environmentMap", 0);

	Shader pbrShader = GenerateShader(Constants::Paths::pbrVertex, Constants::Paths::pbrFrag);

	shaders.insert({Constants::Paths::lightShaderVertex,     std::make_unique<Shader>(lightShader)});
	shaders.insert({Constants::Paths::pointShaderVertex,     std::make_unique<Shader>(pointShader)});
	shaders.insert({Constants::Paths::faceShaderVertex,      std::make_unique<Shader>(faceShader)});
	shaders.insert({Constants::Paths::face2DShaderVertex,    std::make_unique<Shader>(face2DShader)});
	shaders.insert({Constants::Paths::wireframeShaderVertex, std::make_unique<Shader>(wireframeShader)});
	shaders.insert({Constants::Paths::text2DShaderVertex,    std::make_unique<Shader>(text2DShader)});
	shaders.insert({Constants::Paths::text3DShaderVertex,    std::make_unique<Shader>(text3DShader)});
	shaders.insert({Constants::Paths::particleShaderVertex,  std::make_unique<Shader>(particleShader)});
	shaders.insert({Constants::Paths::snowShaderVertex,      std::make_unique<Shader>(snowShader) });
	shaders.insert({Constants::Paths::bezierShaderVertex,    std::make_unique<Shader>(bezierShader) });
	shaders.insert({Constants::Paths::bezierWireframeShader,    std::make_unique<Shader>(bezierWireframeShader) });
	shaders.insert({Constants::Paths::axisDisplayerShaderVertex,    std::make_unique<Shader>(axisDisplayerShader) });
	shaders.insert({Constants::Paths::backgroundVertex,    std::make_unique<Shader>(backgroundShader) });
	shaders.insert({Constants::Paths::pbrVertex,    std::make_unique<Shader>(pbrShader) });

	// Setting default shaders
	SetDefaultPointShader(pointShader);
	SetDefaultFaceShader(faceShader);
	SetDefaultWireframeShader(wireframeShader);
	SetDefault2DTextShader(text2DShader);
	SetDefault3DTextShader(text3DShader);
	SetDefaultLightShader(lightShader);

	pointShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);
	faceShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);
	wireframeShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);
	particleShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);
	snowShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);
	bezierShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);
	bezierWireframeShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);
	backgroundShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);
	pbrShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);

	face2DShader.AddGlobalUbo(Constants::UBO::Ids::projection, Constants::UBO::Names::projection);
	axisDisplayerShader.AddGlobalUbo(Constants::UBO::Ids::projection, Constants::UBO::Names::projection);

	faceShader.AddGlobalUbo(Constants::UBO::Ids::lights, Constants::UBO::Names::lights);
	pbrShader.AddGlobalUbo(Constants::UBO::Ids::lights, Constants::UBO::Names::lights);

	text2DShader.AddGlobalUbo(Constants::UBO::Ids::projection, Constants::UBO::Names::projection);
	text3DShader.AddGlobalUbo(Constants::UBO::Ids::cameraProps, Constants::UBO::Names::cameraProps);
}

void Rendering::RenderCube()
{
	// render Cube
	glBindVertexArray(s_Rendering->GetCubeVAO());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

Shader & Rendering::Shaders(const std::string & str)
{
	return *shaders.at(str);
}
