#ifndef VOXL_RENDER_COMMAND_H
#define VOXL_RENDER_COMMAND_H


#include "render_types.h"
#include <glm/gtc/matrix_transform.hpp>


namespace voxl {


struct RenderCommand {
  MeshHandle mesh;
  MaterialHandle material;
  glm::mat4 transform;
  float distToCamera; // pour le tri, sujet à modifications
};


}


#endif //! VOXL_RENDER_COMMAND_H