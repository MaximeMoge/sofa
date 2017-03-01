/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2017 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_SIMULATION_VISITORPOOL_H
#define SOFA_SIMULATION_VISITORPOOL_H

#include <sofa/simulation/simulationcore.h>


#include <sofa/helper/set.h>
#include <iostream>

#include <sofa/simulation/Visitor.h>

namespace sofa
{

namespace simulation
{


/// Base class for visitors propagated recursively through the scenegraph
class VisitorPool
{
public:

    VisitorPool() {

    }
//    virtual ~VisitorPool();

//    //template sur le type de visiteur
//    //si le visiteur existe, on fait un get visitor
//    //sinon on crée le visitor et on l'ajoute
//    template<class V>
//    V& addVisitor(char* name) {
//    	std::map< char*, Visitor& >::iterator it = this->vPool.find(name);
//    	if ( it != this->vPool.end() ) {
//    		return *it;
//    	}
//    	else {
//    		V& visitor = new V();	//problème, le constructeur d'un visiteur n'est pas le Ctor par défaut
//    		this->vPool.insert(std::make_pair(name, visitor));
//			return visitor;
//    	}
//    }
    //template sur le type de visiteur
    //si le visiteur existe, on fait un get visitor
    //sinon on crée le visitor et on l'ajoute
    template<class V>
    V& addVisitor(std::string name, V& visitor) {
		this->vPool.insert(std::make_pair(name, visitor));
		return visitor;
    }
    void eraseVisitor(std::string name) {
    	std::map< std::string, Visitor& >::iterator it = this->vPool.find(name);
    	this->vPool.erase(it);
    }
    Visitor& getVisitor( std::string name) {
    	std::map< std::string, Visitor& >::iterator it = vPool.find(name);
//    	if( it != this->vPool.end() ) {
    		return it->second;
//    	}
//		return this->vPool[name];
    }
    bool isPresent(std::string name) {
    	std::map< std::string, Visitor& >::iterator it = this->vPool.find(name);
    	return (it != this->vPool.end());
    }

protected:
    std::map< std::string, Visitor& > vPool;

};
} // namespace simulation

} // namespace sofa

#endif
