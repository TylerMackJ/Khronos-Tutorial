#pragma once

#include <vector>

#include "graphicsPipeline/Vertex.hpp"

class ModelLoader
{
public:
    ModelLoader( const char* modelPath );
    ~ModelLoader();

    const std::vector<Vertex>& const getVertices() { return vertices; }
    const std::vector<uint32_t>& const getIndices() { return indices; }

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};