#ifndef VOXL_RENDER_PIPELINE_H
#define VOXL_RENDER_PIPELINE_H


#include <cstdint>
#include <vector>

#include "render_types.h"


namespace voxl {


enum class CullMode { None, Front, Back };
enum class PolygonMode { Fill, Line };
enum class CompareOp { Less, Greater, Always }; // se renseigner d'avantage
enum class BlendFactor { Src, OneMinusSrc, Dst, OneMinusDst };
enum class PrimitiveTopology { Triangle, Line, Point };


struct RasterizerState {
  CullMode cullMode = CullMode::None;
  PolygonMode polygonMode = PolygonMode::Fill;
  bool frontFaceCCW = true; // sens inverse des aiguilles d'une montre pour déterminer la face avant d'un triangle
  float lineWidth = 1.0f;
};


struct DepthStencilState {
  bool enableDepthTest = true;
  bool enableDepthWrite = true;
  CompareOp depthCompareOp = CompareOp::Less;
};


struct BlendState {
  bool enableBlend = false; // utile pour les couche transparentes
  BlendFactor srcFactor;
  BlendFactor dstFactor;
};


enum AttribLocation {
  Position = 0,
  Normal = 1,
  UV = 2,
  Color = 3,
}; // TODO gérer aussi les tangents, etc...


enum class AttribFormat {
  Float1,
  Float2,
  Float3,
  Int1,
  Int2,
  Int3,
  UInt1,
};


// layout des vertex
struct VertexAttribute {
  AttribLocation location;
  AttribFormat format;
  bool normalized = false;
  uint32_t offset;
};


struct VertexInputLayout {
  std::vector<VertexAttribute> attributes;
  uint32_t binding;
  uint32_t stride;
};


struct PipelineDesc {
  ProgramHandle program;

  VertexInputLayout inputLayout;
  RasterizerState rasterizer;
  DepthStencilState depthStencil;
  BlendState blend;

  PrimitiveTopology topology = PrimitiveTopology::Triangle;

  RenderPassHandle compatibleRenderPass = 0; // utile pour vulkan, à voir pour plus tard
};


class IPipeline {
public:
  virtual ~IPipeline() = default;
};


}


#endif //! VOXL_RENDER_PIPELINE_H