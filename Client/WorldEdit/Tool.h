#ifndef TOOL_H
#define TOOL_H

#include <minecraft/mc.h>

mc::Vec3 getLookAtVec(float pitch, float yaw) {
    mc::Vec3 ret = {
        fabs(sinf(degToRad(pitch - 90.0f))) * sinf(degToRad(yaw   - 180.0f)),
                                              sinf(degToRad(pitch - 180.0f)),
        fabs(sinf(degToRad(pitch - 90.0f))) * cosf(degToRad(yaw))
    };

    return ret;
}

class WorldEdit_Tool {
public:
    WorldEdit_Tool(int baseId) {
        id = baseId;
    }

    virtual void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) {
        mc_printf(L"[WorldEdit] Place Holder ZL");
    }

    virtual void onZR(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) {
        mc_printf(L"[WorldEdit] Place Holder ZR");
    }

    virtual void draw3D(mc::BlockPos hitPos) {

    }   

    void teleportPacket(mc::Vec3 pos, float yaw = 0.0f, float pitch = 0.0f, bool onGround = true, bool flying = true) {
        mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
        listener->send(new mc::ServerboundMovePlayerPacket(pos.x, pos.y, pos.z, yaw, pitch, onGround, flying));
    }

    void breakBlock(mc::BlockPos pos, mc::ServerboundPlayerActionPacket::Action action, const mc::Direction* dir = mc::Direction::north) {
        mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
        listener->send(new mc::ServerboundPlayerActionPacket(action, pos, dir, 0));
    }

    void placeBlock(mc::BlockPos pos, const mc::Direction* dir, mc::InteractionHand::EInteractionHand hand) {
        mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
        listener->send(new mc::ServerboundUseItemOnPacket(pos, dir, hand, 0, 0, 0));
    }

    int getId() {
        return id;
    }

private:
    int id;
};

#endif