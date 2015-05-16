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

#define roS11N_ALLOW_PRIVATE_ACCESS friend class boost::serialization::access
#define roS11N_SERIALIZE_FUNCTION template <class Archive> void serialize(Archive& archive, const unsigned int version)

#endif // ROKLEGEND_SERIALIZATION_H
