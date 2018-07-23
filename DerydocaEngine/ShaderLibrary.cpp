#include "ShaderLibrary.h"
#include "ObjectLibrary.h"
#include "ShaderResource.h"

Shader * ShaderLibrary::find(uuid shaderId)
{
	// Look through the list of already loaded shaders
	auto it = m_shaders.find(shaderId);

	// If the shader was found, return it
	if (it != m_shaders.end())
	{
		return it->second;
	}

	// Otherwise, try to load the shader
	// Get the resource object
	Resource* resource = ObjectLibrary::getInstance().getResource(shaderId);
	if (resource == nullptr)
	{
		cout << "Unable to find a resource with the ID of '" << shaderId << "'." << endl;
		return nullptr;
	}

	// Convert the resource object to a shader resource object
	ShaderResource* shaderResource = static_cast<ShaderResource*>(resource);
	if (shaderResource == nullptr)
	{
		cout << "The resource with ID of '" << shaderId << "' is not a shader." << endl;
		return nullptr;
	}

	// Load the shader from the resource object
	Shader* shader = (Shader*)shaderResource->getResourceObject();

	// Add it to the library
	m_shaders[shaderId] = shader;

	// Return the shader
	return shader;
}

Shader * ShaderLibrary::find(string shaderPath)
{
	// Find the uuid associated with the path
	auto it = m_shaderPaths.find(shaderPath);

	// If it could not be found, return with a null shader
	if (it == m_shaderPaths.end())
	{
		cout << "Unable to find a shader uuid associated with the path '" << shaderPath << "'." << endl;
		return nullptr;
	}

	// Otherwise, use the UUID to load the shader
	return find(it->second);
}

void ShaderLibrary::registerShaderName(string shaderPath, uuid shaderUuid)
{
	m_shaderPaths.insert(pair<string, uuid>(shaderPath, shaderUuid));
}