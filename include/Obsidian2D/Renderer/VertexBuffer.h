//
// Created by tracksale on 8/31/17.
//

#ifndef OBSIDIAN2D_VERTEX_BUFFER_H
#define OBSIDIAN2D_VERTEX_BUFFER_H

#include <vector>
#include "Memory.h"
#include "Buffer.h"

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

            VertexBuffer(struct BufferData buffer_data, std::vector<Vertex> vertexData): Buffer(buffer_data)
            {
                this->_vertexData = vertexData;
                updateMemoryWithData();
            }

            void update(std::vector<Vertex> vertex)
            {
                _vertexData = vertex;
                this->updateMemoryWithData();
            }

            unsigned long getVertexSize()
            {
                return _vertexData.size();
            }

            static std::vector<VkBuffer> getBuffersFromVector(std::vector<VertexBuffer *> vector)
            {
                std::vector<VkBuffer> buffers = {};

                for(auto vertex : vector){
                    buffers.push_back(vertex->buf);
                }

                return buffers;
            }

        };
    }
}
#endif //OBSIDIAN2D_VERTEX_BUFFER
