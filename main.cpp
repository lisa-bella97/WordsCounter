#include <boost/process/context.hpp>
#include <boost/process/operations.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <regex>

namespace bp = boost::process;

bp::child start_child(std::string path)
{
    bp::context ctx;
    ctx.stdout_behavior = bp::capture_stream();
    ctx.environment = bp::self::get_environment();

    return bp::launch(path, std::vector<std::string>(1), ctx);
}

int main(int argc, char ** argv)
{
    if (argc != 2)
        return 0;

    bp::child child = start_child(argv[1]);

    bp::pistream & is = child.get_stdout();

    std::string line;
    std::regex pattern("\\S+\\s*");
    std::smatch matches;
    auto words_number = 0;

    while (std::getline(is, line))
    {
        while (std::regex_search(line, matches, pattern))
        {
            words_number += matches.size();
            line = matches.suffix().str();
        }
    }

    std::cout << "The number of words: " << words_number << std::endl;

    bp::status status = child.wait();

    return status.exited() ? status.exit_status() : EXIT_FAILURE;
}