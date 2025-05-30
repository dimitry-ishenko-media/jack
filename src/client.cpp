////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "client.hpp"
#include "error.hpp"

#include <jack/jack.h>

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

auto jack_client_open_helper(const std::string& name, const client_options& options)
{
    int flags = JackNoStartServer;
    const char* server_name = nullptr;
    if (options.server_name)
    {
        flags |= JackServerName;
        server_name = options.server_name->data();
    }

    jack_status_t status;
    return jack_client_open(name.data(), static_cast<jack_options_t>(flags), &status, server_name);
}

}

////////////////////////////////////////////////////////////////////////////////
client::client(const std::string& name, const client_options& options) :
    client_{ jack_client_open_helper(name, options), &jack_client_close }
{
    if (!client_) throw jack::error{EACCES, "jack_client_open()"};

    auto ev = jack_set_process_callback(&*client_, &dispatch_process_callback, this);
    if (ev) throw jack::error{ev, "jack_set_process_callback()"};

    ev = jack_activate(&*client_);
    if (ev) throw jack::error{ev, "jack_activate()"};
}

////////////////////////////////////////////////////////////////////////////////
client::~client()
{
    if (client_)
    {
        jack_deactivate(&*client_);
        client_.reset();
    }
}

////////////////////////////////////////////////////////////////////////////////
std::string client::name() const
{
    if (auto name = jack_get_client_name(&*client_)) return name;
    return { };
}

////////////////////////////////////////////////////////////////////////////////
void client::on_data(process_callback callback)
{
    // don't store empty callback to avoid extra check in dispatch_process_callback()
    if (!callback) callback_.store(nullptr);
    else callback_.store(std::make_shared<process_callback>( std::move(callback)) );
}

////////////////////////////////////////////////////////////////////////////////
int client::dispatch_process_callback(unsigned size, void* ctx)
{
    if (auto cl = reinterpret_cast<client*>(ctx))
        if (auto callback = cl->callback_.load()) (*callback)(size);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
}
