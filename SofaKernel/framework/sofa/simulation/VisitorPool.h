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
#include <sofa/simulation/PropagateEventVisitor.h>
#include <sofa/simulation/BehaviorUpdatePositionVisitor.h>
#include <sofa/simulation/AnimateVisitor.h>
#include <sofa/simulation/UpdateContextVisitor.h>
#include <sofa/simulation/UpdateMappingVisitor.h>
#include <sofa/simulation/UpdateBoundingBoxVisitor.h>
#include <sofa/simulation/MechanicalVisitor.h>

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
//    	std::map< char*, Visitor* >::iterator it = this->vPool.find(name);
//    	if ( it != this->vPool.end() ) {
//    		return *it;
//    	}
//    	else {
//    		V& visitor = new V();	//problème, le constructeur d'un visiteur n'est pas le Ctor par défaut
//    		this->vPool.insert(std::make_pair(name, visitor));
//			return visitor;
//    	}
//    }

    //si le visiteur existe, on fait un get visitor
    //sinon on crée le visitor et on l'ajoute à la liste correspondante
    template<class V>
    V* createNewVisitor(std::string name, const core::ExecParams* params) {
        std::map< std::string, Visitor* >::iterator it = vPool.find(name);
        if ( it == this->vPool.end() ) {
            V* visitor = new V( params );
            this->vPool.insert(std::make_pair(name, visitor));
            return visitor;
        }
        else {
            return it->second;
        }
    }

    template<class V>
    V* createNewVisitor(std::string name, const core::ExecParams* params, sofa::core::objectmodel::Event* ev) {
        std::map< std::string, Visitor* >::iterator it = vPool.find(name);
        if ( it == this->vPool.end() ) {
            V* visitor = new V( params, ev );
            this->vPool.insert(std::make_pair(name, visitor));
            return visitor;
        }
        else {
            return it->second;
        }
    }

    template<class V>
    V* createNewVisitor(std::string name, const core::ExecParams* params, SReal dt) {
        std::map< std::string, Visitor* >::iterator it = vPool.find(name);
        if ( it == this->vPool.end() ) {
            V* visitor = new V( params, dt );
            this->vPool.insert(std::make_pair(name, visitor));
            return visitor;
        }
        else {
            return it->second;
        }
    }


    bool isPresent(std::string name) {
        std::map< std::string, Visitor* >::iterator it = this->vPool.find(name);
        return (it != this->vPool.end());
    }
    Visitor* getVisitor( std::string name) {
        std::map< std::string, Visitor* >::iterator it = vPool.find(name);
        if( it != this->vPool.end() ) {
            return it->second;
        }
        else {
            return NULL;
        }
    }
    void eraseVisitor(std::string name) {
        std::map< std::string, Visitor* >::iterator it = this->vPool.find(name);
        this->vPool.erase(it);
    }



//    //template sur le type de visiteur
//    //si le visiteur existe, on fait un get visitor
//    //sinon on crée le visitor et on l'ajoute
//    template<class V>
//    V& addVisitor(std::string name, V& visitor) {
//        this->vPool.insert(std::make_pair(name, visitor));
//        return visitor;
//    }
//    void eraseVisitor(std::string name) {
//        std::map< std::string, Visitor* >::iterator it = this->vPool.find(name);
//    	this->vPool.erase(it);
//    }
//    Visitor* getVisitor( std::string name) {
//        std::map< std::string, Visitor* >::iterator it = vPool.find(name);
////    	if( it != this->vPool.end() ) {
//    		return it->second;
////    	}
////		return this->vPool[name];
//    }
//    bool isPresent(std::string name) {
//        std::map< std::string, Visitor* >::iterator it = this->vPool.find(name);
//        return (it != this->vPool.end());
//    }


    PropagateEventVisitor* addPEVisitor(std::string name, PropagateEventVisitor* visitor) {
        this->pevPool.insert(std::make_pair(name, visitor));
//        this->pevPool[name] = visitor;
        return visitor;
    }
    PropagateEventVisitor* getPEVisitor( std::string name) {
        std::map< std::string, PropagateEventVisitor* >::iterator it = pevPool.find(name);
        return it->second;
    }
    bool isPresentPE(std::string name) {
        std::map< std::string, PropagateEventVisitor* >::iterator it = this->pevPool.find(name);
        return (it != this->pevPool.end());
    }
    void erasePEVisitor(std::string name) {
        std::map< std::string, PropagateEventVisitor* >::iterator it = this->pevPool.find(name);
        this->pevPool.erase(it);
    }


    BehaviorUpdatePositionVisitor* addBUPVisitor(std::string name, BehaviorUpdatePositionVisitor* visitor) {
        this->bupvPool.insert(std::make_pair(name, visitor));
//        this->bupvPool[name] = visitor;
        return visitor;
    }
    BehaviorUpdatePositionVisitor* getBUPVisitor( std::string name) {
        std::map< std::string, BehaviorUpdatePositionVisitor* >::iterator it = bupvPool.find(name);
        return it->second;
    }
    bool isPresentBUP(std::string name) {
        std::map< std::string, BehaviorUpdatePositionVisitor* >::iterator it = this->bupvPool.find(name);
        return (it != this->bupvPool.end());
    }
    void eraseBUPEVisitor(std::string name) {
        std::map< std::string, BehaviorUpdatePositionVisitor* >::iterator it = this->bupvPool.find(name);
        this->bupvPool.erase(it);
    }


    AnimateVisitor* addAVisitor(std::string name, AnimateVisitor* visitor) {
        this->avPool.insert(std::make_pair(name, visitor));
//        this->avPool[name] = visitor;
        return visitor;
    }
    AnimateVisitor* getAVisitor( std::string name) {
        std::map< std::string, AnimateVisitor* >::iterator it = avPool.find(name);
        return it->second;
    }
    bool isPresentA(std::string name) {
        std::map< std::string, AnimateVisitor* >::iterator it = this->avPool.find(name);
        return (it != this->avPool.end());
    }
    void eraseAVisitor(std::string name) {
        std::map< std::string, AnimateVisitor* >::iterator it = this->avPool.find(name);
        this->avPool.erase(it);
    }


    UpdateSimulationContextVisitor* addUSCVisitor(std::string name, UpdateSimulationContextVisitor* visitor) {
        this->uscPool.insert(std::make_pair(name, visitor));
//        this->avPool[name] = visitor;
        return visitor;
    }
    UpdateSimulationContextVisitor* getUSCVisitor( std::string name) {
        std::map< std::string, UpdateSimulationContextVisitor* >::iterator it = uscPool.find(name);
        return it->second;
    }
    bool isPresentUSC(std::string name) {
        std::map< std::string, UpdateSimulationContextVisitor* >::iterator it = this->uscPool.find(name);
        return (it != this->uscPool.end());
    }
    void eraseUSCVisitor(std::string name) {
        std::map< std::string, UpdateSimulationContextVisitor* >::iterator it = this->uscPool.find(name);
        this->uscPool.erase(it);
    }


    UpdateMappingVisitor* addUMVisitor(std::string name, UpdateMappingVisitor* visitor) {
        this->umPool.insert(std::make_pair(name, visitor));
//        this->avPool[name] = visitor;
        return visitor;
    }
    UpdateMappingVisitor* getUMVisitor( std::string name) {
        std::map< std::string, UpdateMappingVisitor* >::iterator it = umPool.find(name);
        return it->second;
    }
    bool isPresentUM(std::string name) {
        std::map< std::string, UpdateMappingVisitor* >::iterator it = this->umPool.find(name);
        return (it != this->umPool.end());
    }
    void eraseUMVisitor(std::string name) {
        std::map< std::string, UpdateMappingVisitor* >::iterator it = this->umPool.find(name);
        this->umPool.erase(it);
    }


    UpdateBoundingBoxVisitor* addUBBVisitor(std::string name, UpdateBoundingBoxVisitor* visitor) {
        this->ubbPool.insert(std::make_pair(name, visitor));
//        this->avPool[name] = visitor;
        return visitor;
    }
    UpdateBoundingBoxVisitor* getUBBVisitor( std::string name) {
        std::map< std::string, UpdateBoundingBoxVisitor* >::iterator it = ubbPool.find(name);
        return it->second;
    }
    bool isPresentUBB(std::string name) {
        std::map< std::string, UpdateBoundingBoxVisitor* >::iterator it = this->ubbPool.find(name);
        return (it != this->ubbPool.end());
    }
    void eraseUBBVisitor(std::string name) {
        std::map< std::string, UpdateBoundingBoxVisitor* >::iterator it = this->ubbPool.find(name);
        this->ubbPool.erase(it);
    }


    MechanicalResetConstraintVisitor* addMRCVisitor(std::string name, MechanicalResetConstraintVisitor* visitor) {
        this->mrcPool.insert(std::make_pair(name, visitor));
//        this->avPool[name] = visitor;
        return visitor;
    }
    MechanicalResetConstraintVisitor* getMRCVisitor( std::string name) {
        std::map< std::string, MechanicalResetConstraintVisitor* >::iterator it = mrcPool.find(name);
        return it->second;
    }
    bool isPresentMRC(std::string name) {
        std::map< std::string, MechanicalResetConstraintVisitor* >::iterator it = this->mrcPool.find(name);
        return (it != this->mrcPool.end());
    }
    void eraseMRCVisitor(std::string name) {
        std::map< std::string, MechanicalResetConstraintVisitor* >::iterator it = this->mrcPool.find(name);
        this->mrcPool.erase(it);
    }



protected:
    std::map< std::string, Visitor* > vPool;
    std::map< std::string, PropagateEventVisitor* > pevPool;
    std::map< std::string, BehaviorUpdatePositionVisitor* > bupvPool;
    std::map< std::string, AnimateVisitor* > avPool;
    std::map< std::string, UpdateSimulationContextVisitor* > uscPool;
    std::map< std::string, UpdateMappingVisitor* > umPool;
    std::map< std::string, UpdateBoundingBoxVisitor* > ubbPool;
    std::map< std::string, MechanicalResetConstraintVisitor* > mrcPool;

};
} // namespace simulation

} // namespace sofa

#endif
