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

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum jack::chans
 * @brief Count of audio channels.
 */
enum chans : unsigned { mono = 1, stereo = 2 };

namespace literals
{
constexpr auto operator"" _ch (unsigned long long n) noexcept { return static_cast<chans>(n); }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum jack::dir
 * @brief Jack port direction.
 */
enum dir { in = 0x01, out = 0x02 };

constexpr dir operator|(dir x, dir y) { return static_cast<dir>(static_cast<int>(x) | static_cast<int>(y)); }
constexpr dir operator&(dir x, dir y) { return static_cast<dir>(static_cast<int>(x) & static_cast<int>(y)); }

////////////////////////////////////////////////////////////////////////////////
/**
 * @var jack::physical
 * @brief Tag for physical port type.
 */
struct physical_t { explicit physical_t() = default; };
inline constexpr physical_t physical { };

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum jack::rate
 * @brief Audio sample rate.
 */
enum rate : unsigned { };

namespace literals
{
constexpr auto operator"" _hz (unsigned long long n) noexcept { return static_cast<rate>(n); }
constexpr auto operator"" _khz(unsigned long long n) noexcept { return static_cast<rate>(n * 1000); }
constexpr auto operator"" _khz(long double n) noexcept { return static_cast<rate>(n * 1000); }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @typedef jack::sample
 * @brief Type of audio samples.
 */
using sample = float;

////////////////////////////////////////////////////////////////////////////////
/**
 * @enum jack::type
 * @brief Type of audio samples.
 */
enum type : unsigned
{
    u8, s16, s24, s32, f32,
};

////////////////////////////////////////////////////////////////////////////////
/**
 * @struct jack::format
 * @brief Audio format (count of channels, sample rate and sample type).
 */
struct format
{
    jack::chans chans;
    jack::rate rate;
    jack::type type;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
