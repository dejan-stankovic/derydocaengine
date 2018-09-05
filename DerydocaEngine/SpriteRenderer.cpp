#include "SpriteRenderer.h"
#include "ObjectLibrary.h"
#include "YamlTools.h"

void SpriteRenderer::postInit()
{
	if (m_spriteSheet)
	{
		Texture* spriteSheetTexture = m_spriteSheet->getTexture();
		getMaterial()->setTexture("SpriteSheet", spriteSheetTexture);
		markComponentAsDirty(MeshComponents::All);
	}
}

void SpriteRenderer::deserialize(YAML::Node const& compNode)
{
	using namespace YAML;

	m_spriteSheet = getResourceObject<SpriteSheet>(compNode, "spriteSheet");
	Shader* shader = getResourceObject<Shader>(compNode, "shader");
	Material* material = new Material();
	material->setShader(shader);
	setMaterial(material);

	Node sizeNode = compNode["size"];
	if (sizeNode)
	{
		m_size = sizeNode.as<vec2>();
	}

	Node colorNode = compNode["color"];
	if (colorNode)
	{
		m_color = colorNode.as<Color>();
	}

	Node spriteIdNode = compNode["spriteId"];
	if (spriteIdNode)
	{
		unsigned int spriteId = spriteIdNode.as<unsigned int>();
		m_sprite = m_spriteSheet->getSpriteReference(spriteId);
	}
}

vec3 * SpriteRenderer::generateVertices()
{
	// Get the number of verts we will need for this mesh
	int numVerts = generateNumVertices();
	if (!numVerts)
	{
		return nullptr;
	}

	vec3* vertices = new vec3[numVerts];

	bool nativeSize = m_size.x == 0.0f && m_size.y == 0.0f;
	float c0 = 0.0f;
	float c3 = nativeSize ? m_sprite->getWidth() : m_size.x;
	float r0 = -(0.0f);
	float r3 = -(nativeSize ? m_sprite->getHeight() : m_size.y);

	// Set the corner positions
	vertices[0] = vec3(c0, r0, 0.0f);
	vertices[1] = vec3(c3, r0, 0.0f);
	vertices[2] = vec3(c0, r3, 0.0f);
	vertices[3] = vec3(c3, r3, 0.0f);

	// If this is a nine-slice sprite, set the positions for the remaining verts
	if (m_sprite->getType() == SpriteType::NineSlice)
	{
		float c1 = c0 + (m_sprite->getWidth() * m_sprite->getSliceLeft());
		float c2 = c3 - (m_sprite->getWidth() * (1 - m_sprite->getSliceRight()));
		float r1 = r0 - (m_sprite->getHeight() * m_sprite->getSliceTop());
		float r2 = r3 + (m_sprite->getHeight() * (1 - m_sprite->getSliceBottom()));

		// Fill the gap of two in the first line
		vertices[4] = vec3(c1, r0, 0.0f);
		vertices[5] = vec3(c2, r0, 0.0f);

		// Build the second line
		vertices[6] = vec3(c0, r1, 0.0f);
		vertices[7] = vec3(c1, r1, 0.0f);
		vertices[8] = vec3(c2, r1, 0.0f);
		vertices[9] = vec3(c3, r1, 0.0f);

		// Build the third line
		vertices[10] = vec3(c0, r2, 0.0f);
		vertices[11] = vec3(c1, r2, 0.0f);
		vertices[12] = vec3(c2, r2, 0.0f);
		vertices[13] = vec3(c3, r2, 0.0f);

		// Fill the gap of the last line
		vertices[14] = vec3(c1, r3, 0.0f);
		vertices[15] = vec3(c2, r3, 0.0f);
	}

	return vertices;
}

vec2 * SpriteRenderer::generateTexCoords()
{
	// Get the number of verts we will need for this mesh
	int numVerts = generateNumVertices();
	if (!numVerts)
	{
		return nullptr;
	}

	vec2* texCoords = new vec2[numVerts];

	float c0 = m_sprite->getTexPosition().getX();
	float c3 = m_sprite->getTexPosition().getDX();
	float r0 = 1.0f - m_sprite->getTexPosition().getY();
	float r3 = 1.0f - m_sprite->getTexPosition().getDY();

	// Set the corners
	texCoords[0] = vec2(c0, r0);
	texCoords[1] = vec2(c3, r0);
	texCoords[2] = vec2(c0, r3);
	texCoords[3] = vec2(c3, r3);

	// If this is a nine-slice sprite, set the UVs for the remaining verts
	if (m_sprite->getType() == SpriteType::NineSlice)
	{
		float texw = c3 - c0;
		float texh = r3 - r0;

		float c1 = c0 + (texw * m_sprite->getSliceLeft());
		float c2 = c0 + (texw * m_sprite->getSliceRight());
		float r1 = r0 + (texh * m_sprite->getSliceTop());
		float r2 = r0 + (texh * m_sprite->getSliceBottom());

		// Fill the gap of two in the first line
		texCoords[4] = vec2(c1, r0);
		texCoords[5] = vec2(c2, r0);

		// Build the second line
		texCoords[6] = vec2(c0, r1);
		texCoords[7] = vec2(c1, r1);
		texCoords[8] = vec2(c2, r1);
		texCoords[9] = vec2(c3, r1);

		// Build the third line
		texCoords[10] = vec2(c0, r2);
		texCoords[11] = vec2(c1, r2);
		texCoords[12] = vec2(c2, r2);
		texCoords[13] = vec2(c3, r2);

		// Fill the gap of the last line
		texCoords[14] = vec2(c1, r3);
		texCoords[15] = vec2(c2, r3);
	}

	return texCoords;
}

Color * SpriteRenderer::generateVertexColors()
{
	// Get the number of verts we will need for this mesh
	int numVerts = generateNumVertices();
	if (!numVerts)
	{
		return nullptr;
	}

	Color* vertexColors = new Color[numVerts];

	for (int i = 0; i < numVerts; i++)
	{
		vertexColors[i] = m_color;
	}

	return vertexColors;
}

unsigned int * SpriteRenderer::generateTriangleIndices()
{
	switch (m_sprite->getType())
	{
	case SpriteType::Sprite:
		return new unsigned int[2 * 3]{
			0, 2, 1,
			1, 2, 3
		};
	case SpriteType::NineSlice:
		return new unsigned int[18 * 3]{
			0, 6, 4, // Row 1 triangles
			4, 6, 7,
			4, 7, 5,
			5, 7, 8,
			5, 8, 1,
			1, 8, 9,

			6, 10, 7, // Row 2 triangles
			7, 10, 11,
			7, 11, 8,
			8, 11, 12,
			8, 12, 9,
			9, 12, 13,

			10, 2, 11, // Row 3 triangles
			11, 2, 14,
			11, 14, 12,
			12, 14, 15,
			12, 15, 13,
			13, 15, 3
		};
	default:
		return nullptr;
	}
}