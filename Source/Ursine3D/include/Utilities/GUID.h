/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GUID.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace ursine
{
    typedef boost::uuids::uuid GUID;
    typedef boost::hash<GUID> GUIDHasher;
    typedef boost::uuids::random_generator GUIDGenerator;
    typedef boost::uuids::string_generator GUIDStringGenerator;
}