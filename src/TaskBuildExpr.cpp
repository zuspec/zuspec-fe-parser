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
#include "vsc/dm/impl/ValRefBool.h"
#include "zsp/parser/impl/TaskResolveSymbolPathRef.h"
#include "zsp/parser/impl/TaskGetName.h"
#include "zsp/parser/impl/TaskIndexField.h"
#include "TaskBuildDataTypeFunction.h"
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
    DEBUG_LEAVE("build %p", e_dm);
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

static std::map<ast::ExprBinOp, vsc::dm::BinOp> binop_m = {
    { ast::ExprBinOp::BinOp_Eq, vsc::dm::BinOp::Eq },
    { ast::ExprBinOp::BinOp_Ne, vsc::dm::BinOp::Ne },
    { ast::ExprBinOp::BinOp_Gt, vsc::dm::BinOp::Gt },
    { ast::ExprBinOp::BinOp_Ge, vsc::dm::BinOp::Ge },
    { ast::ExprBinOp::BinOp_Lt, vsc::dm::BinOp::Lt },
    { ast::ExprBinOp::BinOp_Le, vsc::dm::BinOp::Le },
    { ast::ExprBinOp::BinOp_Add, vsc::dm::BinOp::Add },
    { ast::ExprBinOp::BinOp_Sub, vsc::dm::BinOp::Sub },
    { ast::ExprBinOp::BinOp_Div, vsc::dm::BinOp::Div },
    { ast::ExprBinOp::BinOp_Mul, vsc::dm::BinOp::Mul },
    { ast::ExprBinOp::BinOp_Mod, vsc::dm::BinOp::Mod },
    { ast::ExprBinOp::BinOp_BitAnd, vsc::dm::BinOp::BinAnd },
    { ast::ExprBinOp::BinOp_BitOr, vsc::dm::BinOp::BinOr },
    { ast::ExprBinOp::BinOp_BitXor, vsc::dm::BinOp::BinXor },
    { ast::ExprBinOp::BinOp_LogAnd, vsc::dm::BinOp::LogAnd },
    { ast::ExprBinOp::BinOp_LogOr, vsc::dm::BinOp::LogOr },
    { ast::ExprBinOp::BinOp_Shl, vsc::dm::BinOp::Sll },
    { ast::ExprBinOp::BinOp_Shr, vsc::dm::BinOp::Srl } /*,
    { ast::ExprBinOp::BinOp_Xor, vsc::dm::BinOp::Xor },
    { ast::ExprBinOp::BinOp_Not, vsc::dm::BinOp::Not },
     */
};

void TaskBuildExpr::visitExprBin(ast::IExprBin *i) { 
    DEBUG_ENTER("visitExprBin");
    vsc::dm::ITypeExpr *lhs = expr(i->getLhs());
    vsc::dm::ITypeExpr *rhs = expr(i->getRhs());

    m_expr = m_ctxt->ctxt()->mkTypeExprBin(
        lhs,
        binop_m.find(i->getOp())->second,
        rhs
    );

    DEBUG_LEAVE("visitExprBin");
}

void TaskBuildExpr::visitExprBitSlice(ast::IExprBitSlice *i) { 
    DEBUG_ENTER("visitExprBitSlice");
    DEBUG("TODO: visitExprBitSlice");
    DEBUG_ENTER("visitExprBitSlice");
}
    
void TaskBuildExpr::visitExprBool(ast::IExprBool *i) {
    DEBUG_ENTER("visitExprBool");
    m_expr = m_ctxt->ctxt()->mkTypeExprVal(
        m_ctxt->ctxt()->mkValRefInt(0, false, 1));
    DEBUG_LEAVE("visitExprBool");
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
    DEBUG_ENTER("visitExprAggregateLiteral");
    DEBUG("TODO: visitExprAggregateLiteral");
    DEBUG_LEAVE("visitExprAggregateLiteral");
}
    
void TaskBuildExpr::visitExprOpenRangeList(ast::IExprOpenRangeList *i) { 
    DEBUG_ENTER("visitExprOpenRangeList");
    DEBUG("TODO: visitExprOpenRangeList");
    DEBUG_LEAVE("visitExprOpenRangeList");
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
    DEBUG("HierId (%p) .size=%d", 
        i->getHier_id(),
        i->getHier_id()->getElems().size());
    DEBUG("TypeScope: %s", m_ctxt->typeScope()->getName().c_str());

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
        DEBUG("Scope: %s ; ii=%d idx=%d", 
            scope->getName().c_str(), 
            ii,
            i->getTarget()->getPath().at(ii).idx);
        ast::IScopeChild *c = scope->getChildren().at(
            i->getTarget()->getPath().at(ii).idx);

        if (c == m_ctxt->typeScope()) {
            DEBUG("Found type scope @ %d (%s)", ii,
                m_ctxt->typeScope()->getName().c_str());
            // we're looking at the *next* entry, so adjust 
            // the index accordingly
            type_scope_idx = ii+1;
        }

        if (ii+1 < i->getTarget()->getPath().size()) {
            scope = dynamic_cast<ast::ISymbolScope *>(c);
        }
    }

    DEBUG("type_scope_idx=%d", type_scope_idx);

    // Determine how to get to the root identifier
    // - It's a field within the current action
    // - It's a field relative to where we are
    //
    // Then, determine how to proceed
    // - 
    vsc::dm::ITypeExpr *expr = 0;
    if (type_scope_idx != -1) {
        DEBUG("type_scope_idx=%d (PathSize-1)=%d",
            type_scope_idx,
            (i->getTarget()->getPath().size()-1));
        if (type_scope_idx == (i->getTarget()->getPath().size()-1)) {
            // Reference is to a field within the active type
            DEBUG("Type-context reference");
            vsc::dm::ITypeExprFieldRef *ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                vsc::dm::ITypeExprFieldRef::RootRefKind::TopDownScope,
                -1);

            // This gets us 
            for (uint32_t ii=type_scope_idx; ii<i->getTarget()->getPath().size(); ii++) {
                DEBUG("Add path elem %d", i->getTarget()->getPath().at(ii).idx);
                ref->addPathElem(i->getTarget()->getPath().at(ii).idx);
            }
            expr = ref;

            // TODO: determine if this is actually a static reference
        } else {
            DEBUG("Bottom-up scope reference");
            vsc::dm::ITypeExprFieldRef *ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                vsc::dm::ITypeExprFieldRef::RootRefKind::BottomUpScope,
                (m_ctxt->symScopes().size()-i->getTarget()->getPath().size()));
            ref->addPathElem(i->getTarget()->getPath().back().idx);
            expr = ref;
        }
    }

    zsp::parser::TaskResolveSymbolPathRef resolver(
            m_ctxt->getDebugMgr(),
            m_ctxt->getRoot());
    ast::IScopeChild *ast_scope = resolver.resolve(i->getTarget());
    for (uint32_t ii=0; ii<i->getHier_id()->getElems().size(); ii++) {
        DEBUG("Path[%d]: %d", ii, i->getHier_id()->getElems().at(ii)->getTarget());
    }

    vsc::dm::ITypeExprFieldRef *field_ref = 0;
    for (uint32_t ii=1; ii<i->getHier_id()->getElems().size(); ii++) {
        ast::IExprMemberPathElem *elem = i->getHier_id()->getElems().at(ii).get();

/*
        int32_t target = i->getHier_id()->getElems().at(ii)->getTarget();
        DEBUG("target=%d", target);
        ast_scope = dynamic_cast<ast::IScope *>(ast_scope)->getChildren().at(target).get();
        DEBUG("ast_scope=%p", ast_scope);
 */

        if (elem->getParams()) {
            // Func
            DEBUG("Elem[%d] has parameters (%s)", ii, elem->getId()->getId().c_str());

            ast_scope = zsp::parser::TaskIndexField(
                m_ctxt->getDebugMgr(),
                m_ctxt->getRoot()).index(
                    ast_scope,
                    elem->getTarget());

            zsp::ast::IScopeChild *func_t = ast_scope;
            std::string fname = zsp::parser::TaskGetName().get(func_t, true);
            DEBUG("FunctionCallStatic: name=%s", fname.c_str());
            arl::dm::IDataTypeFunction *func = m_ctxt->ctxt()->findDataTypeFunction(fname);

            if (!func) {
                DEBUG("Error: failed to find function %s", fname.c_str());
            }

            std::vector<vsc::dm::ITypeExpr *> params;

            for (std::vector<ast::IExprUP>::const_iterator
                it=elem->getParams()->getParameters().begin();
                it!=elem->getParams()->getParameters().end(); it++) {
                params.push_back(TaskBuildExpr(m_ctxt).build(it->get()));
            }

            if (!expr) {
                // Static function call
                DEBUG("Elem %d: Static function call", ii);

                DEBUG("Function Name: %s", fname.c_str());
                arl::dm::ITypeExprMethodCallStaticUP call_e(
                    m_ctxt->ctxt()->mkTypeExprMethodCallStatic(
                    func,
                    params));
                expr = call_e.release();
            } else {
                DEBUG("Elem %d: Context function call", ii);

                // Context method call
                arl::dm::ITypeExprMethodCallContextUP call_e(
                    m_ctxt->ctxt()->mkTypeExprMethodCallContext(
                        func,
                        expr,
                        params
                    )
                );
                expr = call_e.release();
            }
        } else if (elem->getSubscript()) {
            DEBUG("TODO: array subscript reference");
            if (!field_ref) {
                
            }
        } else {
            DEBUG("TODO: field subref expr=%p idx=%d", expr, elem->getTarget());
            if (!field_ref) {
                field_ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                    expr, 
                    elem->getTarget());
                expr = field_ref;
            } else {
                field_ref->addPathElem(elem->getTarget());
            }
            ast_scope = zsp::parser::TaskIndexField(
                m_ctxt->getDebugMgr(),
                m_ctxt->getRoot()).index(
                    ast_scope,
                    elem->getTarget());
        }
    }

    m_expr = expr;

    DEBUG_LEAVE("visitExprRefPathContext %p", m_expr);
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
    DEBUG_ENTER("visitExprRefPathSuper");
    DEBUG_LEAVE("visitExprRefPathSuper");
}
    
void TaskBuildExpr::visitConstraintScope(ast::IConstraintScope *i) { 
    // NOP
}
    
void TaskBuildExpr::visitExprSignedNumber(ast::IExprSignedNumber *i) { 
    DEBUG_ENTER("visitExprSignedNumber");
    m_expr = m_ctxt->ctxt()->mkTypeExprVal(
        m_ctxt->ctxt()->mkValRefInt(i->getValue(), true, i->getWidth()));
    DEBUG_LEAVE("visitExprSignedNumber");
}

void TaskBuildExpr::visitExprUnsignedNumber(ast::IExprUnsignedNumber *i) { 
    DEBUG_ENTER("visitExprUnsignedNumber");
    m_expr = m_ctxt->ctxt()->mkTypeExprVal(
        m_ctxt->ctxt()->mkValRefInt(i->getValue(), false, i->getWidth()));
    DEBUG_LEAVE("visitExprUnsignedNumber");
}

void TaskBuildExpr::visitExprRefPathStatic(ast::IExprRefPathStatic *i) { 
    DEBUG_ENTER("visitExprRefPathStatic");

    DEBUG_LEAVE("visitExprRefPathStatic");
}

void TaskBuildExpr::visitExprRefPathStaticFunc(ast::IExprRefPathStaticFunc *i) { 
    DEBUG_ENTER("visitExprRefPathStaticFunc");
    DEBUG("TODO: visitExprRefPathStaticFunc");
    DEBUG_LEAVE("visitExprRefPathStaticFunc");
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
