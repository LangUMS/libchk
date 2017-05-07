#include <vector>

#include "chk.h"

namespace CHK
{

    File::File(const std::vector<char>& data, bool stripExtraData)
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

            if (type == "LANG")
            {
                chunk = std::make_unique<CHKLangChunk>(bytes, type);
            }
            else if (type == "VER ")
            {
                chunk = std::make_unique<CHKVerChunk>(bytes, type);
            }
            else if (type == "STR ")
            {
                chunk = std::make_unique<CHKStringsChunk>(bytes, type);
            }
            else if (type == "TRIG")
            {
                chunk = std::make_unique<CHKTriggersChunk>(bytes, type);
            }
            else if (type == "MRGN")
            {
                chunk = std::make_unique<CHKLocationsChunk>(bytes, type);
            }
            else if (type == "OWNR")
            {
                chunk = std::make_unique<CHKOwnrChunk>(bytes, type);
            }
            else if (type == "IOWN")
            {
                chunk = std::make_unique<CHKIOwnChunk>(bytes, type);
            }
            else if (type == "DIM ")
            {
                chunk = std::make_unique<CHKDimChunk>(bytes, type);
            }
            else if (type == "UPRP")
            {
                chunk = std::make_unique<CHKCuwpChunk>(bytes, type);
            }
            else if (type == "UPUS")
            {
                chunk = std::make_unique<CHKCuwpUsedChunk>(bytes, type);
            }
            else if (type == "WAV ")
            {
                chunk = std::make_unique<CHKWavChunk>(bytes, type);
            }
            else if (type == "ERA ")
            {
                chunk = std::make_unique<CHKTilesetChunk>(bytes, type);
            }
            else if (type == "SPRP")
            {
                chunk = std::make_unique<CHKSprpChunk>(bytes, type);
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

    size_t File::GetSize() const
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

    void File::Serialize(std::vector<char>& outBytes)
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

    void File::AddChunk(std::unique_ptr<IChunk> chunk)
    {
        m_ChunkTypes.insert(chunk->GetType());
        m_Chunks[chunk->GetType()].push_back(std::move(chunk));
    }

    bool File::HasChunk(ChunkType type) const
    {
        switch (type)
        {
        case ChunkType::DimChunk:
            return HasChunk("DIM ");
        case ChunkType::IOwnChunk:
            return HasChunk("IOWN");
        case ChunkType::OwnrChunk:
            return HasChunk("OWNR");
        case ChunkType::StringsChunk:
            return HasChunk("STR ");
        case ChunkType::WavChunk:
            return HasChunk("WAV ");
        case ChunkType::VerChunk:
            return HasChunk("VER ");
        case ChunkType::TriggersChunk:
            return HasChunk("TRIG");
        case ChunkType::TilesetsChunk:
            return HasChunk("ERA ");
        case ChunkType::LocationsChunk:
            return HasChunk("MRGN");
        case ChunkType::CuwpChunk:
            return HasChunk("UPRP");
        case ChunkType::CuwpUsedChunk:
            return HasChunk("UPUS");
        case ChunkType::LangumsChunk:
            return HasChunk("LANG");
        case ChunkType::SprpChunk:
            return HasChunk("SPRP");
        }

        return false;
    }

}
