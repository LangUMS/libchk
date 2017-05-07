#ifndef __LIBCHK_MRGNCHUNK_H
#define __LIBCHK_MRGNCHUNK_H

#include "ichunk.h"

#define ANYWHERE_LOCATION 64

namespace CHK
{

    struct Location
    {
        uint32_t m_Left;
        uint32_t m_Top;
        uint32_t m_Right;
        uint32_t m_Bottom;
        uint16_t m_StringIndex;
        uint16_t m_Elevation;
    };

    class MRGNChunk : public IChunk
    {
        public:
        MRGNChunk(const std::vector<char>& data, const std::string& type) : IChunk(type)
        {
            SetBytes(data);
        }

        int FindLocation(unsigned int stringIndex) const
        {
            auto count = GetLocationsCount();
            for (auto i = 0u; i < count; i++)
            {
                auto loc = GetLocation(i);
                if (loc && loc->m_StringIndex == stringIndex)
                {
                    return i;
                }
            }

            return -1;
        }

        size_t GetLocationsCount() const
        {
            return 255;
        }

        Location* GetLocation(size_t index) const
        {
            if (index >= 255 || index == ANYWHERE_LOCATION)
            {
                return nullptr;
            }

            auto& bytes = GetBytes();
            return (Location*)(bytes.data() + sizeof(Location) * index);
        }
        
        void SetLocation(size_t index, const Location& location)
        {
            auto& bytes = GetBytes();
            auto loc = (Location*)(bytes.data() + sizeof(Location) * index);
            *loc = location;
        }
    };

}

#endif
