////////////////////////////////////////////////////////////////////////////////
// This source code is part of RokLegend project.
//         (Ragnarok Online Client)
//     Copyright (c) RokLegend Team, 2013
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ROKLEGEND_SERIALIZATION_H
#define ROKLEGEND_SERIALIZATION_H

#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/hash_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/unordered_map.hpp>

#define roS11N_ALLOW_PRIVATE_ACCESS friend class boost::serialization::access
#define roS11N_SERIALIZE_FUNCTION template <class Archive> void serialize(Archive& archive, const unsigned int version)

#include <memory>

namespace boost {
    namespace serialization {

        template<class Archive, class Type>
        void save(Archive & archive, const std::shared_ptr<Type> & value, const unsigned int /*version*/)
        {
            Type *data = value.get();
            archive << data;
        }

        template<class Archive, class Type>
        void load(Archive & archive, std::shared_ptr<Type> & value, const unsigned int /*version*/)
        {
            Type *data;
            archive >> data;

            typedef std::weak_ptr<Type> WeakPtr;
            static boost::unordered_map<void*, WeakPtr> hash;

            if (hash[data].expired())
            {
                value = std::shared_ptr<Type>(data);
                hash[data] = value;
            }
            else value = hash[data].lock();
        }

        template<class Archive, class Type>
        inline void serialize(Archive & archive, std::shared_ptr<Type> & value, const unsigned int version)
        {
            split_free(archive, value, version);
        }

    }
}

#endif // ROKLEGEND_SERIALIZATION_H
