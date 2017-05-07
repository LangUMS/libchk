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
            return m_ScenarioStringId;
        }

        uint16_t GetScenarioDescriptionStringId() const
        {
            return m_ScenarioDescriptionStringId;
        }

        void SetBytes(const std::vector<char>& data);

        private:
        uint16_t m_ScenarioStringId = 0;
        uint16_t m_ScenarioDescriptionStringId = 0;
    };

}

#endif
