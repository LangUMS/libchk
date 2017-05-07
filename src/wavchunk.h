#ifndef __LIBCHK_WAVCHUNK_H
#define __LIBCHK_WAVCHUNK_H

#include "ichunk.h"

namespace CHK
{

    class WAVChunk : public IChunk
    {
        public:
        WAVChunk(const std::vector<char>& data, const std::string& type) : IChunk(type)
        {
            SetBytes(data);
        }

        unsigned int Get(unsigned int index) const
        {
            auto& bytes = GetBytes();
            return *((unsigned int*)(bytes.data() + index * sizeof(uint32_t)));
        }

        void Set(unsigned int index, unsigned int value)
        {
            auto& bytes = GetBytes();
            *((unsigned int*)(bytes.data() + index * sizeof(uint32_t))) = value;
        }

        int FindFreeIndex() const
        {
            for (auto i = 0; i < 512; i++)
            {
                if (Get(i) == 0)
                {
                    return i;
                }
            }

            return -1;
        }
    };

}

#endif
