#pragma once

#include <vector>

#include "Vertex.hpp"

namespace ModelLoader
{

    class Model
    {
    public:
        Model( const char* modelPath );
        ~Model() = default;

        const std::vector< Vertex >& getVertices() { return vertices; }
        const std::vector< uint32_t >& getIndices() { return indices; }

    private:
        std::vector< Vertex > vertices;
        std::vector< uint32_t > indices;
    };

} // namespace ModelLoader