////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "jack++/error.hpp"
#include "jack++/port.hpp"

#include <jack/jack.h>

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

auto jack_port_register_helper(jack_client* client, const std::string& name, jack::dir dir)
{
    int flags = (dir == jack::in) ? JackPortIsInput : JackPortIsOutput;
    return jack_port_register(client, name.data(), JACK_DEFAULT_AUDIO_TYPE, flags, 0);
}

auto jack_port_unregister_helper(jack_client* client)
{
    return [=](jack_port* port) { jack_port_unregister(client, port); };
}

}

////////////////////////////////////////////////////////////////////////////////
port::port(jack_client* client, const std::string& name, jack::dir dir) :
    port_{ jack_port_register_helper(client, name, dir), jack_port_unregister_helper(client) }
{
    if (!port_) throw jack::error{EACCES, "jack_port_register()"};
}

////////////////////////////////////////////////////////////////////////////////
std::string port::name() const { return jack_port_name(&*port_); }
bool port::is_physical() const { return jack_port_flags(&*port_) & JackPortIsPhysical; }

jack::buffer port::buffer(std::size_t size) const
{
    auto buffer = static_cast<sample*>(jack_port_get_buffer(&*port_, size));
    return { buffer, size };
}

////////////////////////////////////////////////////////////////////////////////
}

