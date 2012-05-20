#ifndef GW2DATTOOLS_USECASE_DEDUCEFILETYPE_H
#define GW2DATTOOLS_USECASE_DEDUCEFILETYPE_H

#include "gw2DatTools/dllMacros.h"

#include "gw2DatTools/interface/ANDatInterface.h"

namespace gw2dt
{
namespace usecase
{

GW2DATTOOLS_API format::FileType GW2DATTOOLS_APIENTRY deduceFileType(const uint32_t iInputSize, uint8_t* iUncompressedBuffer);

}
}

#endif // GW2DATTOOLS_USECASE_DEDUCEFILETYPE_H
