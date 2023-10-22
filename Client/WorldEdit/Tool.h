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

void drawLine(mc::BufferBuilder* builder, mc::Vec3 pos1, mc::Vec3 pos2) {
    builder->vertex(pos1.x, pos1.y, pos1.z, true);
    builder->vertex(pos2.x, pos2.y, pos2.z, true);
}

void drawLineBox(mc::AABB box) {
    mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
    mc::GlStateManager::disableTexture();
    mc::GlStateManager::lineWidth(5);
    mc::GlStateManager::pushMatrix();
    builder->begin(MC_GL_LINES);

    // Bottom
    drawLine(builder, {box.min.x, box.min.y, box.min.z}, {box.max.x, box.min.y, box.min.z});
    drawLine(builder, {box.max.x, box.min.y, box.min.z}, {box.max.x, box.min.y, box.max.z});    
    drawLine(builder, {box.max.x, box.min.y, box.max.z}, {box.min.x, box.min.y, box.max.z});
    drawLine(builder, {box.min.x, box.min.y, box.max.z}, {box.min.x, box.min.y, box.min.z});

    // Top
    drawLine(builder, {box.min.x, box.max.y, box.min.z}, {box.max.x, box.max.y, box.min.z});
    drawLine(builder, {box.max.x, box.max.y, box.min.z}, {box.max.x, box.max.y, box.max.z});    
    drawLine(builder, {box.max.x, box.max.y, box.max.z}, {box.min.x, box.max.y, box.max.z});
    drawLine(builder, {box.min.x, box.max.y, box.max.z}, {box.min.x, box.max.y, box.min.z});

    // Lines from Buttom to Top
    drawLine(builder, {box.min.x, box.min.y, box.min.z}, {box.min.x, box.max.y, box.min.z});
    drawLine(builder, {box.max.x, box.min.y, box.min.z}, {box.max.x, box.max.y, box.min.z});
    drawLine(builder, {box.min.x, box.min.y, box.max.z}, {box.min.x, box.max.y, box.max.z});
    drawLine(builder, {box.max.x, box.min.y, box.max.z}, {box.max.x, box.max.y, box.max.z});
        
    builder->end();
    mc::GlStateManager::popMatrix();
}

class WorldEdit_Tool {
public:
    WorldEdit_Tool(int baseId) {
        id = baseId;
    }

    virtual void onZL(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) {}
    virtual void onZR(mc::ItemInstance* mainHand, mc::ItemInstance* offHand, mc::BlockPos hitPos) {}
    virtual void draw3D(mc::BlockPos hitPos, bool wasForced = false) {}   

    void teleportPacket(mc::Vec3 pos, float yaw = 0.0f, float pitch = 0.0f, bool onGround = true, bool flying = true) {
        mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
        listener->send(new mc::ServerboundMovePlayerPacket(pos.x, pos.y, pos.z, yaw, pitch, onGround, flying));
    }

    void breakBlock(mc::BlockPos pos, mc::ServerboundPlayerActionPacket::Action action, const mc::Direction* dir = mc::Direction::north) {
        mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
        listener->send(new mc::ServerboundPlayerActionPacket(action, pos, dir, 0));

        mc::Minecraft::getInstance()->thePlayer->lvl->setBlock(pos, mc::Block::byId(0)->defaultBlockState(), 3, false);
    }

    void placeBlock(mc::BlockPos pos, const mc::Direction* dir, mc::InteractionHand::EInteractionHand hand, mc::ItemInstance* offHand) {
        mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
        listener->send(new mc::ServerboundUseItemOnPacket(pos, dir, hand, 0, 0, 0));

        mc::Minecraft::getInstance()->thePlayer->lvl->setBlock(pos, mc::Block::byId(offHand->item->getId())->defaultBlockState(), 3, false);
    }

    int getId() {
        return id;
    }

    static constexpr double visualOffset = 0.05;

private:
    int id;
};

#endif