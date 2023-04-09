#include "utilities.hpp"

template<class Ret, class... Args, int... Indexes >
Ret cli::apply_tuple_helper( Ret (*pf)(Args...), index< Indexes... >, std::tuple<Args...>&& tup)
{
    return pf( std::forward<Args>( std::get<Indexes>(tup))... );
}

template<class Ret, class ... Args>
Ret cli::apply_tuple(Ret (*pf)(Args...), const std::tuple<Args...>&  tup)
{
    return apply_tuple_helper(pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
}

template<class Ret, class ... Args>
Ret cli::apply_tuple(Ret (*pf)(Args...), std::tuple<Args...>&&  tup)
{
    return apply_tuple_helper(pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
}
