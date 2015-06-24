#include <iostream>

std::string s=R""(#include <iostream>

std::string s=R""(~;

int main()
{
    for(int i=0;i<s.size();++i)
    {
        if(s[i]=='~' && s[i-1]=='(')
        {
            std::cout<<s;
            std::cout<<")\"\"";
        }
        else
            std::cout<<s[i];
    }
    return 0;
}
)"";

int main()
{
    for(int i=0;i<s.size();++i)
    {
        if(s[i]=='~' && s[i-1]=='(')
        {
            std::cout<<s;
            std::cout<<")\"\"";
        }
        else
            std::cout<<s[i];
    }
    return 0;
}
