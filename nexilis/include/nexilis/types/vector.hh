#ifndef NEXILIS_TYPES_VECTOR_HH
#define NEXILIS_TYPES_VECTOR_HH

#include <nexilis/types/vector_type.hh>

namespace nexilis
{

class Vector
{
public:
    virtual VectorType getType() = 0;
};

} // namespace nexilis

#endif
