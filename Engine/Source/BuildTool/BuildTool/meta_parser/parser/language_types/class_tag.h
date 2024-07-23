//
// Created by Mavel Li on 8/12/23.
//

#pragma once
#include <vector>
#include <string>
#include "cursor/cursor.h"


class Cursor;

class class_tag
{
protected:
    std::vector<std::string> Tags;
public:
    std::string SourceFile;
    int SourceLine;
    std::string Name;
    bool Valid = false;

    class_tag() {};
    class_tag(const Cursor& cursor)
    {
        SourceFile = cursor.getSourceFile();
    	std::filesystem::path path(SourceFile);
    	SourceFile = path.lexically_normal().string();

        SourceLine = cursor.getSourceLine();
        Name = cursor.getDisplayName();
        if(Name.substr(0, 4) != "MTAG") return;
        Valid = true;
        std::string temp;
        std::stringstream stringstream { cursor.getDisplayName() };
        while (std::getline(stringstream, temp, '_')) {
            Tags.push_back(temp);
        }
    	std::swap(Tags.front(), Tags.back());
    }

    std::string GetClassName() const { return Tags[0]; }
    bool getFlag(std::string flag) const { return std::find(Tags.begin(), Tags.end(), flag) != Tags.end(); }
};
