#include <iostream>
#include "mjpeg_server.h"
#include <boost/program_options.hpp>
#include "app.h"
namespace po = boost::program_options;

int main(int argc, const char *argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()("help", "")("verbose", "set log to verbose");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("verbose"))
    {
        app::log = app::level::verbose;
    }
    if (vm.count("help"))
    {
        std::cout << desc;
        return 1;
    }
    mjpeg_server(8812);
    return 0;
}
