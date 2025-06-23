////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "jack++/error.hpp"
#include "jack++/param.hpp"
#include "jack++/server.hpp"

#include <jack/control.h>

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
server::server(const std::string& name, jack::driver&& driver, const server_options& options) :
    server_{ jackctl_server_create(nullptr, nullptr), &jackctl_server_destroy }
{
    if (!server_) throw jack::error{server_startup_error, "jackctl_server_create()"};

    auto params = jackctl_server_get_parameters(&*server_);
    auto param_name = param::from_list(params, "name");
    auto param_realtime = param::from_list(params, "realtime");
    auto param_priority = param::from_list(params, "realtime-priority");

    param_name.set(name);
    if (options.realtime) param_realtime.set(*options.realtime);
    if (options.priority) param_priority.set(*options.priority);

    jackctl_driver* driver_;
    driver.set_params(&*server_, &driver_);

    auto success = jackctl_server_open(&*server_, driver_);
    if (!success) throw jack::error{server_startup_error, "jackctl_server_open()"};

    // read back actual params
    name_ = param_name.get_as<std::string>();
    realtime_ = param_realtime.get_as<bool>();
    if (realtime_) priority_ = param_priority.get_as<int>();
    driver.get_params(&*server_, driver_);

    success = jackctl_server_start(&*server_);
    if (!success) throw jack::error{server_startup_error, "jackctl_server_start()"};
}

////////////////////////////////////////////////////////////////////////////////
server::~server()
{
    if (server_)
    {
        jackctl_server_stop(&*server_);
        jackctl_server_close(&*server_);
        server_.reset();
    }
}

////////////////////////////////////////////////////////////////////////////////
void server::wait_for_signal()
{
    auto signals = jackctl_setup_signals(0);
    jackctl_wait_signals(signals);
}

////////////////////////////////////////////////////////////////////////////////
}
