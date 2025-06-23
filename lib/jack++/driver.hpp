////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef JACK_DRIVER_HPP
#define JACK_DRIVER_HPP

////////////////////////////////////////////////////////////////////////////////
#include "jack++/param.hpp"

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

    explicit driver(std::string name) : name_{std::move(name)} { }

    void set_params(jackctl_server*, jackctl_driver**);
    virtual void get_params(jackctl_server*, jackctl_driver*) = 0;
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

    auto&& device() const noexcept { return device_; }
    auto&& chan_in() const noexcept { return chan_in_; }
    auto&& chan_out() const noexcept { return chan_out_; }
    auto&& rate() const noexcept { return rate_; }
    auto&& period() const noexcept { return period_; }
    auto&& periods() const noexcept { return periods_; }

private:
    ////////////////////
    void get_params(jackctl_server*, jackctl_driver*) override;

    std::string device_;
    audio::chans chan_in_;
    audio::chans chan_out_;
    audio::rate rate_;
    unsigned period_;
    unsigned periods_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
