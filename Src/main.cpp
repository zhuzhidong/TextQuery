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
        std::stack<std::string> Operator;
        while (line >> str)
        {
            if ("~" == str)
                Operator.push(str);
            else if ("&" == str || "|" == str)
            {
                if (Operator.empty())
                    Operator.push(str);
                else if ("(" == Operator.top())
                    Operator.push(str);
                else
                {
                    while (!Operator.empty() && "(" != Operator.top()) //short circuit evaluation
                    {
                        PostfixNotation.push_back(Operator.top());
                        Operator.pop();
                    }
                    Operator.push(str);
                }
            }
            else if ("(" == str)
                Operator.push(str);
            else if (")" == str)
            {
                while ("(" != Operator.top())
                {
                    PostfixNotation.push_back(Operator.top());
                    Operator.pop();
                }
                Operator.pop();
            }
            else
                PostfixNotation.push_back(str);
        }
        while (!Operator.empty())
        {
            PostfixNotation.push_back(Operator.top());
            Operator.pop();
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
                CombinedQuery.push(temp2 & temp1);
            }
            else if ("|" == str)
            {
                Query temp1 = CombinedQuery.top();
                CombinedQuery.pop();
                Query temp2 = CombinedQuery.top();
                CombinedQuery.pop();
                CombinedQuery.push(temp2 | temp1);
            }
            else
                CombinedQuery.emplace(str);
        }
        std::cout << "Executing Query for: "<< CombinedQuery.top() << std::endl;
        print(std::cout, CombinedQuery.top().eval(tq)) << std::endl;
    }
}
