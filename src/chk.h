#ifndef __CHK_H
#define __CHK_H

#include <unordered_map>
#include <memory>
#include <string>
#include <set>

#include "constants.h"
#include "ichunk.h"
#include "verchunk.h"
#include "stringschunk.h"
#include "triggerschunk.h"
#include "locationschunk.h"
#include "ownrchunk.h"
#include "iownchunk.h"
#include "dimchunk.h"
#include "cuwpchunk.h"
#include "cuwpusedchunk.h"
#include "wavchunk.h"
#include "tilesetchunk.h"
#include "langumschunk.h"

namespace CHK
{

    class File
    {
        public:
        File(const std::vector<char>& data, bool stripExtraData);

        size_t GetSize() const;
        void Serialize(std::vector<char>& outBytes);

        void AddChunk(std::unique_ptr<IChunk> chunk);
        bool HasChunk(ChunkType type) const;

        size_t GetChunkCount() const
        {
            return m_Chunks.size();
        }

        const std::set<std::string>& GetChunkTypes() const
        {
            return m_ChunkTypes;
        }

        template <typename T>
        T* GetFirstChunk(ChunkType type) const
        {
            switch (type)
            {
                case ChunkType::DimChunk:
                    return GetFirstChunk<T>("DIM ");
                case ChunkType::IOwnChunk:
                    return GetFirstChunk<T>("IOWN");
                case ChunkType::OwnrChunk:
                    return GetFirstChunk<T>("OWNR");
                case ChunkType::StringsChunk:
                    return GetFirstChunk<T>("STR ");
                case ChunkType::WavChunk:
                    return GetFirstChunk<T>("WAV ");
                case ChunkType::VerChunk:
                    return GetFirstChunk<T>("VER ");
                case ChunkType::TriggersChunk:
                    return GetFirstChunk<T>("TRIG");
                case ChunkType::TilesetsChunk:
                    return GetFirstChunk<T>("ERA ");
                case ChunkType::LocationsChunk:
                    return GetFirstChunk<T>("MRGN");
                case ChunkType::CuwpChunk:
                    return GetFirstChunk<T>("UPRP");
                case ChunkType::CuwpUsedChunk:
                    return GetFirstChunk<T>("UPUS");
                case ChunkType::LangumsChunk:
                    return GetFirstChunk<T>("LANG");
            }

            return nullptr;
        }

        template <typename T>
        std::vector<T*> GetChunks(const std::string& type)
        {
            std::vector<IChunk> chunks;

            auto it = m_Chunks.find(type);
            if (it == m_Chunks.end())
            {
                return chunks;
            }

            for (auto& chunk : (*it).second)
            {
                chunks.push_back((T*)chunk.get());
            }

            return chunks;
        }

        private:
        bool HasChunk(const std::string& type) const
        {
            return m_Chunks.find(type) != m_Chunks.end();
        }

        template <typename T>
        T* GetFirstChunk(const std::string& type) const
        {
            auto it = m_Chunks.find(type);
            if (it == m_Chunks.end())
            {
                return nullptr;
            }

            return (T*)(*it).second.front().get();
        }

        std::unordered_map<std::string, std::vector<std::unique_ptr<IChunk>>> m_Chunks;
        std::set<std::string> m_ChunkTypes;
    };

}

#endif
