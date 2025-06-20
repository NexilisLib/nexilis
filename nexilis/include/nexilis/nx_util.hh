#ifndef NEXILIS_NX_CREATE_HH
#define NEXILIS_NX_CREATE_HH

#include <nexilis/util.hh>

namespace nexilis
{

template <typename... Args>
nx_data nx_create(Args&&... args)
{
    // Calculate total size needed.
    size_t total_size = 0;
    (void)std::initializer_list<int>{
            (total_size += std::forward<Args>(args).size(), 0)...};

    // Create result vector with enough capacity.
    std::vector<uint8_t> result;
    result.reserve(total_size);

    // Append all vectors using move semantics where possible.
    (void)std::initializer_list<int>{
            (result.insert(result.end(),
                           std::make_move_iterator(std::forward<Args>(args).begin()),
                           std::make_move_iterator(std::forward<Args>(args).end())),
             0)...};

    return result;
}

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
