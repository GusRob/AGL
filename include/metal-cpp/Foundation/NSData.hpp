//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Foundation/NSData.hpp
//
// Copyright 2020-2021 Apple Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "NSObject.hpp"
#include "NSTypes.hpp"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace NS
{
class Data : public Copying<Data>
{
public:
    static Data*  alloc();
    void*         mutableBytes() const;
    UInteger      length() const;
    Data*         init(const void* bytes, UInteger length);
};
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

_NS_INLINE NS::Data* NS::Data::alloc()
{
    return NS::Object::alloc<Data>(_NS_PRIVATE_CLS(NSData));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

_NS_INLINE void* NS::Data::mutableBytes() const
{
    return Object::sendMessage<void*>(this, _NS_PRIVATE_SEL(mutableBytes));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

_NS_INLINE NS::UInteger NS::Data::length() const
{
    return Object::sendMessage<UInteger>(this, _NS_PRIVATE_SEL(length));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

_NS_INLINE NS::Data* NS::Data::init(const void* bytes, UInteger length)
{
	return NS::Object::sendMessage<Data*>(this, _NS_PRIVATE_SEL(initWithBytesNoCopy_length_), bytes, length);
}
