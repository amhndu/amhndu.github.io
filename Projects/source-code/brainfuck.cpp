#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "Argument required for input.\n";
        return EXIT_FAILURE;
    }
    std::fstream file(argv[1]);
    std::vector<char> data(30000);
    std::size_t cell_ptr = 0;
    std::string input;
    std::string line;
    while(std::getline(file, line))
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
