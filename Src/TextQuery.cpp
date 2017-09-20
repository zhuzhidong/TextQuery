#include "TextQuery.h"
#include <sstream>
#include <algorithm>
#include <cctype>

TextQuery::TextQuery(std::ifstream & is) : file(new std::vector<std::string>)
{
    std::string text;
    while (std::getline(is, text))
    {
        file->push_back(text);
        size_t n = file->size() - 1;
        std::istringstream line(text);
        std::string word;
        while (line >> word)
        {
            word.erase(
                word.begin(),
                std::find_if_not(word.begin(), word.end(),
                    [](const char &ch) { return std::ispunct(ch); })
                      );
            word.erase(
                std::find_if(word.begin(), word.end(),
                    [](const char &ch) { return std::ispunct(ch); }),
                word.end()
                      );
            auto &lines = wm[word];
            if (!lines)
                lines.reset(new std::set<line_no>);
            lines->insert(n);
        }
    }
}

QueryResult TextQuery::query(const std::string & sought) const
{
    static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);
    auto loc = wm.find(sought);
    if (loc == wm.end())
        return QueryResult(sought, nodata, file);
    else
        return QueryResult(sought, loc->second, file);
}
