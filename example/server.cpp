#include <iostream>
#include <jack++.hpp>

using namespace std;

int main(int argc, char* argv[])
try
{
    jack::alsa_options options{
        .rate = 44100,
        .period = 128,
        .periods = 2
    };
    if (argc > 1) options.device = argv[1];

    jack::alsa_driver driver{options};
    jack::server server{"jack", driver};

    cout << endl;
    cout << "SERVER PARAMETERS" << endl;
    cout << "     name : " << server.name() << endl;
    cout << "real-time : " << (server.realtime() ? "yes" : "no") << endl;
    cout << " priority : " << server.priority().value_or(-1) << endl;
    cout << endl;

    cout << "DRIVER PARAMETERS" << endl;
    cout << "   device : " << driver.device()   << endl;
    cout << "  in-chan : " << driver.chan_in()  << endl;
    cout << " out-chan : " << driver.chan_out() << endl;
    cout << "     rate : " << driver.rate()     << endl;
    cout << "   period : " << driver.period()   << endl;
    cout << "  periods : " << driver.periods()  << endl;
    cout << endl;

    cout << "Press CTRL+C to exit." << endl;
    server.wait_for_signal();

    return 0;
}
catch (const jack::error& e)
{
    cerr << e.what() << endl;
    return 1;
}
