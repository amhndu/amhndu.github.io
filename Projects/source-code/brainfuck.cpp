// No rights reserved.
// Author: Amish Naidu (amhndu@gmail.com)
// Pass a file with brainfuck source as an argument
// Pass "-" or nothing to use stdin as input (don't forget to generate EOF at the end!)

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int main(int argc, char** argv)
{
    std::istream *in;
    if(argc < 2)
    {
        in = &std::cin;
    }
    std::fstream file;
    if (argc > 1 && std::string(argv[1]) != "-")
    {
        file.open(argv[1]);
        if (file.is_open() && file.good())
            in = &file;
        else
        {
            std::cout << "Failure in opening source.\n";
            return 1;
        }
    }
    else
        in = &std::cin;
    
    std::vector<char> data(30000);
    std::size_t cell_ptr = 0;
    std::string input;
    std::string line;
    while(std::getline(*in, line))
    {
        input += line;
    }
    std::fill(data.begin(),data.end(),0);
    for(std::size_t i = 0; i < input.size(); ++i)
    {
        switch(input[i])
        {
            case '+':   ++data[cell_ptr]; break;
            case '-':   --data[cell_ptr]; break;
            case '>':   ++cell_ptr; break;
            case '<':   --cell_ptr; break;
            case '.':   std::cout << data[cell_ptr]; break;
            case ',':   std::cin >> data[cell_ptr]; break;
            case '[':
            {
                if(data[cell_ptr] == 0)
                {
                    int c = 0;
                    for(; i < input.size() ; ++i)
                    {
                        if(input[i] == '[') ++c;
                        else if(input[i] == ']' && --c == 0) break;
                    }
                }
            }
            break;
            case ']':
            {
                if(data[cell_ptr] != 0)
                {
                    int c = 0;
                    for(; i >= 0 ; --i)
                    {
                        if(input[i] == '[' && ++c == 0) break;
                        else if(input[i] == ']')    --c;
                    }
                }
            }
        }
    }
    return 0;
}
