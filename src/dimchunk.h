#ifndef __LIBCHK_DIMCHUNK_H
#define __LIBCHK_DIMCHUNK_H

#include "ichunk.h"

namespace CHK
{

    class DIMChunk : public IChunk
    {
        public:
        DIMChunk(const std::vector<char>& data, const std::string& type) : IChunk(type)
        {
            SetBytes(data);
        }

        uint16_t GetWidth() const
        {
            return m_Width;
        }

        uint16_t GetHeight() const
        {
            return m_Height;
        }

        void SetBytes(const std::vector<char>& data)
        {
            m_Width = *((uint16_t*)data.data());
            m_Height = *((uint16_t*)(data.data() + 2));
            IChunk::SetBytes(data);
        }

        private:
        uint16_t m_Width = 0;
        uint16_t m_Height = 0;
    };

}

#endif
