#ifndef NEXILIS_NX_CREATE_HH
#define NEXILIS_NX_CREATE_HH

#include <nexilis/nx_data.hh>

// TODO add nx_emplace here

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

} // namespace nexilis

#endif
