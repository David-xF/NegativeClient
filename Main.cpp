#include <code/code.h>

#include <code/tcpgecko.h>
#include <code/wups.h>

#include <exports/curl_functions.h>
#include <exports/socket_functions.h>
#include <exports/vpad_functions.h>
#include <exports/gx2_functions.h>
#include <exports/kernel.h>
#include <exports/os_functions.h>
#include <exports/memory.h>

#include <minecraft/mc.h>

#include "Client/Aimbot.h"
#include "Client/AntiKick.h"
#include "Client/ChestESP.h"
#include "Client/Client.h"
#include "Client/CustomChat.h"
#include "Client/CustomSky.h"
#include "Client/ESP.h"
#include "Client/Flight.h"
#include "Client/FofBypass.h"
#include "Client/HealthIndicator.h"
#include "Client/Jesus.h"
#include "Client/Keystrokes.h"
#include "Client/Killaura.h"
#include "Client/Module.h"
#include "Client/Scaffold.h"
#include "Client/SeeNameTags.h"
#include "Client/WorldChat.h"
#include "Client/WorldEdit.h"
#include "Client/XRay.h"

#include <xf/String.h>
#include <xf/Benchmark.h>
#include <xf/ItemInstanceHelper.h>

void init() {
    InitTCPGecko();
    InitWups();

    InitSocketFunctionPointers();
    InitKernelFunctionPointers();
    InitVPadFunctionPointers();
    InitMemoryFunctionPointers();
    InitOSFunctionPointers();
    InitGX2FunctionPointers();
    InitLibCurlFunctionPointers();

    memoryInitialize();
}

DECL_FUNCTION(void, renderHitbox__22EntityRenderDispatcherFRQ2_5boost25shared_ptr__tm__8_6EntitydN22fT5i, void* renderer, const mc_boost::shared_ptr<mc::Entity>& ref, uint32_t unk, float x, float y, float z, float a, float b) {
    ESP::draw(renderer, ref, unk, x, y, z, a, b);
}

DECL_HOOK(onFrameInGame, void) {
    Client::draw(false);
    KillAura::draw();
    Scaffold::onTick();
    KeyStrokes::draw();
    Jesus::onTick();
    CustomChat::drawChat();
    WorldEdit::onFrame();

    mc::Minecraft::getInstance()->gui->setSingleMessage(L"Negative Client - Test", 0, 0);
}

DECL_HOOK(onFrameInMenu, void) {
    static mc::C4JThreadImpl* aimbotThread = nullptr;
    if (!aimbotThread) {
        aimbotThread = new mc::C4JThreadImpl([](void*) {
            while (true) Aimbot::aim();

            return 0;
        }, nullptr, "Negative Client - General Purpose Thread", 0x200);
        aimbotThread->Run();
        aimbotThread->SetDeleteOnExit(true);
    }

    Client::draw(false);
    Jesus::onTick();
}

DECL_FUNCTION(void, renderSky__13LevelRendererFf, void* renderer, float f) {    
    if (CustomSky::draw()) real_renderSky__13LevelRendererFf(renderer, f);
}

DECL_FUNCTION(void, tick__11LocalPlayerFv, mc::LocalPlayer* lPlayer) {
    Flight::onTick(true);
    KillAura::onTick(true);
    real_tick__11LocalPlayerFv(lPlayer);
    KillAura::onTick(false);
    Flight::onTick(false);
    Aimbot::selectEntity();
    Scaffold::onTick();
}

DECL_FUNCTION(void, handleDisconnect__20ClientPacketListenerFQ2_5boost37shared_ptr__tm__19_16DisconnectPacket, mc::ClientPacketListener* listener, const mc_boost::shared_ptr<mc::DisconnectPacket>& packet) {
    if (AntiKick::onKick(packet->reason)) real_handleDisconnect__20ClientPacketListenerFQ2_5boost37shared_ptr__tm__19_16DisconnectPacket(listener, packet);
}

DECL_FUNCTION(void, sendPosition__11LocalPlayerFv, mc::LocalPlayer* lPlayer) {
    Scaffold::updatePos(true);
    real_sendPosition__11LocalPlayerFv(lPlayer);
    Scaffold::updatePos(false);
}

DECL_HOOK(ChestRenderer_render, void* renderer, const mc_boost::shared_ptr<mc::BlockEntity>& entity, double x, double y, double z) {
    ChestESP::draw(x, y, z);
}

DECL_FUNCTION(uint32_t, getClipAABB__11LiquidBlockFPC10BlockStateP11LevelSourceRC8BlockPos, void) {
    Jesus* jesus = (Jesus*) staticJesus;
    return jesus->getPtr()[0];
}

DECL_FUNCTION(bool, IsInPublicJoinableGame__19CGameNetworkManagerFv, void* _this) {
    FriendsOfFriendsBypass* fofBypass = (FriendsOfFriendsBypass*) staticFofBypass;
    if (fofBypass->getModule()->getState()) {
        return true;
    } else {
        return real_IsInPublicJoinableGame__19CGameNetworkManagerFv(_this);
    }
}

DECL_FUNCTION(void, addMessage__3GuiFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wibN33, mc::Gui* gui, const mstd::wstring& message, int unk, bool unk1, bool unk2, bool unk3, bool unk4) {
    bool isDisabled = CustomChat::addMessageS(message.c_str());
    if (isDisabled) real_addMessage__3GuiFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wibN33(gui, message, unk, unk1, unk2, unk3, unk4);
}

/*
 * MUST BE DONE LIKE THIS IMPLEMENTATION OR THIS ERROR WILL APPEAR (GOT FIXED BUT IM TOO LAZY TO CHANGE IT :3)
 * C:\Users\David\AppData\Local\Temp\ccoEOp99.s:2025: Error: symbol `_renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3Start' is already defined
 * C:\Users\David\AppData\Local\Temp\ccoEOp99.s:2097: Error: symbol `_renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3' is already defined
 * C:\Users\David\AppData\Local\Temp\ccoEOp99.s:2130: Error: symbol `_renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3Found' is already defined
 */
DECL_FUNCTION(void, renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3, mc::Font* font, const mstd::wstring& name, float x, float y, float z, int idk, float yaw, float pitch, bool unk, bool unk1, int unk2, float unk3) {    
    mc::Player* player = HealthIndicator::getPlayer(name);
    if (!player) {
        real_renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3(font, name, x, y, z, idk, yaw, pitch, unk, unk1, unk2, unk3);
    } else {
        float newY[] = {
            y, y + 0.3f, y
        };

        int mode = HealthIndicator::getMode();
        if (mode == 0) { // Inline
            real_renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3(font, HealthIndicator::displayName(name).c_str(), x, y, z, idk, yaw, pitch, unk, unk1, unk2, unk3);
        } else {
            real_renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3(font, name, x, newY[mode - 1], z, idk, yaw, pitch, unk, unk1, unk2, unk3);
            real_renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3(font, HealthIndicator::getHP(player).c_str(), x, newY[mode], z, idk, yaw, pitch, unk, unk1, unk2, unk3);
        }
    }
}

DECL_FUNCTION(void, tesselateBlockInWorldWithAmbienceOcclusionTexLighting__13BlockRendererFPC10BlockStateRC8BlockPosfN23ib, void* _this, void* state, const mc::BlockPos& pos, float unk_0, float unk_1, float unk_2, int unk_3, bool unk_4) {
    XRay::tesselateBlock(_this, state, pos, unk_0, unk_1, unk_2, unk_3, unk_4);
}

DECL_FUNCTION(bool, renderDebug__9MinecraftSFv) {
    return ((SeeNameTags*) staticSeeNameTags)->getModule()->getState() ? true : real_renderDebug__9MinecraftSFv();
}

// When Not See through, disable hasShadow (Weird Visual Glitch)
void _displayText(mc::Vec3 pos, xf::String<wchar_t> str, uint32_t color, float size, bool hasShadow, bool seeThroughWalls = false, bool followLocalPlayer = false) {
    mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
    code::Func<void, 0x03181508>()(); // Lighting::turnOff(void)
    mc::GlStateManager::disableCull();
    if (seeThroughWalls) mc::GlStateManager::disableDepthTest();
    else                 mc::GlStateManager::enableDepthTest();
    mc::GlStateManager::enableBlend();
    mc::GlStateManager::blendFunc(4, 5);

    mc::GlStateManager::color4f(1, 1, 1, 1);
    mc::GlStateManager::enableTexture();
    mc::GlStateManager::pushMatrix();
    double fx = pos.x - code::Mem(0x104CAA18).as<double>();
    double fy = pos.y - code::Mem(0x104CAA20).as<double>();
    double fz = pos.z - code::Mem(0x104CAA28).as<double>();
    mc::GlStateManager::translatef(fx, fy, fz);
    if (followLocalPlayer) {
        mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
        double diffX = pos.x - lPlayer->position.x;
        double diffY = pos.y - (lPlayer->position.y + lPlayer->getEyeHeight());
        double diffZ = pos.z - lPlayer->position.z;
        float dist = sqrtf(powf(diffX, 2) + powf(diffZ, 2));
        float yaw = (float) atan2(diffZ, diffX) * (180.0f / M_PI) - 90.0f;
        float pitch = (float) atan2(diffY, dist) * (180.0f / M_PI);
        mc::GlStateManager::rotatef(180.0f - yaw, 0.0f, 1.0f, 0.0f);
        mc::GlStateManager::rotatef(pitch,        1.0f, 0.0f, 0.0f);
    }
    float lsize = (mc::toFloat(font->width(str.c_str())) * size) / 2.0f;
    float lheight = (mc::toFloat(FONT_CHAR_HEIGHT) * size) / 2.0f;
    mc::GlStateManager::translatef(-lsize, lheight, 0.0f);
    mc::GlStateManager::scalef(size, -size, 1);
    if (hasShadow) font->drawShadow(str.c_str(), 0, 0, color);
    else           font->draw      (str.c_str(), 0, 0, color);
    mc::GlStateManager::popMatrix();
    mc::GlStateManager::disableBlend();
    mc::GlStateManager::enableDepthTest();
    code::Func<void, 0x0317a08c>()(); // Lighting::turnOn(void)
}

DECL_FUNCTION(void, renderEntities__13LevelRendererFQ2_5boost25shared_ptr__tm__8_6EntityP6Cullerf, void* c, const mc_boost::shared_ptr<mc::Entity>& entity, void* b, float a) {
    real_renderEntities__13LevelRendererFQ2_5boost25shared_ptr__tm__8_6EntityP6Cullerf(c, entity, b, a);
    
    mc::GlStateManager::disableCull();
    mc::GlStateManager::disableLighting();
    mc::GlStateManager::enableDepthTest();
    mc::GlStateManager::enableBlend();
    mc::GlStateManager::blendFunc(4, 5);
    mc::GlStateManager::disableFog();
    WorldEdit::render3D();
    mc::GlStateManager::disableBlend();
    mc::GlStateManager::enableCull();
}

DECL_FUNCTION(void, handleChat__28ServerGamePacketListenerImplFQ2_5boost42shared_ptr__tm__24_21ClientboundChatPacket, mc::ServerGamePacketListenerImpl* listener, const mc_boost::shared_ptr<mc::ClientboundChatPacket>& packet) {
    WorldChat::handleChat(listener, packet);
}

int c_main(void*) {
    init();

    Client* client = new Client(L"Negative");
    Module* CombatPage = new Module(L"Combat", Module::Type::PAGE);
    Module* MovementPage = new Module(L"Movement", Module::Type::PAGE);
    Module* VisualPage = new Module(L"Visual", Module::Type::PAGE);
    Module* PlayerPage = new Module(L"Player", Module::Type::PAGE);
    Module* MiscPage = new Module(L"Miscellaneous", Module::Type::PAGE);
    Module* ClientPage = new Module(L"Client", Module::Type::PAGE);
    client->addPage(CombatPage);
    client->addPage(MovementPage);
    client->addPage(VisualPage);
    client->addPage(PlayerPage);
    client->addPage(MiscPage);
    client->addPage(ClientPage);
    
    Module* CreditsModule = new Module(L"Credits", Module::Type::BUTTON);
    CreditsModule->setEvents(nullptr, nullptr, [](Module* mod) {
        mc_printf(L"[Credits] Made by david.xf");
        mc_printf(L"[Credits] Discord: https://discord.gg/xumaYBBhJv");
        mc_printf(L"[Credits] Client Name: denizdenizdenizdeniz");
        mc_printf(L"[Credits] Extra Credits: Miku666, Syoch, Chadderz, Inupong, BullyWiiPlaza, Yuki (literallyxyuki)");
    });
    MiscPage->addModuleToVector(CreditsModule);
    
    // Will move this Into a Seperate Header file
    // With Settings like: Toggle Specific Item and/or Item Count (Golden Apple/Arrow)
    Module* InstantCraft = new Module(L"Instant Craft", Module::Type::BUTTON);
    InstantCraft->setEvents(nullptr, nullptr, [](Module* mod) {
        mc::ClientPacketListener* listener = mc::Minecraft::getInstance()->getConnection(0);
        mstd::pair<int, int> itemsToCraft[] = {
            {412,  1}, // Diamond Sword
            {368,  1}, // Bow
            { 42, 64}, // Golden Apple
            {172,  1}, // Diamond Helmet
            {173,  1}, // Diamond Chestplate
            {174,  1}, // Diamond Leggings
            {175,  1}, // Diamond Boots
            {407,  8}  // Strength 2 Arrows
        };

        for (mstd::pair<int, int> pair : itemsToCraft) {
            for (int i = 0; i < pair.t2; i++) {
                listener->send(new mc::CraftItemPacket(pair.t1, 0, 0));
            }
        }

        mc::ConsoleUIController::getInstance()->PlayUISFX(mc::SoundEvent::block_chest_open);
    });
    PlayerPage->addModuleToVector(InstantCraft);

    ESP* esp = new ESP();
    VisualPage->addModuleToVector(esp->getModule());

    ChestESP* chestEsp = new ChestESP();
    VisualPage->addModuleToVector(chestEsp->getModule());

    CustomSky* customSky = new CustomSky();
    VisualPage->addModuleToVector(customSky->getModule());

    Flight* flight = new Flight();
    MovementPage->addModuleToVector(flight->getModule());

    Aimbot* aimbot = new Aimbot();
    CombatPage->addModuleToVector(aimbot->getModule());

    AntiKick* antiKick = new AntiKick();
    PlayerPage->addModuleToVector(antiKick->getModule());

    KillAura* killAura = new KillAura();
    CombatPage->addModuleToVector(killAura->getModule());

    Jesus* jesus = new Jesus();
    MovementPage->addModuleToVector(jesus->getModule());

    Scaffold* scaffold = new Scaffold();
    MovementPage->addModuleToVector(scaffold->getModule());

    KeyStrokes* strokes = new KeyStrokes();
    VisualPage->addModuleToVector(strokes->getModule());

    HealthIndicator* healthIndicator = new HealthIndicator();
    VisualPage->addModuleToVector(healthIndicator->getModule());

    Module* reloadTexture = new Module(L"Reload Textures", Module::Type::BUTTON);
    reloadTexture->setEvents(nullptr, nullptr, [](Module* mod) {
        mc::Minecraft::getInstance()->textures->reloadall();
    });
    MiscPage->addModuleToVector(reloadTexture);

    Module* changeNNID = new Module(L"Change NNID", Module::Type::BUTTON);
    changeNNID->setEvents(nullptr, nullptr, [](Module* mod) {
        wchar_t* nnidAddr = ((wchar_t*) ((uint32_t) code::Mem(0x104CCB18).as<wchar_t*>() + 0x50)); // Miku666
        mc::CInput::GetInput()->RequestKeyboard(L"", nnidAddr, 0, 16, [](void* data, bool b) {
            wchar_t temp[17];
            mc::CInput::GetInput()->GetText(temp, 17);
            
            mc_swprintf((wchar_t*) data, 17, L"%ls", temp);

            mc_printf(L"Set NNID to: %ls", temp);
            return 0;
        }, nnidAddr, 0);
    });
    PlayerPage->addModuleToVector(changeNNID);

    Module* OpSkull = new Module(L"OP Skull", Module::Type::BUTTON);
    OpSkull->setEvents(nullptr, nullptr, [](Module*) {
        mc::ItemInstance* item = new mc::ItemInstance(mc::Item::byId(397), 1, 1);
        xf::ItemInstanceHelper::addEnchant(item, 0, 10); // Protection
        xf::ItemInstanceHelper::addEnchant(item, 7, 32767); // Thorns
        xf::ItemInstanceHelper::addEnchant(item, 10, 1); // Binding 
        xf::ItemInstanceHelper::addEnchant(item, 71, 1); // Vanishing

        xf::ItemInstanceHelper::addAttrib(item, 0, 9E12, 0, L"head");
        xf::ItemInstanceHelper::addAttrib(item, 2, 9E12, 0, L"head");
        xf::ItemInstanceHelper::addAttrib(item, 4, 9E12, 0, L"head");

        xf::ItemInstanceHelper::setName(item, L"Negative Client - OP Skull");

        mc_boost::shared_ptr<mc::Packet> packet(new mc::ServerboundSetCreativeModeSlotPacket(5, item));
        mc::Minecraft::getInstance()->getConnection(0)->send(packet);
    });
    PlayerPage->addModuleToVector(OpSkull);

    Module* SwapHands = new Module(L"Swap Hands", Module::Type::BUTTON);
    SwapHands->setEvents(nullptr, nullptr, [](Module*) {
        mc::Minecraft* minecraft = mc::Minecraft::getInstance();
        mc_boost::shared_ptr<mc::Packet> packet(new mc::ServerboundPlayerActionPacket(mc::ServerboundPlayerActionPacket::Action::SWAP_HANDS, minecraft->thePlayer->pos1, mc::Direction::north, 0));
        minecraft->getConnection(0)->send(packet);
    });
    PlayerPage->addModuleToVector(SwapHands);

    WorldChat* worldChat = new WorldChat();
    MiscPage->addModuleToVector(worldChat->getModule());

    FriendsOfFriendsBypass* fofBypass = new FriendsOfFriendsBypass();
    PlayerPage->addModuleToVector(fofBypass->getModule());

    CustomChat* cChat = new CustomChat();
    VisualPage->addModuleToVector(cChat->getModule());

    XRay* xray = new XRay((void*) exec_tesselateBlockInWorldWithAmbienceOcclusionTexLighting__13BlockRendererFPC10BlockStateRC8BlockPosfN23ib);
    VisualPage->addModuleToVector(xray->getModule());

    SeeNameTags* nameTags = new SeeNameTags();
    VisualPage->addModuleToVector(nameTags->getModule());

    WorldEdit* worldEdit = new WorldEdit();
    MiscPage->addModuleToVector(worldEdit->getModule());

    HOOK(0x02D9CAD0, onFrameInGame, 0);
    HOOK(0x02D9C8B0, onFrameInMenu, 0);
    HOOK(0x02FE3224, ChestRenderer_render, 0);
    REPLACE(0x031BC7E0, renderSky__13LevelRendererFf);
    REPLACE(0x031e55d4, sendPosition__11LocalPlayerFv);
    REPLACE(0x030F9784, renderHitbox__22EntityRenderDispatcherFRQ2_5boost25shared_ptr__tm__8_6EntitydN22fT5i);
    REPLACE(0x031e3a80, tick__11LocalPlayerFv);
    REPLACE(0x03052720, handleDisconnect__20ClientPacketListenerFQ2_5boost37shared_ptr__tm__19_16DisconnectPacket);
    REPLACE(0x025A963C, getClipAABB__11LiquidBlockFPC10BlockStateP11LevelSourceRC8BlockPos);
    REPLACE(0x02D5731C, IsInPublicJoinableGame__19CGameNetworkManagerFv);
    REPLACE(0x0313873c, addMessage__3GuiFRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wibN33);
    REPLACE(0x030e9c14, renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3);
    REPLACE(0x02FED918, tesselateBlockInWorldWithAmbienceOcclusionTexLighting__13BlockRendererFPC10BlockStateRC8BlockPosfN23ib);
    REPLACE(0x031B2B24, renderDebug__9MinecraftSFv);
    REPLACE(0x031B8298, renderEntities__13LevelRendererFQ2_5boost25shared_ptr__tm__8_6EntityP6Cullerf);
    REPLACE(0x032E68A4, handleChat__28ServerGamePacketListenerImplFQ2_5boost42shared_ptr__tm__24_21ClientboundChatPacket);
    writeMem(0x030FA014, 0x2C090001); // Enable Hitbox Visibility
    return 0;
}

void _main() {
    mc::C4JThreadImpl* thread = new mc::C4JThreadImpl(c_main, nullptr, "Negative Client - Setup", 0x200);
    thread->Run();
    thread->SetDeleteOnExit(true);
}