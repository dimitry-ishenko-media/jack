////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "jack++/client.hpp"
#include "jack++/error.hpp"

#include <jack/jack.h>
#include <memory>

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
    if (!client_) throw jack::error{EINVAL, "jack_client_open()"};

    auto ev = jack_set_process_callback(&*client_, &dispatch_process_callback, this);
    if (ev) throw jack::error{ev, "jack_set_process_callback()"};

    for (auto&& name : options.inputs) inputs_.emplace_back(&*client_, name);
    for (auto&& name : options.outputs) outputs_.emplace_back(&*client_, name);

    ev = jack_activate(&*client_);
    if (ev) throw jack::error{ev, "jack_activate()"};
}

////////////////////////////////////////////////////////////////////////////////
client::~client()
{
    if (client_)
    {
        jack_deactivate(&*client_);

        inputs_.clear();
        outputs_.clear();

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
const input_port& client::input(const std::string& end) const
{
    auto port_name = name() + ':' + end;
    for (auto&& port : inputs_) if (port.name() == port_name) return port;
    throw jack::error{EINVAL, port_name};
}

////////////////////////////////////////////////////////////////////////////////
const output_port& client::output(const std::string& end) const
{
    auto port_name = name() + ':' + end;
    for (auto&& port : outputs_) if (port.name() == port_name) return port;
    throw jack::error{EINVAL, port_name};
}

////////////////////////////////////////////////////////////////////////////////
void client::connect(const std::string& from, const std::string& to)
{
    auto ev = jack_connect(&*client_, from.data(), to.data());
    if (ev) throw jack::error{ev, "jack_connect()"};
}

////////////////////////////////////////////////////////////////////////////////
names client::find_ports(const std::string& pattern, bool physical, jack::dir dir) const
{
    int flags = physical ? JackPortIsPhysical : 0;
    switch (dir)
    {
        case in : flags |= JackPortIsInput; break;
        case out: flags |= JackPortIsOutput; break;
        // if any other value, both in and out ports will be returned
    }

    jack::names names;
    std::unique_ptr<const char*, void(*)(void*)> ports{
        jack_get_ports(&*client_, pattern.data(), JACK_DEFAULT_AUDIO_TYPE, flags), &jack_free
    };
    if (ports) for (auto port = &*ports; *port; ++port) names.emplace_back(*port);
    return names;
}

////////////////////////////////////////////////////////////////////////////////
int client::dispatch_process_callback(unsigned size, void* ctx)
{
    if (auto instance = reinterpret_cast<client*>(ctx))
        if (auto callback = instance->callback_.load())
            (*callback)(instance->inputs(), instance->outputs(), size);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
}
