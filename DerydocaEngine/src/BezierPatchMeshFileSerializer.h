#pragma once
#include "FileTypeSerializer.h"

namespace DerydocaEngine::Files::Serializers {

	class BezierPatchMeshFileSerializer : public FileTypeSerializer
	{
	public:
		BezierPatchMeshFileSerializer() {}
		~BezierPatchMeshFileSerializer() {}

		YAML::Node generateResourceNodes(std::string const& filePath);
		Files::FileType getFileType();
		Resources::Resource* loadResourceFromMeta(YAML::Node const& resourceNode);
	};

	// Based on http://www.holmes3d.net/graphics/roffview/tools/patchoff/
}