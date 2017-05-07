#ifndef __LIBCHK_SPRPCHUNK_H
#define __LIBCHK_SPRPCHUNK_H

#include <unordered_map>
#include <string>

#include "ichunk.h"
#include "serialization.h"

namespace CHK
{

    class CHKSprpChunk : public IChunk
    {
        public:
        CHKSprpChunk(const std::vector<char>& data, const std::string& type) : IChunk(type)
        {
            SetBytes(data);
        }

        uint16_t GetScenarioStringId() const
        {
            auto& bytes = GetBytes();
            return *((uint16_t*)bytes.data());
        }

        void SetScenarioStringId(uint16_t stringId)
        {
            auto& bytes = GetBytes();
            *((uint16_t*)bytes.data()) = stringId;
        }

        uint16_t GetScenarioDescriptionStringId() const
        {
            auto& bytes = GetBytes();
            return *((uint16_t*)bytes.data() + 1);
        }

        void SetScenarioDescriptionStringId(uint16_t stringId) const
        {
            auto& bytes = GetBytes();
            *((uint16_t*)bytes.data() + 1) = stringId;
        }

    };

}

#endif
