//
//  TMXTile.cpp
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

#include "TMXObject.h"


TMXObject::TMXObject(unsigned int objectID, std::string type, unsigned int gid, unsigned int x, unsigned int y, unsigned int width, unsigned int height, std::unordered_map<std::string, std::string>& propertiesMap) : m_objectID(objectID), m_type(type), m_gid(gid), m_x(x), m_y(y), m_width(width), m_height(height), m_propertiesMap(propertiesMap)
{
}


TMXObject::~TMXObject()
{
    m_propertiesMap.clear();
    std::unordered_map<std::string, std::string>().swap(m_propertiesMap);
}


unsigned int TMXObject::getTileID()
{
    return m_objectID;
}

std::string TMXObject::getType()
{
    return m_type;
}

unsigned int TMXObject::getGID()
{
    return m_gid;
}

unsigned int TMXObject::getXposition()
{
    return m_x;
}

unsigned int TMXObject::getYposition()
{
    return m_y;
}

unsigned int TMXObject::getWidth()
{
    return m_width;
}

unsigned int TMXObject::getHeight()
{
    return m_height;
}


std::string TMXObject::getProperty(std::string propertyName)
{
    std::unordered_map<std::string, std::string>::const_iterator iterator = m_propertiesMap.find(propertyName);
    
    if (iterator == m_propertiesMap.end())
    {
        std::cout << "TMXLoader: property '" << propertyName << "' not found." << std::endl;
    }
    else
    {
        return iterator->second;
    }
    
    return nullptr;
}


void TMXObject::printData()
{
	std::cout << "Tile ID: " << m_objectID << "\n Tile Properties: " << std::endl;
	for (auto index = m_propertiesMap.begin(); index != m_propertiesMap.end(); ++index)
	{
		std::cout << "\n" << index->first << " - " << index->second << std::endl;
	}
}
