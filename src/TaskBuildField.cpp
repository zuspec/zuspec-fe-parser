/*
 * TaskBuildField.cpp
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
#include "TaskBuildDataType.h"
#include "TaskBuildField.h"
#include "TaskGetDataTypeAssocData.h"
#include "dmgr/impl/DebugMacros.h"
#include "zsp/fe/parser/IElemFactoryAssocData.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildField::TaskBuildField(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("TaskBuildField", ctxt->getDebugMgr());
}

TaskBuildField::~TaskBuildField() {

}

vsc::dm::ITypeField *TaskBuildField::build(ast::IScopeChild *f) {
    DEBUG_ENTER("build");
    m_ret = 0;
    f->accept(m_this);
    DEBUG_LEAVE("build");
    return m_ret;
}

void TaskBuildField::visitField(ast::IField *i) { 
    DEBUG_ENTER("visitField %s attr=0x%08x", 
        i->getName()->getId().c_str(),
        i->getAttr());
    zsp::ast::IDataType *adt = i->getType();
    zsp::ast::IAssocData *assoc_d = TaskGetDataTypeAssocData(m_ctxt).get(adt);
    vsc::dm::IDataType *dt = TaskBuildDataType(m_ctxt).build(i->getType());
    vsc::dm::TypeFieldAttr attr = vsc::dm::TypeFieldAttr::NoAttr;
    vsc::dm::ValRef init; // Default is Void

    if ((i->getAttr() & ast::FieldAttr::Rand) != ast::FieldAttr::NoFlags) {
        attr = attr | vsc::dm::TypeFieldAttr::Rand;
    }

    m_ret = 0;
    if (assoc_d) {
        IElemFactoryAssocData *elem_f = 
            dynamic_cast<IElemFactoryAssocData *>(assoc_d);
        m_ret = elem_f->mkTypeFieldPhy(
            m_ctxt,
            i->getName()->getId(),
            i->getType(),
            attr,
            init);
    }

    if (!m_ret) {
        m_ret = m_ctxt->ctxt()->mkTypeFieldPhy(
            i->getName()->getId(),
            dt,
            false,
            attr,
            init);
    }

    DEBUG_LEAVE("visitField");
}
    
void TaskBuildField::visitFieldCompRef(ast::IFieldCompRef *i) { 
    DEBUG_ENTER("visitFieldCompRef");

    DEBUG_LEAVE("visitFieldCompRef");
}
    
void TaskBuildField::visitFieldRef(ast::IFieldRef *i) { 
    DEBUG_ENTER("visitFieldRef");

    DEBUG_LEAVE("visitFieldRef");
}
    
void TaskBuildField::visitFieldClaim(ast::IFieldClaim *i) { 
    DEBUG_ENTER("visitFieldClaim");

    DEBUG_LEAVE("visitFieldClaim");
}

dmgr::IDebug *TaskBuildField::m_dbg = 0;

}
}
}
