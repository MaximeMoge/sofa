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
#include <sofa/simulation/DefaultAnimationLoop.h>
#include <sofa/core/ObjectFactory.h>

#include <sofa/simulation/AnimateVisitor.h>
#include <sofa/simulation/UpdateContextVisitor.h>
#include <sofa/simulation/UpdateMappingVisitor.h>
#include <sofa/simulation/PropagateEventVisitor.h>
#include <sofa/simulation/BehaviorUpdatePositionVisitor.h>
#include <sofa/simulation/AnimateBeginEvent.h>
#include <sofa/simulation/AnimateEndEvent.h>
#include <sofa/simulation/UpdateMappingEndEvent.h>
#include <sofa/simulation/UpdateBoundingBoxVisitor.h>

#include <sofa/helper/system/SetDirectory.h>
//#include <sofa/helper/system/PipeProcess.h>
#include <sofa/helper/AdvancedTimer.h>

#include <sofa/core/visual/VisualParams.h>

#include <stdlib.h>
#include <math.h>
#include <algorithm>

namespace sofa
{

namespace simulation
{

SOFA_DECL_CLASS(DefaultAnimationLoop)

int DefaultAnimationLoopClass = core::RegisterObject("The simplest animation loop, created by default when user do not put on scene")
        .add< DefaultAnimationLoop >()
        ;



DefaultAnimationLoop::DefaultAnimationLoop(simulation::Node* _gnode)
    : Inherit()
    , gnode(_gnode)
{
    //assert(gnode);
}

DefaultAnimationLoop::~DefaultAnimationLoop()
{

}

void DefaultAnimationLoop::init()
{
    if (!gnode)
        gnode = dynamic_cast<simulation::Node*>(this->getContext());
}

void DefaultAnimationLoop::setNode( simulation::Node* n )
{
    gnode=n;
}

void DefaultAnimationLoop::step(const core::ExecParams* params, SReal dt)
{
    if (dt == 0)
        dt = this->gnode->getDt();

    std::cout << "Beginning of DefaultAnimationLoop::step with dt =" << dt << std::endl;


#ifdef SOFA_DUMP_VISITOR_INFO
    simulation::Visitor::printNode("Step");
#endif

    {
//        AnimateBeginEvent ev ( dt );
//        PropagateEventVisitor act ( params, &ev );
//        gnode->execute ( act );

        AnimateBeginEvent ev ( dt );
//        visitorPool->createNewVisitor<PropagateEventVisitor>("PropagateEventVisitor", params, &ev);
//        gnode->execute ( visitorPool->getVisitor("PropagateEventVisitor" ) );
        if ( !visitorPool->isPresentPE("PropagateEventVisitor") ) {
            PropagateEventVisitor* act =new PropagateEventVisitor( params, &ev );
            visitorPool->addPEVisitor( "PropagateEventVisitor",act );
        }
        else {
            visitorPool->getPEVisitor("PropagateEventVisitor")->setParams(params, &ev);
        }
        gnode->execute ( visitorPool->getPEVisitor("PropagateEventVisitor") );
    }

    SReal startTime = gnode->getTime();


//    sofa::helper::AdvancedTimer::stepBegin("BehaviorUpdatePositionVisitor");
//    BehaviorUpdatePositionVisitor beh(params , dt);
//    gnode->execute ( beh );
//    sofa::helper::AdvancedTimer::stepEnd("BehaviorUpdatePositionVisitor");

    sofa::helper::AdvancedTimer::stepBegin("BehaviorUpdatePositionVisitor");
//    BehaviorUpdatePositionVisitor* beh = visitorPool->createNewVisitor<BehaviorUpdatePositionVisitor>("BehaviorUpdatePositionVisitor", params, dt);
//    gnode->execute ( beh );
    if ( !visitorPool->isPresentBUP("BehaviorUpdatePositionVisitor") ) {
        BehaviorUpdatePositionVisitor* beh = new BehaviorUpdatePositionVisitor( params, dt );
        visitorPool->addBUPVisitor( "BehaviorUpdatePositionVisitor",beh );
    }
    else {
        visitorPool->getBUPVisitor("BehaviorUpdatePositionVisitor")->setParams(params, dt);
    }
    gnode->execute ( visitorPool->getBUPVisitor("BehaviorUpdatePositionVisitor") );
    sofa::helper::AdvancedTimer::stepEnd("BehaviorUpdatePositionVisitor");


//    sofa::helper::AdvancedTimer::stepBegin("AnimateVisitor");
//    AnimateVisitor act(params, dt);
//    gnode->execute ( act );
//    sofa::helper::AdvancedTimer::stepEnd("AnimateVisitor");

    sofa::helper::AdvancedTimer::stepBegin("AnimateVisitor");
    if ( !visitorPool->isPresentA("AnimateVisitor") ) {
        AnimateVisitor* act = new AnimateVisitor( params, dt );
        visitorPool->addAVisitor( "AnimateVisitor",act );
    }
    else {
        visitorPool->getAVisitor("AnimateVisitor")->setParams(params, dt);
    }
    gnode->execute ( visitorPool->getAVisitor("AnimateVisitor") );
    sofa::helper::AdvancedTimer::stepEnd("AnimateVisitor");


//    sofa::helper::AdvancedTimer::stepBegin("UpdateSimulationContextVisitor");
//    gnode->setTime ( startTime + dt );
//    gnode->execute< UpdateSimulationContextVisitor >(params);
//    sofa::helper::AdvancedTimer::stepEnd("UpdateSimulationContextVisitor");

    sofa::helper::AdvancedTimer::stepBegin("UpdateSimulationContextVisitor");
    gnode->setTime ( startTime + dt );
    if ( !visitorPool->isPresentUSC("UpdateSimulationContextVisitor") ) {
        UpdateSimulationContextVisitor* asc = new UpdateSimulationContextVisitor( params);
        visitorPool->addUSCVisitor( "UpdateSimulationContextVisitor",asc );
    }
    else {
        visitorPool->getUSCVisitor("UpdateSimulationContextVisitor")->setParams(params);
    }
    gnode->execute ( visitorPool->getUSCVisitor("UpdateSimulationContextVisitor") );
    sofa::helper::AdvancedTimer::stepEnd("UpdateSimulationContextVisitor");

    {
//        AnimateEndEvent ev ( dt );
//        PropagateEventVisitor act ( params, &ev );
//        gnode->execute ( act );

        AnimateEndEvent ev ( dt );
        if ( !visitorPool->isPresentPE("PropagateEventVisitor") ) {
            PropagateEventVisitor* act =new PropagateEventVisitor( params, &ev );
            visitorPool->addPEVisitor( "PropagateEventVisitor",act );
        }
        else {
            visitorPool->getPEVisitor("PropagateEventVisitor")->setParams(params, &ev);
        }
        gnode->execute ( visitorPool->getPEVisitor("PropagateEventVisitor") );
    }

    sofa::helper::AdvancedTimer::stepBegin("UpdateMapping");
    //Visual Information update: Ray Pick add a MechanicalMapping used as VisualMapping
//    gnode->execute< UpdateMappingVisitor >(params);

    if ( !visitorPool->isPresentUM("UpdateMappingVisitor") ) {
        UpdateMappingVisitor* asc = new UpdateMappingVisitor( params);
        visitorPool->addUMVisitor( "UpdateMappingVisitor",asc );
    }
    else {
        visitorPool->getUMVisitor("UpdateMappingVisitor")->setParams(params);
    }
    gnode->execute ( visitorPool->getUMVisitor("UpdateMappingVisitor") );

    {
//        UpdateMappingEndEvent ev ( dt );
//        PropagateEventVisitor act ( params , &ev );
//        gnode->execute ( act );

        UpdateMappingEndEvent ev ( dt );
        if ( !visitorPool->isPresentPE("PropagateEventVisitor") ) {
            PropagateEventVisitor* act =new PropagateEventVisitor( params, &ev );
            visitorPool->addPEVisitor( "PropagateEventVisitor",act );
        }
        else {
            visitorPool->getPEVisitor("PropagateEventVisitor")->setParams(params, &ev);
        }
        gnode->execute ( visitorPool->getPEVisitor("PropagateEventVisitor") );
    }
    sofa::helper::AdvancedTimer::stepEnd("UpdateMapping");

#ifndef SOFA_NO_UPDATE_BBOX
    sofa::helper::AdvancedTimer::stepBegin("UpdateBBox");
//    gnode->execute< UpdateBoundingBoxVisitor >(params);

    if ( !visitorPool->isPresentUBB("UpdateBoundingBoxVisitor") ) {
        UpdateBoundingBoxVisitor* asc = new UpdateBoundingBoxVisitor( params);
        visitorPool->addUBBVisitor( "UpdateBoundingBoxVisitor",asc );
    }
    else {
        visitorPool->getUBBVisitor("UpdateBoundingBoxVisitor")->setParams(params);
    }
    gnode->execute ( visitorPool->getUBBVisitor("UpdateBoundingBoxVisitor") );
    sofa::helper::AdvancedTimer::stepEnd("UpdateBBox");
#endif
#ifdef SOFA_DUMP_VISITOR_INFO
    simulation::Visitor::printCloseNode("Step");
#endif


}


} // namespace simulation

} // namespace sofa
