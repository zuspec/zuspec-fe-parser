/*
 * TaskBuildDataType.cpp
 *
 * Copyright 2022 Matthew Ballance and Contributors
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
#include "vsc/dm/IDataTypeStruct.h"
#include "TaskBuildDataType.h"


namespace zsp {
namespace fe {
namespace parser {

TaskBuildDataType::TaskBuildDataType(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("TaskBuildDataType", dmgr);
    m_ctxt = 0;
}

TaskBuildDataType::~TaskBuildDataType() {

}

vsc::dm::IDataTypeStruct *TaskBuildDataType::build(
        IAst2ArlContext         *ctxt,
        ast::IScopeChild        *type) {
    DEBUG_ENTER("build");
    m_ctxt = ctxt;

    type->accept(this);
    DEBUG_LEAVE("build");
}

void TaskBuildDataType::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope");
    m_ctxt->pushSymScope(i);
    i->getTarget()->accept(this);
    m_ctxt->popSymScope();
    DEBUG_LEAVE("visitSymbolTypeScope");
}

void TaskBuildDataType::visitAction(ast::IAction *i) {
    DEBUG_ENTER("visitAction");
    if (!m_depth && !findType(i)) {
        // We're at top level and the type doesn't exist yet, so let's do it!
    
        std::string fullname = getNamespacePrefix() + i->getName()->getId();
        DEBUG("Building Action Type: %s", fullname.c_str());
        arl::dm::IDataTypeAction *action_t = m_ctxt->ctxt()->mkDataTypeAction(fullname);
        m_ctxt->ctxt()->addDataTypeAction(action_t);

        buildType(action_t, dynamic_cast<ast::ISymbolTypeScope *>(m_ctxt->symScope()));

        // Get the created type and connect up to its component
        if (m_type_s.size()) {
            arl::dm::IDataTypeComponent *comp_t = dynamic_cast<arl::dm::IDataTypeComponent *>(m_type_s.back());
            comp_t->addActionType(action_t);
        }
    }

    // Note: there won't be any other types declared inside an action
    DEBUG_LEAVE("visitAction");
}

void TaskBuildDataType::visitComponent(ast::IComponent *i) {
    DEBUG_ENTER("visitComponent");
    if (!m_depth) {
        arl::dm::IDataTypeComponent *comp_t = 0;
        if (!findType(i)) {
            // We're at top level and the type doesn't exist yet, so let's do it!
    
            std::string fullname = getNamespacePrefix() + i->getName()->getId();
            DEBUG("Building Component Type: %s", fullname.c_str());
            comp_t = m_ctxt->ctxt()->mkDataTypeComponent(fullname);
            m_ctxt->ctxt()->addDataTypeComponent(comp_t);

            buildType(comp_t, dynamic_cast<ast::ISymbolTypeScope *>(m_ctxt->symScope()));
        }

        // Now, back at depth 0, visit children to build out other types
        m_type_s.push_back(comp_t);
        for (std::vector<ast::IScopeChild *>::const_iterator
            it=m_ctxt->symScope()->getChildren().begin();
            it!=m_ctxt->symScope()->getChildren().end(); it++) {
            (*it)->accept(this);
        }
        m_type_s.pop_back();
    }
    DEBUG_LEAVE("visitComponent");
}

void TaskBuildDataType::visitStruct(ast::IStruct *i) {
    DEBUG_ENTER("visitStruct");
    if (!m_depth && !findType(i)) {
        // We're at top level and the type doesn't exist yet, so let's do it!
    
        std::string fullname = getNamespacePrefix() + i->getName()->getId();
        vsc::dm::IDataTypeStruct *struct_t = m_ctxt->ctxt()->mkDataTypeStruct(fullname);

        buildType(struct_t, dynamic_cast<ast::ISymbolTypeScope *>(m_ctxt->symScope()));
    }

    // Note: there won't be any other types declared inside a struct

    DEBUG_LEAVE("visitStruct");
}

void TaskBuildDataType::buildType(
        vsc::dm::IDataTypeStruct    *arl_type,
        ast::ISymbolTypeScope       *ast_type) {
    DEBUG_ENTER("buildType");
    m_depth++;

    m_field_off = 0;
    // Depth 1: build fields
    std::vector<int32_t> offset_l;
    m_type_s.push_back(arl_type);
    buildTypeFields(
        offset_l,
        arl_type,
        ast_type);

    m_type_s.pop_back();

    m_depth--;
    DEBUG_LEAVE("buildType");
}

void TaskBuildDataType::buildTypeFields(
    std::vector<int32_t>                &off_l,
    vsc::dm::IDataTypeStruct            *arl_type,
    ast::ISymbolTypeScope               *ast_type) {
        
    // Recurse first
    ast::ITypeScope *target_t = dynamic_cast<ast::ITypeScope *>(ast_type->getTarget());
    if (target_t->getSuper_t()) {
        ast::IScopeChild *super_t = resolvePath(target_t->getSuper_t()->getTarget());

        super_t->accept(this);
    }

    // Record how many fields are in 'super'
    off_l.push_back(arl_type->getFields().size());

}

std::string TaskBuildDataType::getNamespacePrefix() {
    std::string ret;
    for (std::vector<ast::ISymbolScope *>::const_iterator
        it=m_ctxt->symScopes().begin();
        it+1!=m_ctxt->symScopes().end(); it++) {
        if ((*it)->getName() != "") {
            ret += (*it)->getName();
            ret += "::";
        }
    }
    return ret;
}

vsc::dm::IDataTypeStruct *TaskBuildDataType::findType(ast::IScopeChild *ast_t) {
    std::map<ast::IScopeChild *,vsc::dm::IDataTypeStruct *>::const_iterator it;

    if ((it=m_datatype_m->find(ast_t)) != m_datatype_m->end()) {
        return it->second;
    } else {
        return 0;
    }
}

ast::IScopeChild *TaskBuildDataType::resolvePath(ast::ISymbolRefPath *ref) {
    ast::IScopeChild *ret = 0;
    ast::ISymbolScope *scope = m_ctxt->symScopes().at(0);

    for (uint32_t i=0; i<ref->getPath().size(); i++) {
        ret = scope->getChildren().at(i);

        if (i+1 < ref->getPath().size()) {
            scope = dynamic_cast<ast::ISymbolScope *>(ret);
        }
    }

    return ret;
}

dmgr::IDebug *TaskBuildDataType::m_dbg = 0;

}
}
}
