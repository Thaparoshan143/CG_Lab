#include"OpenGL_Buffer.h"

namespace OpenGL
{
    VertexBufferObject::VertexBufferObject(uint usage)
    {
        glGenBuffers(1, &m_id);
        m_usage = usage;
    }

    void VertexBufferObject::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBufferObject::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBufferObject::OffloadData()
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(float), m_data.data(), m_usage);
    }

    void VertexBufferObject::ReserveBuffer(uint size, uint loadMode)
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, NULL, loadMode);
    }

    void VertexBufferObject::LoadSubBuffer(uint size, float *data)
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * size, data);
    }

    IndexBufferObject::IndexBufferObject(uint usage)
    {
        glGenBuffers(1, &m_id);
        m_usage = usage;
    }

    void IndexBufferObject::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBufferObject::OffloadData()
    {
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_data.size() * sizeof(uint), m_data.data(), m_usage);
    }

    void IndexBufferObject::ReserveBuffer(uint size, uint loadMode)
    {
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * size, NULL, loadMode);
    }

    void IndexBufferObject::LoadSubBuffer(uint size, uint *data)
    {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint) * size, data);
    }

    void IndexBufferObject::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    VertexArrayObject::VertexArrayObject(BufferLayout layout)
    {
        glGenVertexArrays(1, &m_id);
        m_layout = layout;
    }

    void VertexArrayObject::Bind()
    {
        glBindVertexArray(m_id);
    }

    void VertexArrayObject::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArrayObject::SetVertexAttributePointer(bool normalized)
    {
        Bind();
        // offset is in byte;
        uint offset = 0;
        uint vertCount = 0;
        for(uint i = 0;i<getLayoutCount();++i)
        {
            vertCount = BUFFER_3BTTMASK & BITSHIFTRIGHT((uint)m_layout, i*3);
            glVertexAttribPointer(i, vertCount, GL_FLOAT, normalized, getStrideCount() * sizeof(float), (const void*)(offset));
            glEnableVertexAttribArray(i);
            offset += sizeof(float) * vertCount;
        }
    }

    void VertexArrayObject::SetVertexAttributePointerMan(uint count, bool normalized)
    {
        glBindVertexArray(this->m_id);
		glVertexAttribPointer(0, count, GL_FLOAT, normalized, count * sizeof(float), 0);
		glEnableVertexAttribArray(0);
    }

    uint VertexArrayObject::getLayoutCount()
    {
        uint uintLayout = static_cast<uint>(m_layout);
        return ((GETMASK(uintLayout, BUFFER_MASKPOSITION)!=0) + (GETMASK(uintLayout, BUFFER_MASKCOLOR)!=0) + (GETMASK(uintLayout, BUFFER_MASKTEXTURE)!=0));
    }

    uint VertexArrayObject::getStrideCount()
    {
        uint uintLayout = static_cast<uint>(m_layout);
        return (GETMASK(uintLayout, BUFFER_MASKPOSITION) + GETMASKBITSHIFTRIGHT(uintLayout, BUFFER_MASKCOLOR, BITSHIFT3) + GETMASKBITSHIFTRIGHT(uintLayout, BUFFER_MASKTEXTURE, (BITSHIFT3+BITSHIFT3)));
    }
}