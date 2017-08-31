//
// Created by tracksale on 8/31/17.
//

#ifndef OBSIDIAN2D_VERTEX_BUFFER_H
#define OBSIDIAN2D_VERTEX_BUFFER_H

#include <vector>
#include "Memory.h"
#include "Buffer.h"

struct Vertex {
    float pos[3];
    float uv[2];
    float normal[3];
};

std::vector<Vertex> vertexData = {
        { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
        { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
        { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },

        { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
        { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
        { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } }

};

namespace Obsidian2D
{
    namespace Renderer
    {
        class VertexBuffer: public Buffer {

        private:
            std::vector<Vertex> _vertexData = {};

            void updateMemoryWithData()
            {
                Memory::copyMemory(_instance_device, mem, _vertexData.data(), (_vertexData.size() * sizeof(Vertex)));
            }

        public:

            VertexBuffer(struct BufferData buffer_data): Buffer(buffer_data)
            {
                this->_vertexData = vertexData;
                updateMemoryWithData();
            }

            void push(Vertex vertex)
            {
                _vertexData.push_back(vertex);
                this->updateMemoryWithData();
            }

            void update(std::vector<Vertex> vertex)
            {
                _vertexData = vertex;
                this->updateMemoryWithData();
            }

            void remove(int index)
            {
                _vertexData.erase(_vertexData.begin() + index);
                this->updateMemoryWithData();
            }

        };
    }
}
#endif //OBSIDIAN2D_VERTEX_BUFFER
