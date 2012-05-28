#include "gw2DatTools/compression/inflateTextureFileBuffer.h"

#include <memory.h>

#include "gw2DatTools/exception/Exception.h"

#include "huffmanTreeUtils.h"

#include <iostream>
#include <vector>

namespace gw2dt
{
namespace compression
{
namespace texture
{

struct Format
{
	uint16_t flags;
	uint16_t pixelSizeInBits;
};

struct FullFormat
{
	Format format;
	uint32_t nbObPixelBlocks;
	uint32_t bytesPerPixelBlock;
	uint32_t bytesPerComponent;

	uint16_t width;
	uint16_t height;
};

enum FormatFlags
{
	FF_COLOR = 0x10,
	FF_ALPHA = 0x20,
	FF_SINGLEBLOCK = 0x40,
	FF_BOTHBLOCKS = 0x80,
	FF_UNKNOWN = 0x200
};

enum CompressionFlags
{
	CF_DECODE_COLOR_ALT = 0x01,
	CF_DECODE_ALPHA = 0x04,
	CF_DECODE_COLOR = 0x08,
	CF_UNKNOWN = 0x200
};

// Static Values
HuffmanTree sHuffmanTreeDict;
Format sFormats[9];
bool sStaticValuesInitialized(false);

void initializeStaticValues()
{
	// Formats
	{
		Format& aDxt1Format = sFormats[0];
		aDxt1Format.flags = FF_COLOR | FF_SINGLEBLOCK;
		aDxt1Format.pixelSizeInBits = 4;

		Format& aDxt2Format = sFormats[1];
		aDxt2Format.flags = FF_ALPHA | FF_COLOR | FF_BOTHBLOCKS;
		aDxt2Format.pixelSizeInBits = 8;

		sFormats[2] = sFormats[1];
		sFormats[3] = sFormats[1];
		sFormats[4] = sFormats[1];

		Format& aDxtAFormat = sFormats[5];
		aDxtAFormat.flags = FF_ALPHA | FF_BOTHBLOCKS;
		aDxtAFormat.pixelSizeInBits = 4;

		Format& aDxtLFormat = sFormats[6];
		aDxtLFormat.flags = FF_COLOR;
		aDxtLFormat.pixelSizeInBits = 8;

		Format& aDxtNFormat = sFormats[7];
		aDxtNFormat.flags = FF_UNKNOWN;
		aDxtNFormat.pixelSizeInBits = 8;

		Format& a3dcxFormat = sFormats[8];
		a3dcxFormat.flags = FF_UNKNOWN;
		a3dcxFormat.pixelSizeInBits = 8;
	}

    int16_t aWorkingBitTab[MaxCodeBitsLength];
    int16_t aWorkingCodeTab[MaxSymbolValue];

    // Initialize our workingTabs
    memset(&aWorkingBitTab, 0xFF, MaxCodeBitsLength * sizeof(int16_t));
    memset(&aWorkingCodeTab, 0xFF, MaxSymbolValue * sizeof(int16_t));

    fillWorkingTabsHelper(1, 0x00, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
	
    fillWorkingTabsHelper(2, 0x11, &aWorkingBitTab[0], &aWorkingCodeTab[0]);

    fillWorkingTabsHelper(6, 0x10, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x0F, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x0E, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x0D, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x0C, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x0B, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x0A, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x09, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
	fillWorkingTabsHelper(6, 0x08, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x07, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x06, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x05, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x04, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x03, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
    fillWorkingTabsHelper(6, 0x02, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
	fillWorkingTabsHelper(6, 0x01, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
	
    return buildHuffmanTree(sHuffmanTreeDict, &aWorkingBitTab[0], &aWorkingCodeTab[0]);
}

Format deduceFormat(uint32_t iFourCC)
{
	switch(iFourCC)
	{
		case 0x31545844: // DXT1
			return sFormats[0];

		case 0x32545844: // DXT2
			return sFormats[1];

		case 0x33545844: // DXT3
			return sFormats[2];

		case 0x34545844: // DXT4
			return sFormats[3];

		case 0x35545844: // DXT5
			return sFormats[4];

		case 0x41545844: // DXTA
			return sFormats[5];

		case 0x4C545844: // DXTL
			return sFormats[6];

		case 0x4E545844: // DXTN
			return sFormats[7];

		case 0x58434433: // 3DCX
			return sFormats[8];

		default:
			throw exception::Exception("Unknown format.");
	}
}

void handleDxt1(State& ioState, std::vector<bool>& ioAlphaBitMap, std::vector<bool>& ioColorBitMap, const FullFormat& iFullFormat, uint8_t* ioOutputTab)
{
	uint32_t aPixelBlockPos = 0;

	while (aPixelBlockPos < iFullFormat.nbObPixelBlocks)
    {
		// Reading next code
		uint16_t aCode = 0;
		readCode(sHuffmanTreeDict, ioState, aCode);

		std::cout << "CodeDxt1: " << aCode << std::endl;
		
		needBits(ioState, 1);
		uint32_t aValue = readBits(ioState, 1);
		dropBits(ioState, 1);

		if (aValue)
		{
			for (uint32_t aCurrentPixelBlock = 0; aCurrentPixelBlock <= aCode; ++aCurrentPixelBlock)
			{
				if (!ioColorBitMap[aCurrentPixelBlock + aPixelBlockPos])
				{
					*reinterpret_cast<int16_t*>(&(ioOutputTab[iFullFormat.bytesPerPixelBlock * (aCurrentPixelBlock + aPixelBlockPos)])) = -2;
					*reinterpret_cast<int16_t*>(&(ioOutputTab[iFullFormat.bytesPerPixelBlock * (aCurrentPixelBlock + aPixelBlockPos) + 2])) = -1;
					*reinterpret_cast<int32_t*>(&(ioOutputTab[iFullFormat.bytesPerPixelBlock * (aCurrentPixelBlock + aPixelBlockPos) + 4])) = -1;

					ioAlphaBitMap[aCurrentPixelBlock + aPixelBlockPos] = true;
					ioColorBitMap[aCurrentPixelBlock + aPixelBlockPos] = true;
				}
			}
		}

		aPixelBlockPos += aCode + 1;
    }
}

void handleAlphaBitMap(State& ioState, std::vector<bool>& ioAlphaBitMap, const FullFormat& iFullFormat, uint8_t* ioOutputTab)
{
	needBits(ioState, 8);
	uint8_t aAlphaValueByte = readBits(ioState, 8);
	std::cout << "aAlphaValueByte: " << std::hex << aAlphaValueByte << std::dec << std::endl;
	dropBits(ioState, 8); // Drop Byte

	uint32_t aPixelBlockPos = 0;

	uint64_t aAlphaValue = aAlphaValueByte | (aAlphaValueByte << 8);
	uint64_t zero = 0;

	while (aPixelBlockPos < iFullFormat.nbObPixelBlocks)
    {
		// Reading next code
		uint16_t aCode = 0;
		readCode(sHuffmanTreeDict, ioState, aCode);

		std::cout << "CodeColor: " << aCode << std::endl;
		
		needBits(ioState, 2);
		uint32_t aValue = readBits(ioState, 1);
		dropBits(ioState, 1);

		uint8_t isNotNull = readBits(ioState, 1);
		if (aValue)
		{
			dropBits(ioState, 1);

			for (uint32_t aCurrentPixelBlock = 0; aCurrentPixelBlock <= aCode; ++aCurrentPixelBlock)
			{
				memcpy(&(ioOutputTab[iFullFormat.bytesPerPixelBlock * (aCurrentPixelBlock + aPixelBlockPos)]), isNotNull ? &aAlphaValue : &zero, iFullFormat.bytesPerComponent);
				ioAlphaBitMap[aCurrentPixelBlock + aPixelBlockPos] = true;
			}
		}
		
		aPixelBlockPos += aCode + 1;
    }
}


void handleColorBitMap(State& ioState, std::vector<bool>& ioColorBitMap, const FullFormat& iFullFormat, uint8_t* ioOutputTab)
{
	needBits(ioState, 24);
	uint32_t aColorValueBytes = 0xFF000000 | readBits(ioState, 24);
	std::cout << "ColorValueBytes: " << std::hex << aColorValueBytes << std::dec << std::endl;
	dropBits(ioState, 24);

	uint64_t aColorValue = 0;

	if (aColorValueBytes == 0xFFFFFFFF)
	{
		aColorValue = 0x00000000FFFEFFFF;
	}
	else if (aColorValueBytes == 0xFF000000)
	{
		if (iFullFormat.format.flags & FF_SINGLEBLOCK)
		{
			aColorValue = 0xAAAAAAAA00000000;
		}
		else
		{
			aColorValue = 0x5555555500000001;
		}
	}

	uint32_t aPixelBlockPos = 0;

	while (aPixelBlockPos < iFullFormat.nbObPixelBlocks)
    {
		// Reading next code
		uint16_t aCode = 0;
		readCode(sHuffmanTreeDict, ioState, aCode);

		std::cout << "CodeAlpha: " << aCode << std::endl;
		
		needBits(ioState, 1);
		uint32_t aValue = readBits(ioState, 1);
		dropBits(ioState, 1);

		if (aValue)
		{
			for (uint32_t aCurrentPixelBlock = 0; aCurrentPixelBlock <= aCode; ++aCurrentPixelBlock)
			{
				uint32_t aOffset = iFullFormat.bytesPerPixelBlock * (aCurrentPixelBlock + aPixelBlockPos);
				if (iFullFormat.format.flags & FF_BOTHBLOCKS)
				{
					aOffset += iFullFormat.bytesPerComponent;
				}
				memcpy(&(ioOutputTab[aOffset]), &aColorValue, iFullFormat.bytesPerComponent);
				ioColorBitMap[aCurrentPixelBlock + aPixelBlockPos] = true;
			}
		}
		
		aPixelBlockPos += aCode + 1;
    }
}

void inflateData(State& iState, const FullFormat& iFullFormat, uint32_t ioOutputSize, uint8_t* ioOutputTab)
{
	// Getting size of compressed data
	needBits(iState, 32);
    uint32_t aDataSize = readBits(iState, 32);
    dropBits(iState, 32);

	std::cout << "DataSize: " << aDataSize << std::endl;

	// Compression Flags
	needBits(iState, 32);
    uint32_t aCompressionFlags = readBits(iState, 32);
    dropBits(iState, 32);

	std::cout << "Flags: " << aCompressionFlags << std::endl;

	// Bitmaps
	std::vector<bool> aColorBitmap;
	std::vector<bool> aAlphaBitmap;

	std::cout << "flags: " << std::hex << iFullFormat.format.flags << std::dec << std::endl;

	aColorBitmap.assign(iFullFormat.nbObPixelBlocks, false);
	aAlphaBitmap.assign(iFullFormat.nbObPixelBlocks, false);

	if (aCompressionFlags & CF_DECODE_COLOR_ALT)
	{
		handleDxt1(iState, aAlphaBitmap, aColorBitmap, iFullFormat, ioOutputTab);
	}

	if (aCompressionFlags & CF_DECODE_ALPHA)
	{
		handleAlphaBitMap(iState, aAlphaBitmap, iFullFormat, ioOutputTab);
	}

	if (aCompressionFlags & CF_DECODE_COLOR)
	{
		handleColorBitMap(iState, aColorBitmap, iFullFormat, ioOutputTab);
	}	

	uint32_t aLoopIndex;

	if (iState.bits >= 32)
	{
		--iState.inputPos;
	}

	if ((iFullFormat.format.flags) & FF_ALPHA)
	{
		for (aLoopIndex = 0; aLoopIndex < aAlphaBitmap.size() && iState.inputPos < iState.inputSize; ++aLoopIndex)
		{
			if (!aAlphaBitmap[aLoopIndex])
			{
				(*reinterpret_cast<uint32_t*>(&(ioOutputTab[iFullFormat.bytesPerPixelBlock * aLoopIndex]))) = iState.input[iState.inputPos];
				++iState.inputPos;
				if (iFullFormat.bytesPerComponent > 4)
				{
					(*reinterpret_cast<uint32_t*>(&(ioOutputTab[iFullFormat.bytesPerPixelBlock * aLoopIndex + 4]))) = iState.input[iState.inputPos];
					++iState.inputPos;
				}
			}
		}
	}

	if ((iFullFormat.format.flags) & FF_COLOR)
	{
		for (aLoopIndex = 0; aLoopIndex < aColorBitmap.size() && iState.inputPos < iState.inputSize; ++aLoopIndex)
		{
			if (!aColorBitmap[aLoopIndex])
			{
				uint32_t aOffset = iFullFormat.bytesPerPixelBlock * aLoopIndex;
				if (iFullFormat.format.flags & FF_BOTHBLOCKS)
				{
					aOffset += iFullFormat.bytesPerComponent;
				}
				(*reinterpret_cast<uint32_t*>(&(ioOutputTab[aOffset]))) = iState.input[iState.inputPos];
				++iState.inputPos;
			}
		}
	}

	if (((iFullFormat.format.flags) & FF_COLOR))
	{
		for (aLoopIndex = 0; aLoopIndex < aColorBitmap.size() && iState.inputPos < iState.inputSize; ++aLoopIndex)
		{
			if (!aColorBitmap[aLoopIndex])
			{
				uint32_t aOffset = iFullFormat.bytesPerPixelBlock * aLoopIndex + 4;
				if (iFullFormat.format.flags & FF_BOTHBLOCKS)
				{
					aOffset += iFullFormat.bytesPerComponent;
				}
				(*reinterpret_cast<uint32_t*>(&(ioOutputTab[aOffset]))) = iState.input[iState.inputPos];
				++iState.inputPos;
			}
		}
	}
}
}

GW2DATTOOLS_API uint8_t* GW2DATTOOLS_APIENTRY TEST_inflateTextureFileBuffer(const uint32_t iInputSize, uint8_t* iInputTab,  uint32_t& ioOutputSize, uint8_t* ioOutputTab)
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
        if (!texture::sStaticValuesInitialized)
        {
            texture::initializeStaticValues();
            texture::sStaticValuesInitialized = true;
        }
        
        // Initialize state
        State aState;
        aState.input = reinterpret_cast<uint32_t*>(iInputTab);
        aState.inputSize = iInputSize / 4;
        aState.inputPos = 0;

        aState.head = 0;
        aState.bits = 0;
        aState.buffer = 0;

		aState.isEmpty = false;

        // Skipping header
        needBits(aState, 32);
        dropBits(aState, 32);
		
		// Format
		needBits(aState, 32);
		uint32_t aFormatFourCc = readBits(aState, 32);
		dropBits(aState, 32);

		std::cout << "FourCC: " << std::hex << aFormatFourCc << std::dec << std::endl;

		texture::FullFormat aFullFormat;

		aFullFormat.format = texture::deduceFormat(aFormatFourCc);
        
        // Getting width/height
        needBits(aState, 32);
		aFullFormat.width = readBits(aState, 16);
		dropBits(aState, 16);
		aFullFormat.height = readBits(aState, 16);
		dropBits(aState, 16);

		std::cout << "Width: " << aFullFormat.width << std::endl;
		std::cout << "Height: " << aFullFormat.height << std::endl;

		aFullFormat.nbObPixelBlocks = ((aFullFormat.width + 3) / 4) * ((aFullFormat.height + 3) / 4);
		aFullFormat.bytesPerPixelBlock = (aFullFormat.format.pixelSizeInBits * 4 * 4) / 8;
		aFullFormat.bytesPerComponent = (aFullFormat.format.flags & texture::FF_BOTHBLOCKS) ? 8 : (aFullFormat.format.flags & texture::FF_SINGLEBLOCK) ? 8 : 0;
		
		uint32_t anOutputSize = aFullFormat.bytesPerPixelBlock * aFullFormat.nbObPixelBlocks;

		if (ioOutputSize != 0 && ioOutputSize < anOutputSize)
        {
            throw exception::Exception("Output buffer is too small.");
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

		texture::inflateData(aState, aFullFormat, ioOutputSize, anOutputTab);
        
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
