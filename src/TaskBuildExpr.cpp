/*
 * TaskBuildExpr.cpp
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
#include "zsp/parser/impl/TaskResolveSymbolPathRef.h"
#include "zsp/parser/impl/TaskGetName.h"
#include "TaskBuildExpr.h"
#include "TaskResolveGlobalRef.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildExpr::TaskBuildExpr(IAst2ArlContext *ctxt) : m_ctxt(ctxt),
    m_val(ctxt->ctxt()->mkModelValU(0, 32)) {
    DEBUG_INIT("TaskBuildExpr", ctxt->getDebugMgr());
}

TaskBuildExpr::~TaskBuildExpr() {

}

vsc::dm::ITypeExpr *TaskBuildExpr::build(ast::IExpr *e) {
    DEBUG_ENTER("build");
    vsc::dm::ITypeExpr *e_dm = expr(e);
    DEBUG_LEAVE("build");
    return e_dm;
}

void TaskBuildExpr::visitExprStaticRefPath(ast::IExprStaticRefPath *i) { 
    DEBUG_ENTER("visitExprStaticRefPath");

    DEBUG_LEAVE("visitExprStaticRefPath");
}

void TaskBuildExpr::visitExprString(ast::IExprString *i) { 
    DEBUG_ENTER("visitExprString");

    DEBUG_LEAVE("visitExprString");
}

void TaskBuildExpr::visitExprSubscript(ast::IExprSubscript *i) { 
    DEBUG_ENTER("visitExprSubscript");

    DEBUG_LEAVE("visitExprSubscript");
}

void TaskBuildExpr::visitExprUnary(ast::IExprUnary *i) { 
    DEBUG_ENTER("visitExprUnary");

    DEBUG_LEAVE("visitExprUnary");
}

void TaskBuildExpr::visitExprBin(ast::IExprBin *i) { 
    DEBUG_ENTER("visitExprBin");

    DEBUG_LEAVE("visitExprBin");
}

void TaskBuildExpr::visitExprBitSlice(ast::IExprBitSlice *i) { 

}
    
void TaskBuildExpr::visitExprBool(ast::IExprBool *i) { 

}
    
void TaskBuildExpr::visitExprCast(ast::IExprCast *i) { 

}
    
void TaskBuildExpr::visitExprCond(ast::IExprCond *i) { 

}

void TaskBuildExpr::visitExprDomainOpenRangeList(ast::IExprDomainOpenRangeList *i) { 

}
    
void TaskBuildExpr::visitExprDomainOpenRangeValue(ast::IExprDomainOpenRangeValue *i) { 

}
    
void TaskBuildExpr::visitExprHierarchicalId(ast::IExprHierarchicalId *i) { 

}
    
void TaskBuildExpr::visitExprId(ast::IExprId *i) { 

}
    
void TaskBuildExpr::visitExprIn(ast::IExprIn *i) { 

}
    
void TaskBuildExpr::visitExprMemberPathElem(ast::IExprMemberPathElem *i) { 

}
    
void TaskBuildExpr::visitExprNull(ast::IExprNull *i) { 
    DEBUG_ENTER("visitExprNull");

    DEBUG_LEAVE("visitExprNull");
}
    
void TaskBuildExpr::visitExprNumber(ast::IExprNumber *i) { 
    DEBUG_ENTER("visitExprNumber");

    DEBUG_LEAVE("visitExprNumber");
}
    
void TaskBuildExpr::visitExprAggregateLiteral(ast::IExprAggregateLiteral *i) { 

}
    
void TaskBuildExpr::visitExprOpenRangeList(ast::IExprOpenRangeList *i) { 

}
    
void TaskBuildExpr::visitExprOpenRangeValue(ast::IExprOpenRangeValue *i) { 

}
    
void TaskBuildExpr::visitExprRefPath(ast::IExprRefPath *i) { 
    DEBUG_ENTER("visitExprRefPath");

    DEBUG_LEAVE("visitExprRefPath");
}

void TaskBuildExpr::visitExprRefPathId(ast::IExprRefPathId *i) {
    DEBUG_ENTER("visitExprRefPathId %s", i->getId()->getId().c_str());

    // First, is to determine whether we have a:
    // - Context path (relative to type context)
    // - Bottom-up path (relative to the activity/exec context)
    // - Type path (pointer to a constant in a type)
    DEBUG("Path size=%d ; scope depth=%d", 
        i->getTarget()->getPath().size(),
        m_ctxt->symScopes().size());
    ast::ISymbolScope *scope = m_ctxt->symScopes().at(0);
    int32_t type_scope_idx = -1;
    for (uint32_t ii=0; ii<i->getTarget()->getPath().size(); ii++) {
        DEBUG("Scope: %s ;   ii=%d", scope->getName().c_str(), i->getTarget()->getPath().at(ii));
        ast::IScopeChild *c = scope->getChildren().at(
            i->getTarget()->getPath().at(ii).idx);

        if (c == m_ctxt->typeScope()) {
            type_scope_idx = ii;
        }

        if (ii+1 < i->getTarget()->getPath().size()) {
            scope = dynamic_cast<ast::ISymbolScope *>(c);
        }
    }

    DEBUG("type_scope_idx=%d", type_scope_idx);

    if (type_scope_idx != -1) {
        if (type_scope_idx+1 == (i->getTarget()->getPath().size()-1)) {
            DEBUG("Type-context reference");
            vsc::dm::ITypeExprFieldRef *ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                vsc::dm::ITypeExprFieldRef::RootRefKind::TopDownScope,
                0);
            for (uint32_t ii=type_scope_idx+1; ii<i->getTarget()->getPath().size(); ii++) {
                ref->addPathElem(i->getTarget()->getPath().at(ii).idx);
            }
            m_expr = ref;

            // TODO: determine if this is actually a static reference
        } else {
            DEBUG("Bottom-up scope reference");
            vsc::dm::ITypeExprFieldRef *ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                vsc::dm::ITypeExprFieldRef::RootRefKind::BottomUpScope,
                (m_ctxt->symScopes().size()-i->getTarget()->getPath().size()));
            ref->addPathElem(i->getTarget()->getPath().back().idx);
            m_expr = ref;
        }
    } else {
        DEBUG("Static (type) reference, since we didn't encounter the type context");
    }

    DEBUG_LEAVE("visitExprRefPathId");
}
    
void TaskBuildExpr::visitExprRefPathContext(ast::IExprRefPathContext *i) { 
    DEBUG_ENTER("visitExprRefPathContext");

    for (std::vector<ast::SymbolRefPathElem>::const_iterator
        it=i->getTarget()->getPath().begin();
        it!=i->getTarget()->getPath().end(); it++) {
        DEBUG("it: kind=%d idx=%d", it->kind, it->idx);
    }

        // First, is to determine whether we have a:
    // - Context path (relative to type context)
    // - Bottom-up path (relative to the activity/exec context)
    // - Type path (pointer to a constant in a type)
    DEBUG("Path size=%d ; scope depth=%d", 
        i->getTarget()->getPath().size(),
        m_ctxt->symScopes().size());
    ast::ISymbolScope *scope = m_ctxt->symScopes().at(0);
    int32_t type_scope_idx = -1;
    for (uint32_t ii=0; ii<i->getTarget()->getPath().size(); ii++) {
        DEBUG("Scope: %s ;   ii=%d", scope->getName().c_str(), i->getTarget()->getPath().at(ii));
        ast::IScopeChild *c = scope->getChildren().at(
            i->getTarget()->getPath().at(ii).idx);

        if (c == m_ctxt->typeScope()) {
            type_scope_idx = ii;
        }

        if (ii+1 < i->getTarget()->getPath().size()) {
            scope = dynamic_cast<ast::ISymbolScope *>(c);
        }
    }

    DEBUG("type_scope_idx=%d", type_scope_idx);

    if (type_scope_idx != -1) {
        if (type_scope_idx+1 == (i->getTarget()->getPath().size()-1)) {
            DEBUG("Type-context reference");
            vsc::dm::ITypeExprFieldRef *ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                vsc::dm::ITypeExprFieldRef::RootRefKind::TopDownScope,
                0);
            for (uint32_t ii=type_scope_idx+1; ii<i->getTarget()->getPath().size(); ii++) {
                ref->addPathElem(i->getTarget()->getPath().at(ii).idx);
            }
            m_expr = ref;

            // TODO: determine if this is actually a static reference
        } else {
            DEBUG("Bottom-up scope reference");
            vsc::dm::ITypeExprFieldRef *ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                vsc::dm::ITypeExprFieldRef::RootRefKind::BottomUpScope,
                (m_ctxt->symScopes().size()-i->getTarget()->getPath().size()));
            ref->addPathElem(i->getTarget()->getPath().back().idx);
            m_expr = ref;
        }
    } else if (i->getHier_id()->getElems().at(0)->getParams()) {
        DEBUG("Function call %p", i->getTarget());
        DEBUG("Function call %p", i->getHier_id()->getElems().at(0)->getTarget());

        zsp::parser::TaskResolveSymbolPathRef resolver(
            m_ctxt->getDebugMgr(),
            m_ctxt->getRoot());
        
        zsp::ast::IScopeChild *t = resolver.resolve(i->getTarget());
        std::string fname = zsp::parser::TaskGetName().get(t, true);
        DEBUG("Function Name: %s", fname.c_str());
        arl::dm::IDataTypeFunction *func = m_ctxt->ctxt()->findDataTypeFunction(fname);
        arl::dm::ITypeExprMethodCallStaticUP call_e(
            m_ctxt->ctxt()->mkTypeExprMethodCallStatic(
                func,
                {}
            ));
        m_expr = call_e.release();
    } else {
        DEBUG("Static (type) reference, since we didn't encounter the type context");
        // Could be
        // Package
        // TypeScope
        // Function
        // 
//        TaskResolveGlobalRef(m_ctxt).resolve(i->getTarget());
//        vsc::dm::IDataType *type = 
    }


    DEBUG_LEAVE("visitExprRefPathContext");
}
    
void TaskBuildExpr::visitExprRefPathElem(ast::IExprRefPathElem *i) { 
    DEBUG_ENTER("visitExprRefPathElem");

    DEBUG_LEAVE("visitExprRefPathElem");
}
    
void TaskBuildExpr::visitExprRefPathStaticRooted(ast::IExprRefPathStaticRooted *i) { 
    DEBUG_ENTER("visitExprRefPathStaticRooted");
    DEBUG("root=%p leaf=%p", i->getRoot(), i->getLeaf());

    DEBUG_LEAVE("visitExprRefPathStaticRooted");
}
    
void TaskBuildExpr::visitExprRefPathSuper(ast::IExprRefPathSuper *i) { 

}
    
void TaskBuildExpr::visitConstraintScope(ast::IConstraintScope *i) { 

}
    
void TaskBuildExpr::visitExprSignedNumber(ast::IExprSignedNumber *i) { 
    DEBUG_ENTER("visitExprSignedNumber");
    m_val->setBits(i->getWidth());
    m_val->set_val_u(i->getValue());
    m_expr = m_ctxt->ctxt()->mkTypeExprVal(m_val.get());

    DEBUG_LEAVE("visitExprSignedNumber");
}

void TaskBuildExpr::visitExprUnsignedNumber(ast::IExprUnsignedNumber *i) { 
    DEBUG_ENTER("visitExprUnsignedNumber");
    m_val->setBits(i->getWidth());
    m_val->set_val_i(i->getValue());
    m_expr = m_ctxt->ctxt()->mkTypeExprVal(m_val.get());

    DEBUG_LEAVE("visitExprUnsignedNumber");
}

void TaskBuildExpr::visitExprRefPathStatic(ast::IExprRefPathStatic *i) { 
    DEBUG_ENTER("visitExprRefPathStatic");

    DEBUG_LEAVE("visitExprRefPathStatic");
}

void TaskBuildExpr::visitExprRefPathStaticFunc(ast::IExprRefPathStaticFunc *i) { 

}


vsc::dm::ITypeExpr *TaskBuildExpr::expr(ast::IExpr *e) {
    DEBUG_ENTER("expr");
    m_expr = 0;
    e->accept(m_this);
    DEBUG_LEAVE("expr");
    return m_expr;
}

dmgr::IDebug *TaskBuildExpr::m_dbg = 0;

}
}
}
