//
//  TMXObjectLayer.cpp
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

#include "TMXObjectLayer.h"


TMXObjectLayer::TMXObjectLayer(std::string name, std::vector<TMXObject> objectVector) : m_name(name), m_objectVector(objectVector)
{
}


TMXObjectLayer::~TMXObjectLayer()
{
    m_objectVector.clear();
    std::vector<TMXObject>().swap(m_objectVector);
    
    m_layerProperties.clear();
    std::unordered_map<std::string, std::string>().swap(m_layerProperties);
}


std::string TMXObjectLayer::getName()
{
	return m_name;
}


std::vector<TMXObject> TMXObjectLayer::getObjectVector()
{
    return m_objectVector;
}

void TMXObjectLayer::printData()
{
	std::cout << "\nName: " << m_name
		<< "\nLayer Properties:";
}