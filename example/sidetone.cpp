#include <algorithm>
#include <csignal>
#include <iostream>
#include <jack++.hpp>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
try
{
    cout << "Connecting to JACK server" << endl;

    jack::client_options options{
        .server_name = "jack",
        .inputs = {"capture"},
        .outputs = {"playback"},
    };
    jack::client client{"sidetone", options};

    client.on_data([&](const jack::input_ports& inputs, const jack::output_ports& outputs, std::size_t size)
    {
        auto in = inputs[0].buffer(size).as_bytes();
        auto out = outputs[0].buffer(size).as_bytes();
        std::copy(in.begin(), in.end(), out.begin());
    });

    cout << endl;
    cout << "SYSTEM PORTS" << endl;
    for (auto&& name : client.find_ports({}, jack::physical, jack::in | jack::out)) cout << "  " << name << endl;

    cout << endl;
    cout << "CLIENT PORTS" << endl;
    for (auto&& input : client.inputs()) cout << "  " << input.name() << endl;
    for (auto&& output : client.outputs()) cout << "  " << output.name() << endl;
    cout << endl;

    cout << "Connecting ports:" << endl;
    auto&& capture = client.input("capture");
    for (auto&& output : client.find_ports({}, jack::physical, jack::out))
    {
        cout << output << " => " << capture.name() << endl;
        client.connect(output, capture);
    }

    auto&& playback = client.output("playback");
    for (auto&& input : client.find_ports({}, jack::physical, jack::in))
    {
        cout << playback.name() << " => " << input << endl;
        client.connect(playback, input);
    }
    cout << endl;

    std::signal(SIGINT, [](int signal) {
        cout << "Received signal " << signal << ", exiting." << endl;
    });

    cout << "Press CTRL+C to exit." << endl;
    pause();

    return 0;
}
catch (const jack::error& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
