#ifndef __LIBCHK_OWNRCHUNK_H
#define __LIBCHK_OWNRCHUNK_H

#include <unordered_map>
#include <string>

#include "ichunk.h"
#include "serialization.h"

namespace CHK
{

    class CHKOwnrChunk : public IChunk
    {
        public:
        CHKOwnrChunk(const std::vector<char>& data, const std::string& type) : IChunk(type)
        {
            SetBytes(data);
        }

        void SetBytes(const std::vector<char>& data);

        PlayerType GetPlayerType(int playerId)
        {
            return (PlayerType)m_PlayerTypes[playerId];
        }

        void SetPlayerType(int playerId, PlayerType type);

        private:
        uint8_t m_PlayerTypes[12];
    };

}

#endif
