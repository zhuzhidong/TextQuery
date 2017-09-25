#include "Query.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

std::ostream & operator<<(std::ostream &os, const Query &query)
{
    return os << query.rep();
}

QueryResult NotQuery::eval(const TextQuery &text) const
{
    auto result = query.eval(text);
    auto ret_lines = std::make_shared<std::set<line_no>>();
    auto beg = result.begin(), end = result.end();
    auto sz = result.get_file()->size();
    for (size_t n = 0; n != sz; ++n)
    {
        if (beg == end || *beg != n)
            ret_lines->insert(n);
        else if (beg != end)
            ++beg;
    }
    return QueryResult(rep(), ret_lines, result.get_file());
}

QueryResult AndQuery::eval(const TextQuery &text) const
{
    auto right = rhs.eval(text), left = lhs.eval(text);
    auto ret_lines = std::make_shared<std::set<line_no>>();
    std::set_intersection(left.begin(), left.end(),
                          right.begin(), right.end(),
                          std::inserter(*ret_lines, ret_lines->begin()));
    return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult OrQuery::eval(const TextQuery &text) const
{
    auto right = rhs.eval(text), left = lhs.eval(text);
    auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), left.end());
    ret_lines->insert(right.begin(), right.end());
    return QueryResult(rep(), ret_lines, left.get_file());
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
        std::cout << "Executing Query for: " << CombinedQuery.top() << std::endl;
        print(std::cout, CombinedQuery.top().eval(tq)) << std::endl;
    }
}