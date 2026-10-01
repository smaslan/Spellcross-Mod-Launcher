#pragma once

/**
 * =================================================================================================
 * Spellcross Engine Patch V1.0 - C++ Integration Module
 * =================================================================================================
 * Compatibility: SPELCROS.EXE (1,632,112 bytes)
 * Architecture: x86 / DOS / DOS32A / LE (Linear Executable)
 * Author: HonzaQ & AI Assistant (2026)
 *
 * This module applies the complete engine modifications for:
 *   - Extended unit limits (87 -> 127 units)
 *   - Extended sound definitions (A_SOUNDS: 87 -> 127, Z_SOUNDS: 47 -> 80, H_SOUNDS: 27 -> 50)
 *   - Extended upgrades & workshop (36 -> 72 upgrades, 142 -> 180 byte stride)
 *   - Expanded save/load state buffer (29,089 bytes)
 *   - Expanded UNITS.FSU TOC (220 KB -> 1 MB) and .FS TOC (2,600 -> 4,096 files)
 *   - Expanded sprite/frame cache buffers (64 KB each)
 *   - Dynamic MainMenu version overlay with centering & adaptive background box
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

/**
 * @brief Checks if the executable has already been patched with Engine Patch V1.0.
 */
bool IsSpellcrossEnginePatched(const std::vector<uint8_t>& exe_vector);

/**
 * @brief Applies the complete Spellcross Engine Patch V1.0 to the SPELCROS.EXE vector.
 * @param exe_vector Vector containing loaded SPELCROS.EXE bytes.
 * @param error_msg Optional pointer to string receiving error description on failure.
 * @return true on success, false on failure.
 */
bool ApplySpellcrossEnginePatch(std::vector<uint8_t>& exe_vector, std::string* error_msg = nullptr);
