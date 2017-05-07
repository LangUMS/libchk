#ifndef __LIBCHK_OWNRCHUNK_H
#define __LIBCHK_OWNRCHUNK_H

#include "ichunk.h"

namespace CHK
{

    class OWNRChunk : public IChunk
    {
        public:
        OWNRChunk(const std::vector<char>& data, const std::string& type) : IChunk(type)
        {
            SetBytes(data);
        }

        void SetBytes(const std::vector<char>& data)
        {
            IChunk::SetBytes(data);
            memcpy(m_PlayerTypes, data.data(), 12);
        }

        PlayerType GetPlayerType(int playerId)
        {
            return (PlayerType)m_PlayerTypes[playerId];
        }

        void SetPlayerType(int playerId, PlayerType type)
        {
            m_PlayerTypes[playerId] = (uint8_t)type;

            std::vector<char> bytes;
            bytes.resize(12);
            memcpy(bytes.data(), m_PlayerTypes, 12);
            IChunk::SetBytes(bytes);
        }

        private:
        uint8_t m_PlayerTypes[12];
    };

}

#endif
