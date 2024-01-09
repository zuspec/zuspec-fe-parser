/*
 * TaskCalculateFieldOffset.cpp
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
#include "zsp/parser/impl/TaskResolveSymbolPathRef.h"
#include "TaskCalculateFieldOffset.h"


namespace zsp {
namespace fe {
namespace parser {


TaskCalculateFieldOffset::TaskCalculateFieldOffset(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("zsp::fe::parser::TaskCalculateFieldOffset", m_ctxt->getDebugMgr());
}

TaskCalculateFieldOffset::~TaskCalculateFieldOffset() {

}

TaskCalculateFieldOffset::Res TaskCalculateFieldOffset::calculate(
        ast::IScopeChild    *scope,
        int32_t             field_idx,
        int32_t             super_idx) {
    DEBUG_ENTER("calculate");
    m_field_cnt = 0;
    m_depth = 0;
    m_super_idx = (super_idx < 0)?0:super_idx;
    m_field_idx = field_idx;
    m_hit_field = false;

    scope->accept(m_this);

    DEBUG_LEAVE("calculate field_idx=%d", m_field_cnt);
    return {
        .index=m_field_cnt,
        .target=m_target
    };
}

void TaskCalculateFieldOffset::visitDataTypeUserDefined(ast::IDataTypeUserDefined *i) {
    DEBUG_ENTER("visitDataTypeUserDefined");
    ast::IScopeChild *super_t = zsp::parser::TaskResolveSymbolPathRef(
        m_ctxt->getDebugMgr(),
        m_ctxt->rootSymScope()
    ).resolve(i->getType_id()->getTarget());
    super_t->accept(m_this);
    DEBUG_LEAVE("visitDataTypeUserDefined");
}

void TaskCalculateFieldOffset::visitField(ast::IField *i) {
    DEBUG_ENTER("visitField");
    m_field_cnt++;
    DEBUG_LEAVE("visitField");
}

void TaskCalculateFieldOffset::visitFieldCompRef(ast::IFieldCompRef *i) {
    DEBUG_ENTER("visitFieldFieldCompRef");
    m_field_cnt++;
    DEBUG_LEAVE("visitFieldFieldCompRef");
}

void TaskCalculateFieldOffset::visitFieldRef(ast::IFieldRef *i) {
    DEBUG_ENTER("visitFieldFieldRef");
    m_field_cnt++;
    DEBUG_LEAVE("visitFieldFieldRef");
}

void TaskCalculateFieldOffset::visitFieldClaim(ast::IFieldClaim *i) {
    DEBUG_ENTER("visitFieldFieldClaim");
    m_field_cnt++;
    DEBUG_LEAVE("visitFieldFieldClaim");
}

void TaskCalculateFieldOffset::visitProceduralStmtDataDeclaration(ast::IProceduralStmtDataDeclaration *i) {
    DEBUG_ENTER("visitProceduralStmtDataDeclaration");
    i->getDatatype()->accept(m_this);
    DEBUG_LEAVE("visitProceduralStmtDataDeclaration");
}

void TaskCalculateFieldOffset::visitSymbolChildrenScope(ast::ISymbolChildrenScope *i) {
    DEBUG_ENTER("visitSymbolChildrenScope");
    DEBUG_LEAVE("visitSymbolChildrenScope");
}

void TaskCalculateFieldOffset::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope depth=%d", m_depth);
    ast::ITypeScope *ts = dynamic_cast<ast::ITypeScope *>(i->getTarget());
    if (ts->getSuper_t()) {
        ast::IScopeChild *super_t = zsp::parser::TaskResolveSymbolPathRef(
            m_ctxt->getDebugMgr(),
            m_ctxt->rootSymScope()
        ).resolve(ts->getSuper_t()->getTarget());
        DEBUG_ENTER("visit Super");
        m_depth++;
        super_t->accept(m_this);
        m_depth--;
        DEBUG_LEAVE("visit Super");
    }

    if (!m_hit_field) {
        for (uint32_t ii=0; ii<i->getChildren().size(); ii++) {
            DEBUG("m_super_idx=%d m_depth=%d m_field_idx=%d ii=%d",
                m_super_idx, m_depth, m_field_idx, ii);
            if (m_super_idx == m_depth && m_field_idx == ii) {
                m_target = i->getChildren().at(ii).get();
                m_hit_field = true;
                break;
            } else {
                i->getChildren().at(ii)->accept(m_this);
            }
        }
    }
    DEBUG_LEAVE("visitSymbolTypeScope");
}

dmgr::IDebug *TaskCalculateFieldOffset::m_dbg = 0;

}
}
}
