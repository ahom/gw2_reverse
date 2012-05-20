#include <iostream>
#include <cstdint>

#include <sstream>
#include <fstream>

#include "gw2DatTools/interface/ANDatInterface.h"
#include "gw2DatTools/compression/inflateDatFileBuffer.h"

int main(int argc, char* argv[])
{
    const uint32_t aBufferSize = 1024 * 1024 * 30; // We make the assumption that no file is bigger than 30 Mo
    
    auto pANDatInterface = gw2dt::interface::createANDatInterface("F:\\GuildWars2\\Gw2.dat");

    auto aFileRecordVect = pANDatInterface->getFileRecordVect();
    
    uint8_t* pOriBuffer = new uint8_t[aBufferSize];
    uint8_t* pInfBuffer = new uint8_t[aBufferSize];
    
    for (auto it = aFileRecordVect.begin(); it != aFileRecordVect.end(); ++it)
    {
        uint32_t aOriSize = aBufferSize;
        pANDatInterface->getBuffer(*it, aOriSize, pOriBuffer);

        std::ostringstream aStringstream;
        aStringstream << "F:\\unpack\\";
        aStringstream << it->fileId;
        
        std::ofstream aStream(aStringstream.str(), std::ios::binary);
        
        if (aOriSize == aBufferSize)
        {
            std::cout << "File " << it->fileId << " has a size greater than (or equal to) 30Mo." << std::endl;
        }
        
        if (it->isCompressed)
        {
            uint32_t aInfSize = aBufferSize;
            
            try
            {
                gw2dt::compression::inflateDatFileBuffer(aOriSize, pOriBuffer, aInfSize, pInfBuffer);

                aStream.write(reinterpret_cast<char*>(pInfBuffer), aInfSize);
            }
            catch(std::exception& iException)
            {
                std::cout << "File " << it->fileId << " failed to decompress: " << std::string(iException.what()) << std::endl;
            }
        }
        else
        {
            aStream.write(reinterpret_cast<char*>(pOriBuffer), aOriSize);
        }

        aStream.close();
    }

    delete[] pOriBuffer;
    delete[] pInfBuffer;

    return 0;
};