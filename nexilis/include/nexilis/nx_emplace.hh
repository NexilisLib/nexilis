#ifndef NEXILIS_NX_EMPLACE_HH
#define NEXILIS_NX_EMPLACE_HH

#include <nexilis/util.hh>

namespace nexilis
{

template <typename... Args>
static void nx_emplace(nx_data& originalData, Args&&... args)
{
    ([&originalData](const auto& data)
     {
         const auto& byteVector = Util::convertToByteVector(data);
         originalData.reserve(originalData.size() + byteVector.size());
         std::copy(byteVector.begin(), byteVector.end(), std::back_inserter(originalData)); }(std::forward<Args>(args)),
     ...);
}

} // namespace nexilis

#endif
