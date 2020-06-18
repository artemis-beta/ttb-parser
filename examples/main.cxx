#include "TTBParser.hxx"

int main(int argc, char** argv)
{
    std::string _file_name = (argc > 1) ? argv[1] : "";
    TTBParser::Parser* parser = new TTBParser::Parser;
    parser->ParseServices(_file_name);
    return 0;
}