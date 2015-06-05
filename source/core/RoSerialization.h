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

#if BOOST_VERSION <= 105500
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
#elif BOOST_VERSiON < 105800
#   include <boost/serialization/utility.hpp> 
#   include <boost/serialization/collections_save_imp.hpp> 
#   include <boost/serialization/collections_load_imp.hpp> 
#   include <boost/serialization/split_free.hpp> 

namespace boost {
    namespace serialization {

        template<class Archive, class Type, class Key, class Hash, class Compare, class Allocator >
        inline void save(
            Archive & ar,
            const boost::unordered_map<Key, Type, Hash, Compare, Allocator> &t,
            const unsigned int /* file_version */
            ){
            boost::serialization::stl::save_collection <
                Archive,
                boost::unordered_map<Key, Type, Hash, Compare, Allocator>
            > (ar, t);
        }

        template<class Archive, class Type, class Key, class Hash, class Compare, class Allocator >
        inline void load(
            Archive & ar,
            boost::unordered_map<Key, Type, Hash, Compare, Allocator> &t,
            const unsigned int /* file_version */
            ){
            boost::serialization::stl::load_collection <
                Archive,
                boost::unordered_map<Key, Type, Hash, Compare, Allocator>,
                boost::serialization::stl::archive_input_map<
                Archive, boost::unordered_map<Key, Type, Hash, Compare, Allocator> >,
                boost::serialization::stl::no_reserve_imp < boost::unordered_map <
                Key, Type, Hash, Compare, Allocator
                >
                >
            > (ar, t);
        }

        // split non-intrusive serialization function member into separate 
        // non intrusive save/load member functions 
        template<class Archive, class Type, class Key, class Hash, class Compare, class Allocator >
        inline void serialize(
            Archive & ar,
            boost::unordered_map<Key, Type, Hash, Compare, Allocator> &t,
            const unsigned int file_version
            ){
            boost::serialization::split_free(ar, t, file_version);
        }

        // multimap 
        template<class Archive, class Type, class Key, class Hash, class Compare, class Allocator >
        inline void save(
            Archive & ar,
            const boost::unordered_multimap<Key, Type, Hash, Compare, Allocator> &t,
            const unsigned int /* file_version */
            ){
            boost::serialization::stl::save_collection <
                Archive,
                boost::unordered_multimap<Key, Type, Hash, Compare, Allocator>
            > (ar, t);
        }

        template<class Archive, class Type, class Key, class Hash, class Compare, class Allocator >
        inline void load(
            Archive & ar,
            boost::unordered_multimap<Key, Type, Hash, Compare, Allocator> &t,
            const unsigned int /* file_version */
            ){
            boost::serialization::stl::load_collection <
                Archive,
                boost::unordered_multimap<Key, Type, Hash, Compare, Allocator>,
                boost::serialization::stl::archive_input_multimap<
                Archive, boost::unordered_multimap<Key, Type, Hash, Compare, Allocator>
                >,
                boost::serialization::stl::no_reserve_imp <
                boost::unordered_multimap<Key, Type, Hash, Compare, Allocator>
                >
            > (ar, t);
        }

        // split non-intrusive serialization function member into separate 
        // non intrusive save/load member functions 
        template<class Archive, class Type, class Key, class Hash, class Compare, class Allocator >
        inline void serialize(
            Archive & ar,
            boost::unordered_multimap<Key, Type, Hash, Compare, Allocator> &t,
            const unsigned int file_version
            ){
            boost::serialization::split_free(ar, t, file_version);
        }

    } // serialization 
} // namespace boost
#endif // BOOST_VERSION <= 105500

#endif // ROKLEGEND_SERIALIZATION_H
