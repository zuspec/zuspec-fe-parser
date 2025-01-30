/*
 * TaskBuildExprStatic.cpp
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
#include "TaskBuildEnumType.h"
#include "TaskBuildExprStatic.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildExprStatic::TaskBuildExprStatic(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("zsp::fe::parser::TaskBuildExprStatic", ctxt->getDebugMgr());
}

TaskBuildExprStatic::~TaskBuildExprStatic() {

}

vsc::dm::ITypeExpr *TaskBuildExprStatic::build(ast::IScopeChild *item) {
    DEBUG_ENTER("build");
    m_ret = 0;

    item->accept(this);

    DEBUG_LEAVE("build %p", m_ret);
    return m_ret;
}

void TaskBuildExprStatic::visitEnumItem(ast::IEnumItem *i) {
    DEBUG_ENTER("visitEnumItem");

    // Ensure the type has been created
    ast::ISymbolEnumScope *enum_s = i->getUpper();
    ast::IScopeChild *enum_t = enum_s->getTarget();
    vsc::dm::IDataType *enum_dt;

    if (!(enum_dt=m_ctxt->findType(enum_t))) {
        enum_dt = TaskBuildEnumType(m_ctxt).build("", enum_s);
        m_ctxt->addType(enum_s, enum_dt);
    }

    int32_t enum_id = dynamic_cast<vsc::dm::IDataTypeEnum *>(
        enum_dt)->getEnumeratorId(i->getName()->getId());

    // TODO: Need an expression that references an enumerator
    m_ret = m_ctxt->ctxt()->mkTypeExprEnumRef(
        dynamic_cast<vsc::dm::IDataTypeEnum *>(enum_dt),
        enum_id);

    DEBUG("type: %p", enum_dt);
    DEBUG_LEAVE("visitEnumItem");
}

dmgr::IDebug *TaskBuildExprStatic::m_dbg = 0;

}
}
}
