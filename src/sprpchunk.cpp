#include "sprpchunk.h"

namespace CHK
{

    void CHKSprpChunk::SetBytes(const std::vector<char>& data)
    {
        m_ScenarioStringId = *((uint16_t*)data.data());
        m_ScenarioDescriptionStringId = *((uint16_t*)(data.data() + sizeof(uint16_t)));
        IChunk::SetBytes(data);
    }

}
