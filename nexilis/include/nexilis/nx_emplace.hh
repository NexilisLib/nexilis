#ifndef NEXILIS_NX_EMPLACE_HH
#define NEXILIS_NX_EMPLACE_HH

#include <nexilis/nexilis_constants.hh>
#include <nexilis/util.hh>

namespace nexilis
{

template <typename... Args>
static void nx_emplace(nx_data& originalData, Args&&... args)
{
    ([&originalData](const auto& data)
     {
        const auto& byteVector = Util::convertToByteVector(data);
        for (const auto& byte : byteVector)
        {
            originalData.emplace_back(byte);
        }

     } (std::forward<Args>(args)), ...);
}

}

#endif
