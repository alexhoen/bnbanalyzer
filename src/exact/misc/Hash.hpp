/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*               This file is part of the program and library                */
/*    EXACT                                                                  */
/*                                                                           */
/* Copyright (C) 2024             Zuse Institute Berlin                      */
/*                                                                           */
/*  Licensed under the Apache License, Version 2.0 (the "License");          */
/*  you may not use this file except in compliance with the License.         */
/*  You may obtain a copy of the License at                                  */
/*                                                                           */
/*      http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                           */
/*  Unless required by applicable law or agreed to in writing, software      */
/*  distributed under the License is distributed on an "AS IS" BASIS,        */
/*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/*  See the License for the specific language governing permissions and      */
/*  limitations under the License.                                           */
/*                                                                           */
/*  You should have received a copy of the Apache-2.0 license                */
/*  along with MIP-DD; see the file LICENSE. If not visit scipopt.org.       */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _EXACT_MISC_HASH_HPP_
#define _EXACT_MISC_HASH_HPP_

#include "exact/Config.hpp"

#ifndef PAPILO_USE_STANDARD_HASHMAP
#include "exact/external/ska/bytell_hash_map.hpp"
#else
#include <unordered_map>
#include <unordered_set>
#endif

#include <cstdint>
#include <type_traits>

namespace exact
{

template <typename T, int TWidth = sizeof( T )>
struct HashHelpers;

template <typename T>
struct HashHelpers<T, 4>
{
   static uint32_t
   fibonacci_muliplier()
   {
      return uint32_t( 0x9e3779b9 );
   }

   static uint32_t
   rotate_left( uint32_t x, int n )
   {
      return ( x << n ) | ( x >> ( 32 - n ) );
   }
};

template <typename T>
struct HashHelpers<T, 8>
{
   static uint64_t
   fibonacci_muliplier()
   {
      return uint64_t( 0x9e3779b97f4a7c15 );
   }

   static uint64_t
   rotate_left( uint64_t x, int n )
   {
      return ( x << n ) | ( x >> ( 64 - n ) );
   }
};

template <typename T, typename U = typename std::make_unsigned<T>::type>
struct Hasher;

// only add specialization for unsigned result types
template <typename T>
struct Hasher<T, T>
{
   T state;

   Hasher( T init = 0 ) : state( init ) {}

   template <typename U,
             typename std::enable_if<std::is_integral<U>::value, int>::type = 0>
   void
   addValue( U val )
   {
      state = ( HashHelpers<T>::rotate_left( state, 5 ) ^ T( val ) ) *
              HashHelpers<T>::fibonacci_muliplier();
   }

   T
   getHash() const
   {
      return state;
   }
};

#ifndef PAPILO_USE_STANDARD_HASHMAP

template <typename K, typename V, typename H = std::hash<K>,
          typename E = std::equal_to<K>>
using HashMap =
    ska::bytell_hash_map<K, V, H, E, Allocator<std::pair<const K, V>>>;

template <typename T, typename H = std::hash<T>, typename E = std::equal_to<T>>
using HashSet = ska::bytell_hash_set<T, H, E, Allocator<T>>;

#else

template <typename K, typename V, typename H = std::hash<K>,
          typename E = std::equal_to<K>>
using HashMap =
    std::unordered_map<K, V, H, E, Allocator<std::pair<const K, V>>>;

template <typename T, typename H = std::hash<T>, typename E = std::equal_to<T>>
using HashSet = std::unordered_set<T, H, E, Allocator<T>>;

#endif

} // namespace exact

#endif
