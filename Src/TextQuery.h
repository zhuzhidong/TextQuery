#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <map>
#include <set>
#include "QueryResult.h"

class TextQuery
{
    using line_no = QueryResult::line_no;
public:
    TextQuery(std::ifstream &);
    QueryResult query(const std::string &) const;
private:
    std::shared_ptr<std::vector<std::string>> file;
    std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};