#ifndef IRON_AXE_TOOL_H
#define IRON_AXE_TOOL_H

#include <minecraft/mc.h>

#include <xf/Vector.h>

#include "Tool.h"

#define FOR_SPHERE_PICK_AXE(name) for (int name = min_##name; name < max_##name; name++)

class WorldEdit_IronAxe : public WorldEdit_Tool {
public:
    struct blockData {
        int id; // Item ID
        int aux; // Item Damage
        int count; // Count for how many for a row there is
    };  

    struct placeData {
        int aux;
        mc::Direction* dir;
        mc::Direction* pitch;
        float yaw;
    };

    WorldEdit_IronAxe(mc::BlockPos* _pos1, mc::BlockPos* _pos2) : WorldEdit_Tool(itemId) {
        pos1 = _pos1;
        pos2 = _pos2;

        sizeX = -1;
        sizeY = -1;
        sizeZ = -1;

        _blockData = xf::Vector<blockData>();
    }

    // Copy
    void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        if (pos1->y == -1 || pos2->y == -1) return;

        int min_x = we_min(pos1->x, pos2->x);
        int min_y = we_min(pos1->y, pos2->y);
        int min_z = we_min(pos1->z, pos2->z);
        int max_x = we_max(pos1->x, pos2->x) + 1;
        int max_y = we_max(pos1->y, pos2->y) + 1;
        int max_z = we_max(pos1->z, pos2->z) + 1;   

        sizeX = 0;
        sizeY = 0;
        sizeZ = 0;

        FOR_SPHERE_PICK_AXE(x) sizeX++;
        FOR_SPHERE_PICK_AXE(y) sizeY++;
        FOR_SPHERE_PICK_AXE(z) sizeZ++;

        _blockData.clear();

        FOR_SPHERE_PICK_AXE(x) {
            FOR_SPHERE_PICK_AXE(y) {
                FOR_SPHERE_PICK_AXE(z) {
                    mc::Level* level = mc::Minecraft::getInstance()->thePlayer->lvl;
                    int id = level->getBlockId(x, y, z);
                    int aux = level->getBlockData(x, y, z);
                    if (_blockData.getSize() > 0) {
                        blockData* data = &_blockData[_blockData.getSize() - 1];
                        if (data->id == id && data->aux == aux) data->count++;
                        else _blockData.push_back({id, aux, 1});
                    } else {
                        _blockData.push_back({id, aux, 1});
                    }
                }
            }
        }
    }

    placeData getPlaceData(blockData* data) {
        auto dirToYaw = [](mc::Direction* dir) -> float {
            if (dir == mc::Direction::north) return   0 - 90; // North = x++
            if (dir == mc::Direction::east)  return  90 - 90; // East = z++
            if (dir == mc::Direction::south) return 180 - 90; // South = x--
            if (dir == mc::Direction::west)  return 270 - 90; // West = z--
            return 0;
        };
        
        auto stairs = [dirToYaw](blockData* data) -> placeData {
            mc::Direction* pitch = data->aux >= 4 ? mc::Direction::down : mc::Direction::up;
            mc::Direction* dirs[4] = {
                mc::Direction::north, mc::Direction::south, mc::Direction::east, mc::Direction::west
            };

            int aux = data->aux >= 4 ? (data->aux - 4) : data->aux;
            return {0, dirs[aux], pitch, dirToYaw(dirs[aux])};
        };

        switch (data->id) {
        case 53: 
            return stairs(data);
        case 134: 
            return stairs(data);
        case 135: 
            return stairs(data);
        case 136: 
            return stairs(data);
        case 163: 
            return stairs(data);
        case 164: 
            return stairs(data);
        case 67: 
            return stairs(data);
        case 108: 
            return stairs(data);
        case 109: 
            return stairs(data);
        case 114: 
            return stairs(data);
        case 128: 
            return stairs(data);
        case 180: 
            return stairs(data);
        case 156: 
            return stairs(data);
        case 203: 
            return stairs(data);
        case 291: // 1059 
            return stairs(data);
        case 292: // 1060 
            return stairs(data);
        case 293: // 1061 
            return stairs(data);
        default:
            return {-1, mc::Direction::north, mc::Direction::down, 0};
        }
    }

    // Paste
    void onZR(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) override {
        if (hitPos.y == -1) return;
        if (sizeX == -1 || sizeY == -1 || sizeZ == -1) return;
        
        xf::Vector<blockData> copyData = _blockData;

        blockData lastGiven = {0, 0, 0};

        for (int x = 0; x < sizeX; x++) {
            for (int y = 0; y < sizeY; y++) {
                for (int z = 0; z < sizeZ; z++) {
                    mc::BlockPos finalPos(x + hitPos.x - (sizeX / 2), y + hitPos.y, z + hitPos.z - (sizeZ / 2));
                    blockData* currData = &copyData.front();
                    if (currData->count == 0) {
                        copyData.pop_front();
                        currData = &copyData.front();
                    }
                    currData->count--;
                    
                    mc::Level* level = mc::Minecraft::getInstance()->thePlayer->lvl;
                    int id = level->getBlockId(finalPos.x, finalPos.y, finalPos.z);
                    int aux = level->getBlockData(finalPos.x, finalPos.y, finalPos.z);
                    
                    bool shouldBeBroken = false;
                    bool shouldBePlaced = false;
                    if (currData->id == 0 && id != 0) {
                        shouldBeBroken = true;
                    } else if (currData->id != id || currData->aux != aux) {
                        shouldBeBroken = true;
                        shouldBePlaced = true;
                    }

                    placeData _placeData = getPlaceData(currData);
                    if (shouldBeBroken || shouldBePlaced) teleportPacket({mc::toFloat(finalPos.x + 2), mc::toFloat(finalPos.y + 2), mc::toFloat(finalPos.z + 2)}, _placeData.yaw, _placeData.pitch == mc::Direction::up ? 90.0f : -90.0f);
                    if (shouldBeBroken) breakBlock(finalPos, mc::ServerboundPlayerActionPacket::Action::START_BREAKING);
                    if (shouldBePlaced) {
                        if (currData->id != lastGiven.id || currData->aux != lastGiven.aux) {
                            mc::Minecraft::getInstance()->getConnection(0)->send(new mc::ServerboundSetCreativeModeSlotPacket(45, new mc::ItemInstance(mc::Item::byId(currData->id), 1, _placeData.aux == -1 ? currData->aux : _placeData.aux)));
                        }
                        placeBlock(finalPos, _placeData.pitch, mc::InteractionHand::OFF_HAND);
                    }
                }
            }
        }
    }

    void draw3D(mc::BlockPos hitPos, bool wasForced = false) override {
        if (hitPos.y == -1) return;
        if (sizeX == -1 || sizeY == -1 || sizeZ == -1) return;

        
    }   

    static constexpr int itemId = 258;

private:
    mc::BlockPos* pos1;
    mc::BlockPos* pos2;
    xf::Vector<blockData> _blockData;
    int sizeX;
    int sizeY;
    int sizeZ;
};

#endif