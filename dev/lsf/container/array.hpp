// File:        array.hpp
// Description: ---
// Notes:       ---
// Author:      leoxiang <leoxiang@tencent.com>
// Revision:    2012-12-13 by leoxiang

#pragma once

#include <stdexcept>
#include "lsf/basic/macro.hpp"
#include "lsf/container/detail/basic_container.hpp"
#include "lsf/container/detail/impl_static_array.hpp"
#include "lsf/container/shared_mem.hpp"

namespace lsf {
namespace container {

////////////////////////////////////////////////////////////
// Array
////////////////////////////////////////////////////////////
template<
    typename ElemType, 
    typename StoreType = SharedMem,
    typename SizeType = size_t>
class Array : 
    public detail::BasicContainer<
        ElemType, 
        SizeType, 
        StoreType,
        detail::StaticArrayState<ElemType, SizeType>, 
        detail::StaticArrayIterator<ElemType, SizeType> >
{
public:
    typedef detail::BasicContainer<
        ElemType, 
        SizeType, 
        StoreType,
        detail::StaticArrayState<ElemType, SizeType>, 
        detail::StaticArrayIterator<ElemType, SizeType> > base_type;
    typedef typename base_type::value_type          value_type;
    typedef typename base_type::size_type           size_type;
    typedef typename base_type::iterator            iterator;
    typedef typename base_type::reverse_iterator    reverse_iterator;

public:
    // access func
    value_type & operator[](size_type index) {
        if (index >= base_type::MaxSize()) throw std::runtime_error(LSF_DEBUG_INFO);
        return *(base_type::_ptr_state->GetDataPtr(index));
    }

    value_type const & operator[](size_type index) const {
        if (index >= base_type::MaxSize()) throw std::runtime_error(LSF_DEBUG_INFO);
        return *(base_type::_ptr_state->GetDataPtr(index));
    }

    value_type & Get(size_type index) {
        if (index >= base_type::MaxSize()) throw std::runtime_error(LSF_DEBUG_INFO);
        return *(base_type::_ptr_state->GetDataPtr(index));
    }

    value_type const & Get(size_type index) const {
        if (index >= base_type::MaxSize()) throw std::runtime_error(LSF_DEBUG_INFO);
        return *(base_type::_ptr_state->GetDataPtr(index));
    }

    value_type * GetPtr(size_type index) {
        if (index >= base_type::MaxSize()) return NULL;
        return base_type::_ptr_state->GetDataPtr(index);
    }

    value_type const * GetPtr(size_type index) const {
        if (index >= base_type::MaxSize()) return NULL;
        return base_type::_ptr_state->GetDataPtr(index);
    }

    // iterator
    iterator Begin() { return iterator(base_type::_ptr_state, 0); }
    iterator End()   { return iterator(base_type::_ptr_state, base_type::MaxSize()); }

    reverse_iterator RBegin() { return reverse_iterator(End()); }
    reverse_iterator REnd()   { return reverse_iterator(Begin()); }
};


} // end of namespace container
} // end of namespace lsf

// vim:ts=4:sw=4:et:ft=cpp:
