#pragma once

#include <code/code.h>

#include "../../client/resource/shared_ptr.h"

#include "../packet/DisconnectPacket.h"

namespace mc {
    class ServerGamePacketListenerImpl {
    public:
        void send(mc_boost::shared_ptr<Packet> packet) {
            code::Func<void, 0x03249FB4, ServerGamePacketListenerImpl*, mc_boost::shared_ptr<Packet>>()(this, packet);
        }

        void disconnect(DisconnectPacket::eDisconnectReason reason) {
            code::Func<void, 0x03288878, ServerGamePacketListenerImpl*, DisconnectPacket::eDisconnectReason>()(this, reason);
        }

        void teleport(double x, double y, double z, float yaw, float pitch, bool unk1, bool unk2) {
            code::Func<void, 0x03293e88, ServerGamePacketListenerImpl*, double, double, double, float, float, bool, bool>()(this, x, y, z, yaw, pitch, unk1, unk2);
        }
    };
}