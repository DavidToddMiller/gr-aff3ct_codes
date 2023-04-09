#ifndef ARGUMENT_UTILITIES_HPP
#define ARGUMENT_UTILITIES_HPP

#include <utility>
#include <vector>
#include <string>
#include <tuple>

namespace cli
{

std::vector<std::string> split(const std::string &s);

std::vector<std::string> split(const std::string &s, char delim);

void getline(std::istream &file, std::string &line);

/******************************************************************************
               TUPLE UTILITIES
 ******************************************************************************/

template<int...> struct index{};

template<int I, typename Index, typename... Types>
struct make_indexes_impl;

template<int I, int... Indexes, typename T, typename ... Types>
struct make_indexes_impl<I, index<Indexes...>, T, Types...>
{
    typedef typename make_indexes_impl<I + 1, index<Indexes..., I>, Types...>::type type;
};

template<int I, int... Indexes>
struct make_indexes_impl<I, index<Indexes...> >
{
    typedef index<Indexes...> type;
};

template<typename ... Types>
struct make_indexes : make_indexes_impl<0, index<>, Types...>
{};


template<class Ret, class... Args, int... Indexes >
Ret apply_tuple_helper( Ret (*pf)(Args...), index< Indexes... >, std::tuple<Args...>&& tup);

template<class Ret, class ... Args>
Ret apply_tuple(Ret (*pf)(Args...), const std::tuple<Args...>&  tup);

template<class Ret, class ... Args>
Ret apply_tuple(Ret (*pf)(Args...), std::tuple<Args...>&&  tup);

/******************************************************************************
               SYSTEM FUNCTIONS
 ******************************************************************************/

/*!
 * \brief return the full location/path of the binary
 *
 * \return the binary path if supported, empty string else
 */
std::string get_binary_path();

/*!
 * \brief split a path in basedir and filename
 *
 * \param path is the path (basedir/filename)
 * \param basedir is the directory without the filename
 * \param filename is the name of the file without the base directory
 */
void split_path(const std::string& path, std::string &basedir, std::string &filename);

}

#include "utilities.hxx"

#endif /* ARGUMENT_UTILITIES_HPP */