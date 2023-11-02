stwu r1, -0x38(r1)
lis r9, 0x316
mflr r0
ori r9, r9, 0x6818
stw r0, 0x3c(r1)
mtctr r9
bctrl # Minecraft* minecraft = Minecraft::getInstance();

cmpwi r3, 0
beq 0x90 # if (!minecraft) return;

stw r30, 0x30(r1)
lwz r30, 0x34(r3) # LocalPlayer* thePlayer = minecraft->thePlayer;

cmpwi cr7, r30, 0
beq cr7, 0xac # if (!thePlayer) return;

lwz r10, 0x5f0(r30) # Inventory* inv = thePlayer->inventory;

lis r9, 0x248
ori r9, r9, 0x6698 # Item::byId();

stw r31, 0x34(r1)
lwz r10, 0x28(r10)
mtctr r9
li r3, 0x142 # Golden Apple / 332
lwz r10, 0(r10) # offHandItemIns = code::Mem(inventory->field_0x28)[0].as<ItemInstance*>();
lwz r31, 0x10(r10) # offHandItem = ItemInstance->item
bctrl # goldenAppleItem = Item::byId(332);

lis r10, 0x4e80
cmpw cr7, r3, r31 # if (offHandItem == goldenAppleItem) idk
lis r31, 0x31e
ori r31, r31, 0xa23c # LocalPlayer::startUsingItem(InteractionHand::EInteractionHand) = 0x031ea23c
ori r10, r10, 0x20
lwz r9, 0(r31)
beq cr7, 0xcc
cmpw cr7, r9, r10
beq cr7, 0x20c # End
lwz r30, 0x30(r1)
lwz r31, 0x34(r1)
lwz r0, 0x3c(r1)
lis r9, 0x10f
ori r9, r9, 0x6ae0
addi r1, r1, 0x38
mtctr r9
mtlr r0
bctr # return;

lwz r0, 0x3c(r1)
lis r9, 0x10f
lwz r30, 0x30(r1)
ori r9, r9, 0x6ae0
mtctr r9
mtlr r0
addi r1, r1, 0x38
bctr 
cmpw cr7, r9, r10
beq cr7, 0x1a8
stw r27, 0x24(r1)
li r27, 0
stw r27, 0x10(r1)
stw r29, 0x2c(r1)
stw r26, 0x20(r1)
stw r28, 0x28(r1)
lwz r29, 0x10(r1)
mr r3, r31
li r0, 0x200
sc 
mr r29, r3
stw r29, 0x10(r1)
lis r28, 0x31e
ori r28, r28, 0xa438
lwz r26, 0x10(r1)
stw r27, 0x14(r1)
lwz r29, 0x14(r1)
mr r3, r28
li r0, 0x200
sc 
mr r29, r3
stw r29, 0x14(r1)
li r28, 4
lwz r29, 0x14(r1)
mr r3, r26
mr r4, r29
mr r5, r28
li r0, 0x2500
sc 
li r9, 0x7e0
mtctr r9
dcbf 0, r31
addi r31, r31, 0x20
bdnz 0x150
lis r9, 0x31f
ori r9, r9, 0x9e1c
dcbf 0, r9
sync 0
eieio 
lis r9, 0x31e
li r10, 0x7e0
mtctr r10
ori r9, r9, 0xa23c
icbi 0, r9
addi r9, r9, 0x20
bdnz 0x180
sync 0
eieio 
isync 
lwz r26, 0x20(r1)
lwz r27, 0x24(r1)
lwz r28, 0x28(r1)
lwz r29, 0x2c(r1)
lis r9, 0x28b
li r4, 1
ori r9, r9, 0x2b8c
li r3, 0
mtctr r9
lwz r31, 0x878(r30)
bctrl 
lis r9, 0x304
ori r9, r9, 0xa5d8
li r10, 0
stw r3, 0x18(r1)
mtctr r9
stw r10, 0x1c(r1)
mr r3, r31
addi r4, r1, 0x18
bctrl 
lwz r0, 0x3c(r1)
lis r9, 0x10f
lwz r30, 0x30(r1)
lwz r31, 0x34(r1)
ori r9, r9, 0x6ae0
mtctr r9
mtlr r0
addi r1, r1, 0x38
bctr 
stw r28, 0x28(r1)
li r28, 0
stw r28, 8(r1)
stw r27, 0x24(r1)
stw r29, 0x2c(r1)
lwz r30, 8(r1)
mr r3, r31
li r0, 0x200
sc 
mr r30, r3
stw r30, 8(r1)
lis r29, 0x31e
ori r29, r29, 0xa444
lwz r27, 8(r1)
stw r28, 0xc(r1)
lwz r30, 0xc(r1)
mr r3, r29
li r0, 0x200
sc 
mr r30, r3
stw r30, 0xc(r1)
li r29, 4
lwz r30, 0xc(r1)
mr r3, r27
mr r4, r30
mr r5, r29
li r0, 0x2500
sc 
li r9, 0x7e0
mtctr r9
dcbf 0, r31
addi r31, r31, 0x20
bdnz 0x284
lis r9, 0x31f
ori r9, r9, 0x9e1c
dcbf 0, r9
sync 0
eieio 
lis r9, 0x31e
li r10, 0x7e0
mtctr r10
ori r9, r9, 0xa23c
icbi 0, r9
addi r9, r9, 0x20
bdnz 0x2b4
sync 0
eieio 
isync 
lis r9, 0x10f
lwz r0, 0x3c(r1)
lwz r27, 0x24(r1)
ori r9, r9, 0x6ae0
lwz r28, 0x28(r1)
mtctr r9
lwz r29, 0x2c(r1)
mtlr r0
lwz r30, 0x30(r1)
lwz r31, 0x34(r1)
addi r1, r1, 0x38
bctr 
nop