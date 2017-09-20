#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <vector>

class QueryResult
{
    friend std::ostream & print(std::ostream &, const QueryResult &);
public:
    using line_no = std::vector<std::string>::size_type;
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

std::ostream & print(std::ostream &, const QueryResult &);