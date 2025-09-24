////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef JACK_TYPES_HPP
#define JACK_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

/**
 * @enum jack::dir
 * @brief Jack port direction.
 */
enum dir { in = 0x01, out = 0x02 };

constexpr dir operator|(dir x, dir y) { return static_cast<dir>(static_cast<int>(x) | static_cast<int>(y)); }
constexpr dir operator&(dir x, dir y) { return static_cast<dir>(static_cast<int>(x) & static_cast<int>(y)); }

/**
 * @var jack::physical
 * @brief Tag for physical port type.
 */
struct physical_t { explicit physical_t() = default; };
inline constexpr physical_t physical { };

/**
 * @typedef jack::sample
 * @brief Type of audio samples.
 */
using sample = float;

}

////////////////////////////////////////////////////////////////////////////////
#endif
