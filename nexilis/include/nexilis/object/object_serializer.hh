#ifndef NEXILIS_OBJECT_SERIALIZER_HH
#define NEXILIS_OBJECT_SERIALIZER_HH

#include <nexilis/nexilis_constants.hh>
#include <nexilis/object/object2d.hh>
#include <nexilis/object/object3d.hh>

#include <memory>

namespace nexilis
{

class ObjectSerializer
{
public:
    nx_data data(const std::unique_ptr<Object<T>> object)
    {
        return object.getData();
    }

    virtual Object2D* asObject2D(const nx_data& data)
    {
        (void)data;
        return nullptr;
    }

    virtual Object3D* asObject3D(const nx_data& data)
    {
        (void)data;
        return nullptr;
    }

};

}

#endif
