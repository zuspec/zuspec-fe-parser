/*
 * TaskBuildDataTypeReg.cpp
 *
 * Copyright 2023 Matthew Ballance and Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may 
 * not use this file except in compliance with the License.  
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 *
 * Created on:
 *     Author:
 */
#include "dmgr/impl/DebugMacros.h"
#include "TaskBuildDataTypeReg.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildDataTypeReg::TaskBuildDataTypeReg(IAst2ArlContext *ctxt) : TaskBuildDataType(ctxt) {

}

TaskBuildDataTypeReg::~TaskBuildDataTypeReg() {

}

void TaskBuildDataTypeReg::visitComponent(ast::IComponent *i) {
    DEBUG_ENTER("visitComponent m_depth=%d", m_depth);
//    if (!m_depth) {
    i->getParams()->getParams().at(0)->accept(m_this);
#ifdef UNDEFINED
    arl::dm::IDataTypeComponent *comp_t = 0;
    if (!m_depth && !(m_type=findType(i->getParams()))) {
        // We're at top level and the type doesn't exist yet, so let's do it!
 
        std::string fullname = getNamespacePrefix() + i->getName()->getId();
        DEBUG("Building Component Type: %s", fullname.c_str());
        comp_t = m_ctxt->ctxt()->mkDataTypeComponent(fullname);
        m_ctxt->ctxt()->addDataTypeStruct(comp_t);
        m_ctxt->addType(m_ctxt->symScope(), comp_t);

        buildType(comp_t, dynamic_cast<ast::ISymbolTypeScope *>(m_ctxt->symScope()));

        // Now, back at depth 0, visit children to build out other types
        m_type_s.push_back(comp_t);
        for (std::vector<ast::IScopeChild *>::const_iterator
            it=m_ctxt->symScope()->getChildren().begin();
            it!=m_ctxt->symScope()->getChildren().end(); it++) {
            (*it)->accept(this);
        }
        m_type_s.pop_back();

        m_type = comp_t;
    }
#endif /* UNDEFINED */

    DEBUG_LEAVE("visitComponent");
}

#ifdef UNDEFINED
void TaskBuildDataTypeReg::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope %s", i->getName().c_str());
    ast::ITypeScope *i_ts = dynamic_cast<ast::ITypeScope *>(i->getTarget());

    if (i_ts->getParams()) {
        DEBUG("Type has parameters");
        if (i_ts->getParams()->getSpecialized()) {
            DEBUG("Type is specialized (%p)", i->getTarget());
            m_ctxt->pushSymScope(i);
            i->getTarget()->accept(this);
            m_ctxt->popSymScope();
        } else {
            DEBUG("Type is unspecialized (%d specializations)", i->getSpec_types().size());
            for (std::vector<ast::ISymbolTypeScopeUP>::const_iterator
                it=i->getSpec_types().begin();
                it!=i->getSpec_types().end(); it++) {
                (*it)->accept(this);
            }
        }
    } else {
        m_ctxt->pushSymScope(i);
        i->getTarget()->accept(this);
        m_ctxt->popSymScope();
    }

    DEBUG_LEAVE("visitSymbolTypeScope");
}
#endif /* UNDEFINED */

}
}
}
