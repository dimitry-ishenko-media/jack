////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "error.hpp"
#include "server.hpp"

#include <jack/control.h>

////////////////////////////////////////////////////////////////////////////////
void jackctl_server_delete::operator()(jackctl_server* server)
{
    jackctl_server_destroy(server);
}

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
server::server(const std::string& name, const jack::driver& driver, const server_options& options) :
    server_{jackctl_server_create(nullptr, nullptr)}
{
    if (!server_) throw jack::error{EACCES, "jackctl_server_create()"};

        params_ = extract_from(jackctl_server_get_parameters(&*server_));

        find(params_, "name").value(name);
        if (options.realtime) find(params_, "realtime").value(*options.realtime);
        if (options.priority) find(params_, "realtime-priority").value(*options.priority);

        jackctl_driver* driver_;
        driver.setup(&*server_, &driver_);

        auto success = jackctl_server_open(&*server_, driver_);
        if (!success) throw jack::error{EACCES, "jackctl_server_open()"};

        success = jackctl_server_start(&*server_);
        if (!success) throw jack::error{EACCES, "jackctl_server_start()"};
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
std::string server::name() const { return find(params_, "name").get<std::string>(); }
bool server::realtime() const { return find(params_, "realtime").get<bool>(); }

std::optional<int> server::priority() const
{
    if (!realtime()) return { };
    return find(params_, "realtime-priority").get<int>();
}

////////////////////////////////////////////////////////////////////////////////
void server::wait_for_signal()
{
    auto signals = jackctl_setup_signals(0);
    jackctl_wait_signals(signals);
}

////////////////////////////////////////////////////////////////////////////////
}
