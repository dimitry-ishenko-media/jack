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

#include <audio++.hpp>
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

    auto&& name() const { return name_; }

protected:
    ////////////////////
    std::string name_;
    std::map<std::string, value> options_;
    params params_;

    explicit driver(std::string name) : name_{std::move(name)} { }

private:
    ////////////////////
    void setup(jackctl_server*, jackctl_driver**);
    friend class server;
};

////////////////////////////////////////////////////////////////////////////////
struct alsa_options
{
    std::optional<std::string> device;
    std::optional<audio::chans> chan_in;
    std::optional<audio::chans> chan_out;
    std::optional<audio::rate> rate;
    std::optional<unsigned> period;
    std::optional<unsigned> periods;
};

class alsa_driver : public driver
{
public:
    ////////////////////
    explicit alsa_driver(const alsa_options& = { });

    std::string device() const;
    audio::chans chan_in() const;
    audio::chans chan_out() const;
    audio::rate rate() const;
    unsigned period() const;
    unsigned periods() const;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
