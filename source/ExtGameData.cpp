#include "ModuleData_ExtGameData.h"
#include "ExtGameDataUtil.h"

namespace {
    void extCreateChunks(ExtGameDataHolder *pHolder) {
        for (s32 i = 1; i < cExtSaveChunkCount; i++) {
            BinaryDataChunkBase *pChunk = cExtSaveChunkCreateTable[i]();

            if (pChunk) {
                // TODO Find a better way to do this
                u32 memberOffset = (u32)pHolder + sizeof(GameDataHolder) + 4 * (i - 1);
                *(u32 *)memberOffset = (u32)pChunk;
                OSReport("%p, %d\n", pChunk, sizeof(GameDataHolder) + 4 * (i - 1));
                pHolder->mBinaryDataChunkHolder->addChunk(pChunk);
            }
        }

        pHolder->resetAllData();
    }

}

kmWrite16(0x804D483A, 6 + cExtSaveChunkCount);
kmCall(0x804D488C, extCreateChunks);

kmWrite16(0x804DF00E, sizeof(ExtGameDataHolder));


void extResetAllData(ExtGameDataHolder* pHolder) {
    ((BinaryDataChunkBase*)pHolder->mEventValueChecker)->initializeData();
    BinaryDataChunkBase* pChunk = 0;
    u32 ptr = 0;
    for (s32 i = 1; i < cExtSaveChunkCount; i++) {
        ptr = *((u32*)pHolder+0xB+(i-1));
        ((BinaryDataChunkBase*)ptr)->initializeData();
    }
}

kmWrite32(0x804D4B6C, 0x7FE3FB78); // mr r3, r31
kmCall(0x804D4B70, extResetAllData);
kmWrite32(0x804D4B74, 0x4800000C); // b 0xC