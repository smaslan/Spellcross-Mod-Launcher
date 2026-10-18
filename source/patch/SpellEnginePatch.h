#pragma once

/**
 * =================================================================================================
 * Spellcross Engine Patch V1.20 - C++ Integration Module
 * =================================================================================================
 * Compatibility: SPELCROS.EXE (1,632,112 bytes)
 * Architecture: x86 / DOS / DOS4GW / LE (Linear Executable)
 * Author: HonzaQ & AI Assistant (2026-10-17)
 *
 * This module applies the complete unified engine modifications:
 *   1. Extended limits V1.0:
 *      - Extended unit limits (90 -> 127 units)
 *      - Extended sound definitions (A_SOUNDS: 90 -> 127, Z_SOUNDS: 48 -> 80, H_SOUNDS: 28 -> 50)
 *      - Extended upgrades & workshop (36 -> 72 upgrades, 142 -> 180 byte stride)
 *      - Expanded save/load state buffer (29,089 bytes payload, Option B)
 *      - Expanded UNITS.FSU TOC (220 KB -> 1 MB) and .FS TOC (2,600 -> 4,096 files)
 *      - Expanded sprite/frame cache buffers (64 KB each)
 *      - Dynamic MainMenu version overlay with centering & adaptive background box
 *   2. Selective panic & entrenchment floor (Panika V5):
 *      - Restored 2x multiplier for prolonged entrenchment (>22 turns)
 *      - Enforced hard floor clamping to entrenchment limit instead of 0
 *      - Hard difficulty entrenchment limit set to 20% (0x14)
 *      - Panic threshold raised to 14 (0x0E) across all 4 engine checks
 *   3. Group selection and tactical map clicking lock (Vyber skupin V1.14):
 *      - Code Cave at VA 0x00054D16 checking motion/combat animation states
 *      - Hook 1 at VA 0x0001A361 silently discarding keys 1-8 during unit movement
 *      - Hook 2 at VA 0x000210EE silently discarding map clicks during unit movement
 *      - UI buttons (Next turn, Info, Menu) remain fully responsive
 *   4. Convoy / Slot 49 selection & grouping unlock (Konvoj):
 *      - Fixes off-by-one check (cmp [ebp-0x1b0], 50 -> 51) at VA 0x00021348, 0x00021379,
 *        0x000213A3, and 0x000213D4 so player slot 49 (map ID 50, 2nd convoy) can be
 *        selected and assigned to groups
 *   5. Defense bypass bit flags in JEDNOTKY.DEF byte 0x48 (Protitankovy orkove):
 *      - Replaces hardcoded Anti-tank Orcs (unit_type_id == 0x28) check at VA 0x0007C748
 *        with data-driven bit flags in JEDNOTKY.DEF offset 0x48:
 *        * Bit 0 (0x01): Healed by fire (original flag, preserved at VA 0x00076D61)
 *        * Bit 1 (0x02): Ignore armor/tank defense (100% hit chance vs target flag 0x20)
 *        * Bit 2 (0x04): Ignore air defense (100% hit chance vs target flag 0x04)
 *        * Bit 3 (0x08): Ignore infantry/light defense (100% hit chance vs target flag 0x10)
 *   6. Scripted units spawn level fix after Save/Load & "Continue" (Spam lv bug):
 *      - Fixup-free Spawn Hook at VA 0x0002B9DF (file offset 0x0005A5DF, 7 bytes)
 *      - 132-byte PIC code cave at VA 0x000E62A0 (file offset 0x00114EA0)
 *      - Dynamically recalculates average player army level, applies game difficulty (-1/0/+2),
 *        clamps to 0..12, writes to DAT_0066fd50 and raises spawned unit level [ebp-0x18] if lower.
 * =================================================================================================
 */

#include <vector>
#include <cstdint>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <string>

/**
 * @brief Overwrites existing bytes in an executable vector at a given offset.
 * @param exe Vector of executable bytes.
 * @param offset Start byte address (0-indexed).
 * @param bytes Initializer list of byte values to write.
 */
inline void SetExeData(std::vector<uint8_t>& exe, size_t offset, std::initializer_list<uint8_t> bytes) {
    if (offset + bytes.size() > exe.size()) {
        throw std::out_of_range("SetExeData: attempt to write beyond end of executable");
    }
    std::copy(bytes.begin(), bytes.end(), exe.begin() + offset);
}

/**
 * @brief Overwrites existing bytes in an executable vector from raw buffer.
 */
inline void SetExeData(std::vector<uint8_t>& exe, size_t offset, const uint8_t* data, size_t size) {
    if (offset + size > exe.size()) {
        throw std::out_of_range("SetExeData: attempt to write beyond end of executable");
    }
    std::copy(data, data + size, exe.begin() + offset);
}

/**
 * @brief Inserts bytes into an executable vector at a given offset (shifts subsequent bytes).
 * @param exe Vector of executable bytes.
 * @param offset Byte address where new bytes should be inserted.
 * @param bytes Initializer list of byte values to insert.
 */
inline void InsertExeData(std::vector<uint8_t>& exe, size_t offset, std::initializer_list<uint8_t> bytes) {
    if (offset > exe.size()) {
        throw std::out_of_range("InsertExeData: attempt to insert beyond end of executable");
    }
    exe.insert(exe.begin() + offset, bytes.begin(), bytes.end());
}

/**
 * @brief Inserts bytes into an executable vector from raw buffer.
 */
inline void InsertExeData(std::vector<uint8_t>& exe, size_t offset, const uint8_t* data, size_t size) {
    if (offset > exe.size()) {
        throw std::out_of_range("InsertExeData: attempt to insert beyond end of executable");
    }
    exe.insert(exe.begin() + offset, data, data + size);
}

// =================================================================================================
// Modular Patch Subroutines
// =================================================================================================
void Patch_LE_Header(std::vector<uint8_t>& exe);
void Patch_Relocations(std::vector<uint8_t>& exe);
void Patch_Code_Displacements(std::vector<uint8_t>& exe);
void Patch_Upgrade_Stride(std::vector<uint8_t>& exe);
void Patch_Upgrade_Limits(std::vector<uint8_t>& exe);
void Patch_Save_Load_Blocks(std::vector<uint8_t>& exe);
void Patch_Loop_Bounds(std::vector<uint8_t>& exe);
void Patch_Archive_TOC_Limits(std::vector<uint8_t>& exe);
void Patch_MainMenu_Overlay(std::vector<uint8_t>& exe);
void Patch_Panic_Morale(std::vector<uint8_t>& exe);
void Patch_Group_Selection_Lock(std::vector<uint8_t>& exe);
void Patch_Convoy_Slot49(std::vector<uint8_t>& exe);
void Patch_Defense_Bypass_Flags(std::vector<uint8_t>& exe);
void Patch_Spawn_Level_Fix(std::vector<uint8_t>& exe);

/**
 * @brief Checks if the executable has already been patched with Engine Patch V1.20.
 */
bool IsSpellcrossEnginePatched(const std::vector<uint8_t>& exe_vector);

/**
 * @brief Applies the complete Spellcross Engine Patch V1.20 to the SPELCROS.EXE vector.
 * @param exe_vector Vector containing loaded SPELCROS.EXE bytes.
 * @param error_msg Optional pointer to string receiving error description on failure.
 * @return true on success, false on failure.
 */
bool ApplySpellcrossEnginePatch(std::vector<uint8_t>& exe_vector, std::string* error_msg = nullptr);
