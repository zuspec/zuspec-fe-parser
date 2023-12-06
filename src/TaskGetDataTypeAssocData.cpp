/*
 * TaskGetDataTypeAssocData.cpp
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
#include "TaskGetDataTypeAssocData.h"
#include "zsp/parser/impl/TaskResolveSymbolPathRef.h"

namespace zsp {
namespace fe {
namespace parser {


TaskGetDataTypeAssocData::TaskGetDataTypeAssocData(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("zsp::fe::parser::TaskGetDataTypeAssocData", m_ctxt->getDebugMgr());

}

TaskGetDataTypeAssocData::~TaskGetDataTypeAssocData() {

}

ast::IAssocData *TaskGetDataTypeAssocData::get(ast::IDataType *dt) {
    DEBUG_ENTER("get");
    m_ret = 0;
    dt->accept(m_this);

    DEBUG_LEAVE("get %p", m_ret);
    return m_ret;
}

void TaskGetDataTypeAssocData::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope %s", i->getName().c_str());
    if (i->getAssocData()) {
        m_ret = i->getAssocData();
    } else if (i->getTarget()) {
        ast::ITypeScope *ts_target = 
            dynamic_cast<ast::ITypeScope *>(i->getTarget());
        if (ts_target->getSuper_t()) {
            DEBUG("TODO: Keep looking through Super");
            ast::ISymbolScope *scope = m_ctxt->symScopes().at(0);

            zsp::parser::TaskResolveSymbolPathRefResult result = 
                zsp::parser::TaskResolveSymbolPathRef(
                    m_ctxt->getDebugMgr(),
                    scope).resolveFull(ts_target->getSuper_t()->getTarget());
            result.val.ts->getTarget()->accept(m_this);
        } else {
            DEBUG("End of the line. Nothing else to search for");
        }
    }
    DEBUG_LEAVE("visitSymbolTypeScope %s", i->getName().c_str());
}

void TaskGetDataTypeAssocData::visitDataTypeUserDefined(ast::IDataTypeUserDefined *i) { 
    DEBUG_ENTER("visitDataTypeUserDefined");
    i->getType_id()->getElems().size();
    for (std::vector<ast::ITypeIdentifierElemUP>::const_iterator
        it=i->getType_id()->getElems().begin();
        it!=i->getType_id()->getElems().end(); it++) {
        DEBUG("  TypeId Elem: %s", (*it)->getId()->getId().c_str());
    }

    for (std::vector<ast::SymbolRefPathElem>::const_iterator
        it=i->getType_id()->getTarget()->getPath().begin();
        it!=i->getType_id()->getTarget()->getPath().end(); it++) {
        DEBUG("  Elem: kind=%d idx=%d", it->kind, it->idx);
    }

    ast::ISymbolScope *scope = m_ctxt->symScopes().at(0);

    zsp::parser::TaskResolveSymbolPathRefResult result = 
        zsp::parser::TaskResolveSymbolPathRef(
            m_ctxt->getDebugMgr(),
            scope).resolveFull(i->getType_id()->getTarget());

    DEBUG("kind=%d", result.kind);

    switch (result.kind) {
        case zsp::parser::TaskResolveSymbolPathRefResult::SymbolTypeScope:
            DEBUG("SymbolTypeScope");
            if (result.val.ts->getAssocData()) {
                DEBUG("Found associated data");
                m_ret = result.val.ts->getAssocData();
            } else {
                ast::ITypeScope *ts_target = 
                    dynamic_cast<ast::ITypeScope *>(result.val.ts->getTarget());
                if (ts_target->getSuper_t()) {
                    if (ts_target->getSuper_t()->getTarget()) {
                        DEBUG("TODO: Keep looking through Super");
                        zsp::parser::TaskResolveSymbolPathRefResult result = 
                            zsp::parser::TaskResolveSymbolPathRef(
                                m_ctxt->getDebugMgr(),
                                scope).resolveFull(ts_target->getSuper_t()->getTarget());
                        result.val.ts->accept(m_this);
                    } else {
                        ERROR("Failed to resolve super type");
                    }
                } else {
                    DEBUG("End of the line. Nothing else to search for");
                }
            }
//            result.val.ts->accept(m_this);
            break;
        case zsp::parser::TaskResolveSymbolPathRefResult::DataType:
            DEBUG("DataType");
            result.val.dt->accept(m_this);
            break;
        default:
            DEBUG("Unhandled case");
            break;
    }

    DEBUG_LEAVE("visitDataTypeUserDefined (%p)", m_ret);
}

dmgr::IDebug *TaskGetDataTypeAssocData::m_dbg = 0;

}
}
}
