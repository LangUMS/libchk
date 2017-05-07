#ifndef __LIBCHK_ERACHUNK_H
#define __LIBCHK_ERACHUNK_H

#include "ichunk.h"

namespace CHK
{
    
    enum class TilesetType
    {
        Badlands,
        SpacePlatform,
        Installation,
        Ashworld,
        Jungle,
        Desert,
        Arctic,
        Twilight,
    };

    class ERAChunk : public IChunk
    {
        public:
        ERAChunk(const std::vector<char>& data, const std::string& type) : IChunk(type)
        {
            SetBytes(data);
        }

        TilesetType GetTilesetType() const
        {
            auto& bytes = GetBytes();
            auto type = *((uint16_t*)bytes.data());
            return (TilesetType)(type & 7);
        }

        std::string GetTilesetTypeString() const
        {
            auto type = GetTilesetType();

            switch (type)
            {
            case TilesetType::Badlands:
                return "Badlands";
            case TilesetType::SpacePlatform:
                return "SpacePlatform";
            case TilesetType::Installation:
                return "Installation";
            case TilesetType::Ashworld:
                return "Ashworld";
            case TilesetType::Jungle:
                return "Jungle";
            case TilesetType::Desert:
                return "Desert";
            case TilesetType::Arctic:
                return "Arctic";
            case TilesetType::Twilight:
                return "Twilight";
            }

            return "UnknownTileset";
        }
    };

}

#endif
