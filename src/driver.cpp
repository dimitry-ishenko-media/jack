////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "driver.hpp"
#include "error.hpp"

#include <jack/control.h>

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
void driver::setup(jackctl_server* server, jackctl_driver** driver)
{
    for (auto node = jackctl_server_get_drivers_list(server); node; node = node->next)
    {
        *driver = static_cast<jackctl_driver*>(node->data);
        if (name_ == jackctl_driver_get_name(*driver))
        {
            params_ = extract_from(jackctl_driver_get_parameters(*driver));
            for (auto&& [name, value] : options_) find(params_, name).value(value);
            return;
        }
    }
    throw jack::error{EINVAL, name_};
}

////////////////////////////////////////////////////////////////////////////////
alsa_driver::alsa_driver(const alsa_options& options)
{
    name_ = "alsa";
    if (options.device) options_.emplace("device", *options.device);
    if (options.chan_in) options_.emplace("inchannels", *options.chan_in);
    if (options.chan_out) options_.emplace("outchannels", *options.chan_out);
    if (options.rate) options_.emplace("rate", *options.rate);
    if (options.period) options_.emplace("period", *options.period);
    if (options.periods) options_.emplace("nperiods", *options.periods);
}

////////////////////////////////////////////////////////////////////////////////
std::string alsa_driver::device() const { return find(params_, "device").get<std::string>(); }
unsigned alsa_driver::chan_in() const { return find(params_, "inchannels").get<unsigned>(); }
unsigned alsa_driver::chan_out() const { return find(params_, "outchannels").get<unsigned>(); }
unsigned alsa_driver::rate() const { return find(params_, "rate").get<unsigned>(); }
unsigned alsa_driver::period() const { return find(params_, "period").get<unsigned>(); }
unsigned alsa_driver::periods() const { return find(params_, "nperiods").get<unsigned>(); }

////////////////////////////////////////////////////////////////////////////////
}
