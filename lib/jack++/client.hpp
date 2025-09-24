////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef JACK_CLIENT_HPP
#define JACK_CLIENT_HPP

////////////////////////////////////////////////////////////////////////////////
#include "jack++/port.hpp"

#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

struct _jack_client;
using jack_client = _jack_client;

////////////////////////////////////////////////////////////////////////////////
namespace jack
{

////////////////////////////////////////////////////////////////////////////////
using names = std::vector<std::string>;
using process_callback = std::function<void(const input_ports&, const output_ports&, std::size_t)>;

////////////////////////////////////////////////////////////////////////////////
struct client_options
{
    std::optional<std::string> server_name;
    names inputs, outputs;
};

class client
{
public:
    ////////////////////
    explicit client(const std::string& name, const client_options& = { });
    ~client();

    // NB: Instances of this class cannot be movable, as their addresses
    // are passed to the JACK server and are used in the callback dispatch.

    ////////////////////
    std::string name() const;
    void on_data(process_callback);

    ////////////////////
    constexpr auto& inputs() const noexcept { return inputs_; }
    const input_port& input(const std::string& name) const;

    const output_port& output(const std::string& name) const;
    constexpr auto& outputs() const noexcept { return outputs_; }

    names find_ports(const std::optional<std::string>& pattern, jack::dir dir) const
    {
        return find_ports(pattern.value_or(""), false, dir);
    }
    names find_ports(const std::optional<std::string>& pattern, physical_t, jack::dir dir) const
    {
        return find_ports(pattern.value_or(""), true, dir);
    }

    void connect(const output_port& from, const  input_port& to) { connect(from.name(), to.name()); }
    void connect(const output_port& from, const std::string& to) { connect(from.name(), to); }
    void connect(const std::string& from, const  input_port& to) { connect(from, to.name()); }
    void connect(const std::string& from, const std::string& to);

private:
    ////////////////////
    std::unique_ptr<jack_client, int(*)(jack_client*)> client_;
    // use shared_ptr to allow atomic store/load
    std::atomic<std::shared_ptr<process_callback>> callback_;

    input_ports inputs_;
    output_ports outputs_;
    names find_ports(const std::string& pattern, bool physical, jack::dir) const;

    static int dispatch_process_callback(unsigned, void*);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
