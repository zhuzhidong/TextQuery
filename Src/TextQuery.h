#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <map>
#include <set>
#include <iostream>
#include <vector>

class TextQuery
{
public:
    using line_no = std::vector<std::string>::size_type;
    class QueryResult;
    TextQuery(std::ifstream &);
    QueryResult query(const std::string &) const;
private:
    std::shared_ptr<std::vector<std::string>> file;
    std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

class TextQuery::QueryResult
{
    friend std::ostream & print(std::ostream &, const QueryResult &);
public:
    QueryResult(const std::string s,
                std::shared_ptr<std::set<line_no>> p,
                std::shared_ptr<const std::vector<std::string>> f) :
            sought(s), lines(p), file(f) { }
    std::set<line_no>::const_iterator begin() const { return lines->begin(); }
    std::set<line_no>::const_iterator end() const { return lines->end(); }
    std::shared_ptr<const std::vector<std::string>> get_file() const { return file; }
private:
    std::string sought;
    std::shared_ptr<std::set<line_no>> lines;
    std::shared_ptr<const std::vector<std::string>> file;
};

std::ostream & print(std::ostream &, const TextQuery::QueryResult &);