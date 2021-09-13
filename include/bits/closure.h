#pragma once

#include "detail/closure_helper.h"

namespace compiler
{
    template <typename T>
    using set_type = typename detail::closure_helper<T>::set_type;
    template <typename T>
    using grammar_type = typename detail::closure_helper<T>::grammar_type;

    template <typename T, detail::fixed_point fp>
    set_type<T> closure(set_type<T> const& items, grammar_type<T> const& grammar)
    {
        return detail::closure_helper<T, fp> {grammar}(items);
    }

    template <typename T>
    set_type<T> closure(set_type<T> const& items, grammar_type<T> const& grammar)
    {
        return detail::closure_helper<T> {grammar}(items);
    }
}