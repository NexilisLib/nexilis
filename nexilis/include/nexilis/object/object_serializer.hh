#ifndef NEXILIS_OBJECT_SERIALIZER_HH
#define NEXILIS_OBJECT_SERIALIZER_HH

#include <nexilis/nexilis_constants.hh>

#include <nexilis/object/object_2d.hh>
#include <nexilis/object/object_3d.hh>

namespace nexilis
{

class ObjectSerializer
{
public:
    template <typename T>
    static nx_data data(const std::unique_ptr<Object<T>> object)
    {
        return object.getData();
    }

    virtual Object2D* asObject2D(const nx_data& input_data)
    {
        (void)input_data;
        return nullptr;
    }

    virtual Object3D* asObject3D(const nx_data& input_data)
    {
        (void)input_data;
        return nullptr;
    }
};

} // namespace nexilis

#endif
