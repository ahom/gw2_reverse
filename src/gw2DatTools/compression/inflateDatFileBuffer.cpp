#include "gw2DatTools/compression/inflateDatFileBuffer.h"

#include <memory.h>

#include "gw2DatTools/exception/Exception.h"

#include "huffmanTreeUtils.h"

namespace gw2dt
{
namespace compression
{

// Static HuffmanTreeDict
HuffmanTree sHuffmanTreeDict;
bool sHuffmanTreeDictInitialized(false);

void inflateDatFileBuffer_initializeHuffmanTreeDict()
{
    int16_t aWorkingBitTab[MaxCodeBitsLength];
    int16_t aWorkingCodeTab[MaxSymbolValue];

    // Initialize our workingTabs
    memset(&aWorkingBitTab, 0xFF, MaxCodeBitsLength * sizeof(int16_t));
    memset(&aWorkingCodeTab, 0xFF, MaxSymbolValue * sizeof(int16_t));

    fillTabsHelper(3, 0x0A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(3, 0x09, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(3, 0x08, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(4, 0x0C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(4, 0x0B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(4, 0x07, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(4, 0x00, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(5, 0xE0, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(5, 0x2A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(5, 0x29, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(5, 0x06, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(6, 0x4A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(6, 0x40, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(6, 0x2C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(6, 0x2B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(6, 0x28, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(6, 0x20, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(6, 0x05, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(6, 0x04, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(7, 0x49, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(7, 0x48, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(7, 0x27, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(7, 0x26, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(7, 0x25, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(7, 0x0D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(7, 0x03, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(8, 0x6A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(8, 0x69, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(8, 0x4C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(8, 0x4B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(8, 0x47, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(8, 0x24, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(9, 0xE8, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(9, 0xA0, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(9, 0x89, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(9, 0x88, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(9, 0x68, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(9, 0x67, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(9, 0x63, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(9, 0x60, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(9, 0x46, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(9, 0x23, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(10, 0xE9, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0xC9, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0xC0, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0xA9, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0xA8, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x8A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x87, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x80, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x66, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x65, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x45, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x44, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x43, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x2D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x02, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(10, 0x01, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(11, 0xE5, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0xC8, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0xAA, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0xA5, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0xA4, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0x8B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0x85, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0x84, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0x6C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0x6B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0x64, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0x4D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(11, 0x0E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(12, 0xE7, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(12, 0xCA, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(12, 0xC7, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(12, 0xA7, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(12, 0xA6, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(12, 0x86, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(12, 0x83, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(13, 0xE6, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(13, 0xE4, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(13, 0xC4, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(13, 0x8C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(13, 0x2E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(13, 0x22, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(14, 0xEC, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(14, 0xC6, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(14, 0x6D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(14, 0x4E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(15, 0xEA, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(15, 0xCC, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(15, 0xAC, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(15, 0xAB, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(15, 0x8D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(15, 0x11, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(15, 0x10, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(15, 0x0F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillTabsHelper(16, 0xFF, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xFE, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xFD, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xFC, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xFB, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xFA, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF9, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF8, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF7, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF6, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF5, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF4, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF3, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF2, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF1, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xF0, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xEF, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xEE, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xED, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xEB, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xE3, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xE2, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xE1, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xDF, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xDE, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xDD, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xDC, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xDB, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xDA, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD9, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD8, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD7, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD6, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD5, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD4, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD3, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD2, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD1, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xD0, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xCF, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xCE, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xCD, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xCB, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xC5, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xC3, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xC2, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xC1, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xBF, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xBE, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xBD, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xBC, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xBB, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xBA, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB9, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB8, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB7, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB6, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB5, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB4, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB3, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB2, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB1, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xB0, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xAF, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xAE, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xAD, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xA3, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xA2, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0xA1, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x9F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x9E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x9D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x9C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x9B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x9A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x99, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x98, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x97, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x96, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x95, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x94, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x93, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x92, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x91, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x90, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x8F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x8E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x82, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x81, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x7F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x7E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x7D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x7C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x7B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x7A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x79, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x78, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x77, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x76, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x75, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x74, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x73, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x72, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x71, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x70, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x6F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x6E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x62, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x61, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x5F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x5E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x5D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x5C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x5B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x5A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x59, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x58, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x57, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x56, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x55, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x54, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x53, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x52, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x51, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x50, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x4F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x42, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x41, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x3F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x3E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x3D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x3C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x3B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x3A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x39, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x38, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x37, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x36, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x35, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x34, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x33, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x32, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x31, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x30, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x2F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x21, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x1F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x1E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x1D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x1C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x1B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x1A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x19, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x18, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x17, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x16, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x15, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x14, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x13, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillTabsHelper(16, 0x12, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    return buildHuffmanTree(sHuffmanTreeDict, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
}

// Parse and build a huffmanTree
void inflateDatFileBuffer_parseHuffmanTree(State& ioState, HuffmanTree& ioHuffmanTree)
{
    // Reading the number of symbols to read
    needBits(ioState, 16);
    uint16_t aNumberOfSymbols = static_cast<uint16_t>(readBits(ioState, 16));
    dropBits(ioState, 16);

    if (aNumberOfSymbols > MaxSymbolValue)
    {
        throw exception::Exception("Too many symbols to decode.");
    }

    int16_t aWorkingBitTab[MaxCodeBitsLength];
    int16_t aWorkingCodeTab[MaxSymbolValue];

    // Initialize our workingTabs
    memset(&aWorkingBitTab, 0xFF, MaxCodeBitsLength * sizeof(int16_t));
    memset(&aWorkingCodeTab, 0xFF, MaxSymbolValue * sizeof(int16_t));

    int16_t aRemainingSymbols = aNumberOfSymbols - 1;

    // Fetching the code repartition
    while (aRemainingSymbols > -1)
    {
        uint16_t aCode = 0;
        readCode(HuffmanTreeDict, ioState, aCode);

        uint16_t aCodeNumberOfBits = aCode & 0x1F;
        uint16_t aCodeNumberOfSymbols = (aCode >> 5) + 1;

        if (aCodeNumberOfBits == 0)
        {
            aRemainingSymbols -= aCodeNumberOfSymbols;
        }
        else
        {
            while (aCodeNumberOfSymbols > 0)
            {
                if (aWorkingBitTab[aCodeNumberOfBits] == -1)
                {
                    aWorkingBitTab[aCodeNumberOfBits] = aRemainingSymbols;
                }
                else
                {
                    aWorkingCodeTab[aRemainingSymbols] = aWorkingBitTab[aCodeNumberOfBits];
                    aWorkingBitTab[aCodeNumberOfBits] = aRemainingSymbols;
                }
                --aRemainingSymbols;
                --aCodeNumberOfSymbols;
            }
        }
    }

    // Effectively build the HuffmanTree
    return buildHuffmanTree(ioHuffmanTree, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
}

void inflateDatFileBuffer_data(State& ioState,const uint32_t iOutputSize,  uint8_t* ioOutputTab)
{
    uint32_t anOutputPos = 0;

    // Reading the const write size addition value
    needBits(ioState, 8);
    dropBits(ioState, 4);
    uint16_t aWriteSizeConstAdd = readBits(ioState, 4) + 1;
    dropBits(ioState, 4);

    // Declaring our HuffmanTrees
    HuffmanTree aHuffmanTreeSymbol;
    HuffmanTree aHuffmanTreeCopy;

    while (anOutputPos < iOutputSize)
    {
        // Reading HuffmanTrees
        inflateDatFileBuffer_parseHuffmanTree(ioState, aHuffmanTreeSymbol);
        inflateDatFileBuffer_parseHuffmanTree(ioState, aHuffmanTreeCopy);

        // Reading MaxCount
        needBits(ioState, 4);
        uint32_t aMaxCount = (readBits(ioState, 4) + 1) << 12;
        dropBits(ioState, 4);

        uint32_t aCurrentCodeReadCount = 0;

        while ((aCurrentCodeReadCount < aMaxCount) &&
               (anOutputPos < iOutputSize))
        {
            ++aCurrentCodeReadCount;

            // Reading next code
            uint16_t aCode = 0;
            readCode(aHuffmanTreeSymbol, ioState, aCode);

            if (aCode < 0x100)
            {
                ioOutputTab[anOutputPos] = static_cast<uint8_t>(aCode);
                ++anOutputPos;
                continue;
            }

            // We are in copy mode !
            // Reading the additional info to know the write size
            aCode -= 0x100;

            // write size
            div_t aCodeDiv4 = div(aCode, 4);

            uint32_t aWriteSize = 0;
            if (aCodeDiv4.quot == 0)
            {
                aWriteSize = aCode;
            }
            else if (aCodeDiv4.quot < 7)
            {
                aWriteSize = ((1 << (aCodeDiv4.quot - 1)) * (4 + aCodeDiv4.rem));
            }
            else if (aCode == 28)
            {
                aWriteSize = 0xFF;
            }
            else
            {
                throw exception::Exception("Invalid value for writeSize code.");
            }

            //additional bits
            if (aCodeDiv4.quot > 1 && aCode != 28)
            {
                uint8_t aWriteSizeAddBits = aCodeDiv4.quot - 1;
                needBits(ioState, aWriteSizeAddBits);
                aWriteSize |= readBits(ioState, aWriteSizeAddBits);
                dropBits(ioState, aWriteSizeAddBits);
            }
            aWriteSize += aWriteSizeConstAdd;

            // write offset
            // Reading the write offset
            readCode(aHuffmanTreeCopy, ioState, aCode);

            div_t aCodeDiv2 = div(aCode, 2);

            uint32_t aWriteOffset = 0;
            if (aCodeDiv2.quot == 0)
            {
                aWriteOffset = aCode;
            }
            else if (aCodeDiv2.quot < 17)
            {
                aWriteOffset = ((1 << (aCodeDiv2.quot - 1)) * (2 + aCodeDiv2.rem));
            }
            else
            {
                throw exception::Exception("Invalid value for writeOffset code.");
            }

            //additional bits
            if (aCodeDiv2.quot > 1)
            {
                uint8_t aWriteOffsetAddBits = aCodeDiv2.quot - 1;
                needBits(ioState, aWriteOffsetAddBits);
                aWriteOffset |= readBits(ioState, aWriteOffsetAddBits);
                dropBits(ioState, aWriteOffsetAddBits);
            }
            aWriteOffset += 1;

            uint32_t anAlreadyWritten = 0;
            while ((anAlreadyWritten < aWriteSize) &&
                   (anOutputPos < iOutputSize))
            {
                ioOutputTab[anOutputPos] = ioOutputTab[anOutputPos - aWriteOffset];
                ++anOutputPos;
                ++anAlreadyWritten;
            }
        }
    }
}

GW2DATTOOLS_API uint8_t* GW2DATTOOLS_APIENTRY inflateDatFileBuffer(const uint32_t iInputSize, uint8_t* iInputTab,  uint32_t& ioOutputSize, uint8_t* ioOutputTab)
{
    if (iInputTab == nullptr)
    {
        throw exception::Exception("Input buffer is null.");
    }

    if (ioOutputTab != nullptr && ioOutputSize == 0)
    {
        throw exception::Exception("Output buffer is not null and outputSize is not defined.");
    }

    uint8_t* anOutputTab(nullptr);
    bool isOutputTabOwned(true);

    try
    {
        if (!sHuffmanTreeDictInitialized)
        {
            inflateDatFileBuffer_initializeHuffmanTreeDict();
            sHuffmanTreeDictInitialized = true;
        }
        
        // Initialize state
        State aState;
        aState.input = reinterpret_cast<uint32_t*>(iInputTab);
        aState.inputSize = iInputSize / 4;
        aState.inputPos = 0;

        aState.head = 0;
        aState.bits = 0;
        aState.buffer = 0;

        // Skipping header & Getting size of the uncompressed data
        needBits(aState, 32);
        dropBits(aState, 32);
        
        // Getting size of the uncompressed data
        needBits(aState, 32);
        uint32_t anOutputSize = readBits(aState, 32);
        dropBits(aState, 32);

        if (ioOutputSize != 0)
        {
            anOutputSize = std::min(anOutputSize, ioOutputSize);
        }

        ioOutputSize = anOutputSize;

        if (ioOutputTab == nullptr)
        {
            anOutputTab = static_cast<uint8_t*>(malloc(sizeof(uint8_t) * anOutputSize));
        }
        else
        {
            isOutputTabOwned = false;
            anOutputTab = ioOutputTab;
        }

        inflateDatFileBuffer_data(aState, anOutputSize, anOutputTab);
        
        return anOutputTab;
    }
    catch(exception::Exception& iException)
    {
        if (isOutputTabOwned)
        {
            free(anOutputTab);
        }
        throw iException; // Rethrow exception
    }
    catch(std::exception& iException)
    {
        if (isOutputTabOwned)
        {
            free(anOutputTab);
        }
        throw iException; // Rethrow exception
    }
}

}
}
