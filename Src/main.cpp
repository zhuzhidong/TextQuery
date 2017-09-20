#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include "TextQuery.h"
#include "Query.h"

void runQueries(std::ifstream & infile);

void runOOPQueries(std::ifstream & infile);

void main(int argc, char **argv)
{
    std::ifstream infile(argv[1], std::fstream::in);
    //runQueries(infile);
    runOOPQueries(infile);
}

void runQueries(std::ifstream & infile)
{
    TextQuery tq(infile);
    while (true)
    {
        std::cout << "enter word to look for, or q to quit: ";
        std::string s;
        if (!(std::cin >> s) || s == "q") break;
        print(std::cout, tq.query(s)) << std::endl;
    }
}

void runOOPQueries(std::ifstream & infile)
{
    TextQuery tq(infile);
    while (true)
    {
        std::cout << "enter query to look for, or q to quit: ";
        std::string query_line;
        if (!(std::getline(std::cin, query_line)) || query_line == "q") break;
        std::istringstream line(query_line);
        std::string str;
        std::vector<std::string> PostfixNotation;
        std::stack<std::string> Operand;
        while (line >> str)
        {
            if ("~" == str)
                Operand.push(str);
            else if ("&" == str || "|" == str)
            {
                if (Operand.empty())
                    Operand.push(str);
                else if ("(" == Operand.top())
                    Operand.push(str);
                else
                {
                    while ("(" != Operand.top() && !Operand.empty())
                    {
                        PostfixNotation.push_back(Operand.top());
                        Operand.pop();
                    }
                }
            }
            else if ("(" == str)
                Operand.push(str);
            else if (")" == str)
            {
                while ("(" != Operand.top())
                {
                    PostfixNotation.push_back(Operand.top());
                    Operand.pop();
                }
                Operand.pop();
            }
            else
                PostfixNotation.push_back(str);
        }
        while (!Operand.empty())
        {
            PostfixNotation.push_back(Operand.top());
            Operand.pop();
        }
        std::stack<Query> CombinedQuery;
        for (auto &str : PostfixNotation)
        {
            if ("~" == str)
            {
                Query temp = ~CombinedQuery.top();
                CombinedQuery.pop();
                CombinedQuery.push(temp);
            }
            else if ("&" == str)
            {
                Query temp1 = CombinedQuery.top();
                CombinedQuery.pop();
                Query temp2 = CombinedQuery.top();
                CombinedQuery.pop();
                CombinedQuery.push(temp1 & temp2);
            }
            else if ("|" == str)
            {
                Query temp1 = CombinedQuery.top();
                CombinedQuery.pop();
                Query temp2 = CombinedQuery.top();
                CombinedQuery.pop();
                CombinedQuery.push(temp1 | temp2);
            }
            else
                CombinedQuery.emplace(str);
        }
        std::cout << "Executing Query for: "<< CombinedQuery.top() << std::endl;
        print(std::cout, CombinedQuery.top().eval(tq)) << std::endl;
    }
}
