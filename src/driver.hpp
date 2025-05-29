////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef JACK_DRIVER_HPP
#define JACK_DRIVER_HPP

////////////////////////////////////////////////////////////////////////////////
#include "param.hpp"

#include <map>
#include <optional>
#include <string>

struct jackctl_driver;
struct jackctl_server;

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
class driver
{
public:
    ////////////////////
    driver(const driver&) = delete;
    driver(driver&&) = default;

    driver& operator=(const driver&) = delete;
    driver& operator=(driver&&) = default;

protected:
    ////////////////////
    std::string name_;
    std::map<std::string, value> options_;

    mutable params params_;

    ////////////////////
    driver() = default;
    void setup(jackctl_server*, jackctl_driver**) const;

    friend class server;
};

////////////////////////////////////////////////////////////////////////////////
struct alsa_options
{
    std::optional<std::string> device;
    std::optional<unsigned> chan_in;
    std::optional<unsigned> chan_out;
    std::optional<unsigned> rate;
    std::optional<unsigned> period;
    std::optional<unsigned> periods;
};

class alsa_driver : public driver
{
public:
    ////////////////////
    explicit alsa_driver(const alsa_options& = { });

    std::string device() const;
    unsigned chan_in() const;
    unsigned chan_out() const;
    unsigned rate() const;
    unsigned period() const;
    unsigned periods() const;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
