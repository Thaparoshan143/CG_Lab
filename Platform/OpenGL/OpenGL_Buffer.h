#pragma once

#include"../../Core/Buffer.h"

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"

#define BITSHIFTRIGHT(bits, shift) ( bits>>shift )
#define GETMASK(target, against) ( against & target )
#define GETMASKBITSHIFTRIGHT(target, against, shift) ( ((against & target)>>shift) )

namespace OpenGL 
{
	#define BUFFER_MASKPOSITION 0x00000007
	#define BUFFER_MASKCOLOR 0x00000038
	#define BUFFER_MASKTEXTURE 0x000001C0
    #define BUFFER_3BTTMASK BUFFER_MASKPOSITION
    #define BITSHIFT3 3

    enum class BufferLayout : uint
    {
        PP = 2,
		PPP = 3,
		PPPP = 4,
        MASK_PPPP = 7,

		PP_RGB = 26,
		PPP_RGB = 27,
		PPPP_RGB = 28,

		PP_RGBA = 34,
		PPP_RGBA = 35,
		PPPP_RGBA = 36,
        MASK_PPPP_RGBA = 56,

		PP_RGB_UV = 154,
		PPP_RGB_UV = 155,
		PPPP_RGB_UV = 156,

        PP_RGBA_UV = 162,
		PPP_RGBA_UV = 163,
		PPPP_RGBA_UV = 164,
        MASK_PPPP_RGBA_UV = 448
    };

    class VertexBufferObject : public Interface::IBufferObject<float> 
    {
        public:
        VertexBufferObject(uint usage = GL_DYNAMIC_DRAW);

        void Bind() override;
        void Unbind() override;
        // Buffer is Offload from CPU to GPU Memory
        void OffloadData() override;
		void ReserveBuffer(uint size, uint loadMode = GL_DYNAMIC_DRAW);
		void LoadSubBuffer(uint size, float *data);
    };

    class IndexBufferObject : public Interface::IBufferObject<uint> 
    {
        public:
        IndexBufferObject(uint usage = GL_DYNAMIC_DRAW);

        void Bind() override;
        void Unbind() override;
        // Buffer is Offload from CPU to GPU Memory
        void OffloadData() override;
		void ReserveBuffer(uint size, uint loadMode = GL_DYNAMIC_DRAW);
		void LoadSubBuffer(uint size, uint *data);
    };

    class VertexArrayObject
    {
        public:
        VertexArrayObject(BufferLayout layout = BufferLayout::PPP_RGB);

        void SetBufferLayout(BufferLayout layout) { m_layout = layout;   }
        void SetVertexAttributePointer(bool normalized = GL_FALSE);
        void SetVertexAttributePointerMan(uint count, bool normalized = GL_FALSE);

        void Bind();
        void Unbind();

        uint getLayoutCount();
        uint getStrideCount();

        uint m_id;
        BufferLayout m_layout;
    };
}