#ifndef OBSIDIAN2D_CORE_CAMERA_H
#define OBSIDIAN2D_CORE_CAMERA_H

#include "Obsidian2D/Renderer/VulkanInfo.h"
#include "Pipeline.h"

namespace Obsidian2D
{
	namespace Renderer
	{
		class Camera : public Pipeline
		{
		private:

			ViewCamera _view_camera;

			glm::mat4 _projection;
			glm::mat4 _view;
			glm::mat4 _model;
			glm::mat4 _clip;

			glm::mat4 _mvp; // Model View Projection

			void* _buffer_address = nullptr;

			void updateMVP()
			{
				this->_mvp = this->_clip * this->_projection * this->_view * this->_model;
			}

		public:

			void initCamera(int width, int height)
			{
				float fov = glm::radians(45.0f);
				if (width > height) {
					fov *= static_cast<float>(height) / static_cast<float>(width);
				}
				this->_projection = glm::perspective(fov, static_cast<float>(width) / static_cast<float>(height), 0.2f, 1.0f);

				this->_view_camera.eye 	  = glm::vec3(0, 0, -10); 	// Camera is at (-5,3,-10), in World Space
				this->_view_camera.center = glm::vec3(0, 0,   0); 	// and looks at the origin
				this->_view_camera.up     = glm::vec3(0, 1,   0);    // Head is up (set to 0,-1,0 to look upside-down)

				this->_view = glm::lookAt( this->_view_camera.eye, this->_view_camera.center, this->_view_camera.up );

				this->_model = glm::mat4(1.0f);
				// Vulkan clip space has inverted Y and half Z.
				this->_clip = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
										0.0f, -1.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.5f, 0.5f,
										0.0f, 0.0f, 0.0f, 1.0f);

				this->updateMVP();
			}

			void setCameraBufferAddress(void * buffer_address)
			{
				this->_buffer_address = buffer_address;
			}

			void setCameraViewEye(glm::vec3 eye)
			{
				this->_view_camera.eye = eye;
				this->_view = glm::lookAt( this->_view_camera.eye, this->_view_camera.center, this->_view_camera.up );
				this->updateMVP();
			}

			void setCameraViewCenter(glm::vec3 center)
			{
				this->_view_camera.center = center;
				this->_view = glm::lookAt( this->_view_camera.eye, this->_view_camera.center, this->_view_camera.up );
				this->updateMVP();
			}

			void setCameraViewUp(glm::vec3 up)
			{
				this->_view_camera.up = up;
				this->_view = glm::lookAt( this->_view_camera.eye, this->_view_camera.center, this->_view_camera.up );
				this->updateMVP();
			}

			void updateCamera()
			{
				assert(this->_buffer_address != nullptr);
				memcpy(this->_buffer_address, &this->_mvp, sizeof(this->_mvp));
			}

		};
	}
}


#endif //OBSIDIAN2D_CORE_CAMERA_H
