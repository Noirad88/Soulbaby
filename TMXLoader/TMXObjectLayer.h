//
//  TMXTileLayer.h
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

#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>

#ifndef __TMXOBJECT_H_INCLUDED__
#define __TMXOBJECT_H_INCLUDED__
#include "TMXObject.h"
#endif


class TMXObject;

class TMXObjectLayer
{
	/* Class constructors & destructors */
public:
    TMXObjectLayer(std::string name, std::vector<TMXObject> tileVector);
	~TMXObjectLayer();

	/* Getter functions  */
public:
	std::string getName();
    std::vector<TMXObject> getObjectVector();
    
    /* Debug functions */
    
public:
	void printData();

	/* Layer variables */
private:
	std::string m_name;
    std::vector<TMXObject> m_objectVector;

	/* User-defined Properties */
private:
	std::unordered_map<std::string, std::string> m_layerProperties;
};