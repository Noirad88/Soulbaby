//
//  TMXTile.h
//  TMXLoader
//
//  Created by Marty on 06/09/2015.
//  Copyright (c) 2015 Martin Grant. All rights reserved.
//  Available under MIT license. See License.txt for more information.
//
//  Uses RapidXML for file parsing.
//  Copyright (c) 2006, 2007 Marcin Kalicinski
//  http://rapidxml.sourceforge.net/
//  See /RapidXML/License.txt for more information.
//
//  www.midnightpacific.com
//  contact@midnightpacific.com
//  @_martingrant
//  http://bitbucket.org/martingrant/tmxloader
//

#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

class TMXObject
{
    /* Class constructors & destructors */
public:
    TMXObject(unsigned int objectID, std::string type, unsigned int gid, unsigned int x, unsigned int y, unsigned int width, unsigned int height, std::unordered_map<std::string, std::string>& propertiesMap);
	~TMXObject();

    /* Getter functions */
public:
    unsigned int getTileID();
    std::string getType();
    unsigned int getGID();
    unsigned int getXposition();
    unsigned int getYposition();
    unsigned int getWidth();
    unsigned int getHeight();
    std::string getProperty(std::string propertyName);
    
    /* Debug functions */
public:
	void printData();

    /* Tile variables */
private:
	unsigned int m_objectID;
    std::string m_type;
    unsigned int m_gid;
    unsigned int m_x;
    unsigned int m_y;
    unsigned int m_width;
    unsigned int m_height;
    
    /* User-defined properties */
private:
	std::unordered_map<std::string, std::string> m_propertiesMap;
};

