#ifndef __CHK_H
#define __CHK_H

#include <unordered_map>
#include <memory>
#include <string>
#include <set>

#include "constants.h"
#include "ichunk.h"
#include "verchunk.h"
#include "strchunk.h"
#include "trigchunk.h"
#include "mrgnchunk.h"
#include "ownrchunk.h"
#include "iownchunk.h"
#include "dimchunk.h"
#include "uprpchunk.h"
#include "upuschunk.h"
#include "wavchunk.h"
#include "erachunk.h"
#include "sprpchunk.h"

namespace CHK
{

    class File
    {
        public:
        File(const std::vector<char>& data, bool stripExtraData)
        {
            auto i = 0u;
            while (i < data.size())
            {
                std::string type(data.data() + i, 4);
                i += 4;
                auto size = *((uint32_t*)(data.data() + i));
                i += 4;

                std::vector<char> bytes(data.data() + i, data.data() + i + size);
                i += size;

                std::unique_ptr<IChunk> chunk = nullptr;

                m_ChunkTypes.insert(type);

                if (type == "VER ")
                {
                    chunk = std::make_unique<VERChunk>(bytes, type);
                }
                else if (type == "STR ")
                {
                    chunk = std::make_unique<STRChunk>(bytes, type);
                }
                else if (type == "TRIG")
                {
                    chunk = std::make_unique<TRIGChunk>(bytes, type);
                }
                else if (type == "MRGN")
                {
                    chunk = std::make_unique<MRGNChunk>(bytes, type);
                }
                else if (type == "OWNR")
                {
                    chunk = std::make_unique<OWNRChunk>(bytes, type);
                }
                else if (type == "IOWN")
                {
                    chunk = std::make_unique<IOWNChunk>(bytes, type);
                }
                else if (type == "DIM ")
                {
                    chunk = std::make_unique<DIMChunk>(bytes, type);
                }
                else if (type == "UPRP")
                {
                    chunk = std::make_unique<UPRPChunk>(bytes, type);
                }
                else if (type == "UPUS")
                {
                    chunk = std::make_unique<UPUSChunk>(bytes, type);
                }
                else if (type == "WAV ")
                {
                    chunk = std::make_unique<WAVChunk>(bytes, type);
                }
                else if (type == "ERA ")
                {
                    chunk = std::make_unique<ERAChunk>(bytes, type);
                }
                else if (type == "SPRP")
                {
                    chunk = std::make_unique<SPRPChunk>(bytes, type);
                }
                else if
                    ( // skip all non-required chunks to lower file-size
                        type == "TYPE" ||
                        type == "IVER" ||
                        type == "IVE2" ||
                        type == "IOWN" ||
                        type == "UPGR" ||
                        type == "PTEC" ||
                        type == "ISOM" ||
                        type == "TILE" ||
                        type == "DD2 " ||
                        type == "UPUS" ||
                        type == "UNIS" ||
                        type == "UPGS" ||
                        type == "TECS"
                        )
                {
                    if (stripExtraData)
                    {
                        continue;
                    }
                    else
                    {
                        chunk = std::make_unique<IChunk>(bytes, type);
                    }
                }
                else
                {
                    chunk = std::make_unique<IChunk>(bytes, type);
                }

                if (chunk != nullptr)
                {
                    m_Chunks[type].push_back(std::move(chunk));
                }
            }
        }

        size_t GetSize() const
        {
            size_t size = 0;
            for (auto& pair : m_Chunks)
            {
                for (auto& chunk : pair.second)
                {
                    size += 8; // chunk id + chunk size
                    size += chunk->GetBytes().size();
                }
            }

            return size;
        }

        void Serialize(std::vector<char>& outBytes)
        {
            outBytes.resize(GetSize());

            size_t offset = 0u;

            for (auto& pair : m_Chunks)
            {
                for (auto& chunk : pair.second)
                {
                    memcpy(outBytes.data() + offset, chunk->GetType().c_str(), 4);
                    offset += 4;

                    auto& bytes = chunk->GetBytes();
                    auto size = (uint32_t)bytes.size();

                    memcpy(outBytes.data() + offset, &size, 4);
                    offset += 4;

                    memcpy(outBytes.data() + offset, bytes.data(), bytes.size());
                    offset += bytes.size();
                }
            }
        }

        void AddChunk(std::unique_ptr<IChunk> chunk)
        {
            m_ChunkTypes.insert(chunk->GetType());
            m_Chunks[chunk->GetType()].push_back(std::move(chunk));
        }

        bool HasChunk(ChunkType type) const
        {
            switch (type)
            {
            case ChunkType::DIM:
                return HasChunk("DIM ");
            case ChunkType::IOWN:
                return HasChunk("IOWN");
            case ChunkType::OWNR:
                return HasChunk("OWNR");
            case ChunkType::STR:
                return HasChunk("STR ");
            case ChunkType::WAV:
                return HasChunk("WAV ");
            case ChunkType::VER:
                return HasChunk("VER ");
            case ChunkType::TRIG:
                return HasChunk("TRIG");
            case ChunkType::ERA:
                return HasChunk("ERA ");
            case ChunkType::MRGN:
                return HasChunk("MRGN");
            case ChunkType::UPRP:
                return HasChunk("UPRP");
            case ChunkType::UPUS:
                return HasChunk("UPUS");
            case ChunkType::SPRP:
                return HasChunk("SPRP");
            }

            return false;
        }

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
                case ChunkType::DIM:
                    return GetFirstChunk<T>("DIM ");
                case ChunkType::IOWN:
                    return GetFirstChunk<T>("IOWN");
                case ChunkType::OWNR:
                    return GetFirstChunk<T>("OWNR");
                case ChunkType::STR:
                    return GetFirstChunk<T>("STR ");
                case ChunkType::WAV:
                    return GetFirstChunk<T>("WAV ");
                case ChunkType::VER:
                    return GetFirstChunk<T>("VER ");
                case ChunkType::TRIG:
                    return GetFirstChunk<T>("TRIG");
                case ChunkType::ERA:
                    return GetFirstChunk<T>("ERA ");
                case ChunkType::MRGN:
                    return GetFirstChunk<T>("MRGN");
                case ChunkType::UPRP:
                    return GetFirstChunk<T>("UPRP");
                case ChunkType::UPUS:
                    return GetFirstChunk<T>("UPUS");
                case ChunkType::SPRP:
                    return GetFirstChunk<T>("SPRP");
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
