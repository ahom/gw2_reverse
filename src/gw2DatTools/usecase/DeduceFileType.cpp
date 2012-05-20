#include "gw2DatTools/usecase/DeduceFileType.h"

namespace gw2dt
{
namespace usecase
{

GW2DATTOOLS_API format::FileType GW2DATTOOLS_APIENTRY deduceFileType(const uint32_t iInputSize, uint8_t* iUncompressedBuffer)
{
    return format::FT_Unknown;
}

}
}

