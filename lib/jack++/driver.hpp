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
#include "jack++/types.hpp"

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
    driver& operator=(const driver&) = delete;

    driver(driver&&) = default;
    driver& operator=(driver&&) = default;

    constexpr auto& name() const noexcept { return name_; }

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
    std::optional<jack::chans> chan_in;
    std::optional<jack::chans> chan_out;
    std::optional<jack::rate> rate;
    std::optional<unsigned> period;
    std::optional<unsigned> periods;
};

class alsa_driver : public driver
{
public:
    ////////////////////
    explicit alsa_driver(const alsa_options& = { });

    constexpr auto& device() const noexcept { return device_; }
    constexpr auto& chan_in() const noexcept { return chan_in_; }
    constexpr auto& chan_out() const noexcept { return chan_out_; }
    constexpr auto& rate() const noexcept { return rate_; }
    constexpr auto& period() const noexcept { return period_; }
    constexpr auto& periods() const noexcept { return periods_; }

private:
    ////////////////////
    void get_params(jackctl_server*, jackctl_driver*) override;

    std::string device_;
    jack::chans chan_in_;
    jack::chans chan_out_;
    jack::rate rate_;
    unsigned period_;
    unsigned periods_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
