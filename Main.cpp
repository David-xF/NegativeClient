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
#include "Client/XRay.h"

#include <xf/String.h>
#include <xf/Benchmark.h>

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

char n = 0;
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
}

int WriteCallback(char* contents, int size, int nmemb, xf::String<char>* userp) {
    for (int i = 0; i < (size * nmemb); i++) {
        userp->operator+=(contents[i]);
    }
    return size * nmemb;
}

DECL_HOOK(onFrameInMenu, void) {
    static mc::C4JThreadImpl* aimbotThread = nullptr;
    if (!aimbotThread) {
        aimbotThread = new mc::C4JThreadImpl([](void*) {
            while (true) {
                Aimbot::aim();
            }
            return 0;
        }, nullptr, "Aimbot Thread", 0x200);
        aimbotThread->Run();
        aimbotThread->SetDeleteOnExit(true);
    }

    Client::draw(true);
    Jesus::onTick();
}

DECL_FUNCTION(void, renderSky__13LevelRendererFf, void* renderer, float f) {
    if (CustomSky::draw()) real_renderSky__13LevelRendererFf(renderer, f);
    
    static bool wasCurlStarted = false;
    if (!wasCurlStarted) {
        void* curl = curl_easy_init();
        if (curl) {
            xf::String<char> t = "";
            curl_easy_setopt(curl, CURLOption::CURLOPT_URL, "http://chain-pushy-sesame.glitch.me/");
            curl_easy_setopt(curl, CURLOption::CURLOPT_WRITEFUNCTION, (void*) WriteCallback);
            curl_easy_setopt(curl, CURLOption::CURLOPT_USERAGENT, "curl/1.0");
            curl_easy_setopt(curl, CURLOption::CURLOPT_IDK, "cURL");
            curl_easy_setopt(curl, CURLOption::CURLOPT_FILE, &t);
            int res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            mc_printf(L"CURL Output: [%d][%s]", res, t.c_str());
        } else {
            mc_printf(L"CURL Error [curl_easy_init() failed]");
        }

        wasCurlStarted = true;
    }
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

DECL_FUNCTION(void, renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3, mc::Font* font, const mstd::wstring& name, float x, float y, float z, int idk, float yaw, float pitch, bool unk, bool unk1, int unk2, float unk3) {
    real_renderNameTagInWorld__12GameRendererSFP4FontRCQ2_3std78basic_string__tm__58_wQ2_3std20char_traits__tm__2_wQ2_3std18allocator__tm__2_wfN23iN23bT9T6T3(font, HealthIndicator::displayName(name), x, y, z, idk, yaw, pitch, unk, unk1, unk2, unk3);
}

DECL_FUNCTION(void, renderBlock__13BlockRendererFPC10BlockStateP5LevelRC8BlockPos, struct BlockRenderer* _this, struct BlockState* state, mc::Level* level, const mc::BlockPos& pos) {
    XRay::tesselateBlock(_this, state, level, pos);
}

int c_main(void*) {
    init();

    Client* client = new Client(L"Negative");
    Module* CombatPage = new Module(L"Combat", Module::Type::PAGE);
    Module* MovementPage = new Module(L"Movement", Module::Type::PAGE);
    Module* VisualPage = new Module(L"Visual", Module::Type::PAGE);
    Module* PlayerPage = new Module(L"Player", Module::Type::PAGE);
    Module* MiscPage = new Module(L"Miscellaneous", Module::Type::PAGE);
    client->addPage(CombatPage);
    client->addPage(MovementPage);
    client->addPage(VisualPage);
    client->addPage(PlayerPage);
    client->addPage(MiscPage);
    
    Module* CreditsModule = new Module(L"Credits", Module::Type::BUTTON);
    CreditsModule->setEvents(nullptr, nullptr, [](Module* mod) {
        mc_printf(L"[Credits] Made by david.xf");
        mc_printf(L"[Credits] Discord: https://discord.gg/xumaYBBhJv");
        mc_printf(L"[Credits] Client Name: denizdenizdenizdeniz");
        mc_printf(L"[Credits] Extra Credits: Miku666, Syoch, Chadderz, Inupong, BullyWiiPlaza");
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

    FriendsOfFriendsBypass* fofBypass = new FriendsOfFriendsBypass();
    MiscPage->addModuleToSettings(fofBypass->getModule());

    CustomChat* cChat = new CustomChat();
    VisualPage->addModuleToVector(cChat->getModule());

    XRay* xray = new XRay((void*) real_renderBlock__13BlockRendererFPC10BlockStateP5LevelRC8BlockPos);
    VisualPage->addModuleToVector(xray->getModule());

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
    REPLACE(0x0301DB14, renderBlock__13BlockRendererFPC10BlockStateP5LevelRC8BlockPos);
    writeMem(0x030FA014, 0x2C090001); // Enable Hitbox Visibility
    return 0;
}

void _main() {
    mc::C4JThreadImpl* thread = new mc::C4JThreadImpl(c_main, nullptr, "Loading Negative Client Assets", 0x200);
    thread->Run();
    thread->SetDeleteOnExit(true);
}