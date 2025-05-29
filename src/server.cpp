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
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
server::server(const std::string& name, const server_options& options) :
    server_{jackctl_server_create(nullptr, nullptr)}
{
    if (!server_) throw jack::error{EINVAL, "jackctl_server_create()"};

    params_ = extract_from(jackctl_server_get_parameters(server_));

    find(params_, "name").value(name);
    if (options.realtime) find(params_, "realtime").value(*options.realtime);
    if (options.priority) find(params_, "realtime-priority").value(*options.priority);

    // TODO: set up driver

    auto success = jackctl_server_open(server_, nullptr);
    if (!success) throw jack::error{EINVAL, "jackctl_server_open()"};

    success = jackctl_server_start(server_);
    if (!success) throw jack::error{EINVAL, "jackctl_server_start()"};
}

////////////////////////////////////////////////////////////////////////////////
server::~server()
{
    if (server_)
    {
        jackctl_server_stop(server_);
        jackctl_server_close(server_);

        jackctl_server_destroy(server_);
        server_ = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
server::server(server&& rhs) :
    server_{ rhs.server_ },
    params_{ std::move(rhs.params_) }
{
    rhs.server_ = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
server& server::operator=(server&& rhs)
{
    server::~server();
    server_ = rhs.server_;
    params_ = std::move(rhs.params_);
    rhs.server_ = nullptr;
    return *this;
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
}
