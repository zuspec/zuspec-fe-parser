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
    if (!e_dm) {
        ERROR("Failed to build expression");
    }
    DEBUG_LEAVE("build %p", e_dm);
    return e_dm;
}

void TaskBuildExpr::visitExprStaticRefPath(ast::IExprStaticRefPath *i) { 
    DEBUG_ENTER("visitExprStaticRefPath");

    DEBUG_LEAVE("visitExprStaticRefPath");
}

void TaskBuildExpr::visitExprString(ast::IExprString *i) { 
    DEBUG_ENTER("visitExprString \"%s\"", i->getValue().c_str());
    m_expr = m_ctxt->ctxt()->mkTypeExprVal(
        m_ctxt->ctxt()->mkValRefStr(i->getValue()));
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
    DEBUG_ENTER("visitExprMemberPathElem");
    if (i->getParams()) {
        DEBUG("Method call");
        if (m_ctxt->isPyRef()) {
            DEBUG("Create Python method call");
            std::vector<vsc::dm::ITypeExpr *> params;
            m_ctxt->pushBaseExpr(0); // No parent for these elements
            for (std::vector<ast::IExprUP>::const_iterator
                it=i->getParams()->getParameters().begin();
                it!=i->getParams()->getParameters().end(); it++) {
                m_expr = 0;
                (*it)->accept(m_this);
                params.push_back(m_expr);
            }
            m_ctxt->popBaseExpr();

            arl::dm::ITypeExprPythonMethodCall *call = m_ctxt->ctxt()->mkTypeExprPythonMethodCall(
                m_ctxt->baseExpr(),
                true,
                i->getId()->getId(),
                params);
            m_expr = call;
        } else {
            // TODO: base tells us how to get here. 
            // How do we know if it's a context or static call?
            // Use baseExpr() to determine
        }
    } else {
        DEBUG("Sub-field reference");
    }

    DEBUG_LEAVE("visitExprMemberPathElem");
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
        
        DEBUG("Scope=%p typeScope=%p symScope=%p",
            c, m_ctxt->typeScope(), m_ctxt->symScope());

        if (c == m_ctxt->typeScope()) {
            type_scope_idx = ii;
        } else if (c == m_ctxt->symScope()) {
            DEBUG("TODO: bottom-up scope");
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
    DEBUG_ENTER("visitExprRefPathContext elem[0]=%s", i->getHier_id()->getElems().at(0)->getId()->getId().c_str());

    if (!m_ctxt->baseExpr()) {
        if (!i->getTarget()) {
            ERROR("expression target is null");
            DEBUG_LEAVE("visitExprRefPathContext");
            return;
        }

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
        int32_t type_scope_idx=-1, bup_scope_idx=-1;
        uint32_t ii;
        for (ii=0; ii<i->getTarget()->getPath().size(); ii++) {
            DEBUG("Scope: %s ; ii=%d idx=%d", 
                scope->getName().c_str(), 
                ii,
                i->getTarget()->getPath().at(ii).idx);
            ast::IScopeChild *c = scope->getChildren().at(
                i->getTarget()->getPath().at(ii).idx);

            DEBUG("Scope=%s typeScope=%s symScope=%s",
                scope->getName().c_str(), 
                m_ctxt->typeScope()->getName().c_str(), 
                m_ctxt->symScope()->getName().c_str());
            if (c == m_ctxt->typeScope()) {
                DEBUG("Found type scope @ %d (%s)", ii,
                    m_ctxt->typeScope()->getName().c_str());
                // we're looking at the *next* entry, so adjust 
                // the index accordingly
                type_scope_idx = ii+1;
                break;
            } else if ((bup_scope_idx=m_ctxt->findBottomUpScope(scope)) != -1) {
                // Find the first scope on the top-down path
                // that matches a bottom-up scope.
                DEBUG("bottom-up scope %d", bup_scope_idx);
                break;
            }

            if (ii+1 < i->getTarget()->getPath().size()) {
                scope = dynamic_cast<ast::ISymbolScope *>(c);
            }
        }

        DEBUG("type_scope_idx=%d bup_scope_idx=%d", 
            type_scope_idx, bup_scope_idx);

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
                if (ref->getPath().size() == 0) {
                    ERROR("type-context reference path is empty");
                }

                expr = ref;

                // TODO: determine if this is actually a static reference
            } else {
                ERROR("Failed consistency check");
            }
        } else if (bup_scope_idx != -1) {
            // First, keep going along the path to find the last
            // bottom-up scope on the path
            for (; ii<i->getTarget()->getPath().size()-1; ii++) {
                int32_t t_bup_scope_idx;
                ast::IScopeChild *c = scope->getChildren().at(
                    i->getTarget()->getPath().at(ii).idx);
                scope = dynamic_cast<ast::ISymbolScope *>(c);

                if ((t_bup_scope_idx=m_ctxt->findBottomUpScope(scope)) != -1) {
                    DEBUG("Found bottom-up scope %d @ path index %d", t_bup_scope_idx, ii);
                    bup_scope_idx = t_bup_scope_idx;
                } else {
                    DEBUG("Reached the end of bottom-up scopes @ %d", ii);
                    break;
                }
            }
            fflush(stdout);

            DEBUG("Processing bottom-up reference");
            ast::ISymbolExecScope *lscope = dynamic_cast<ast::ISymbolExecScope *>(
                    m_ctxt->symScopes().at(m_ctxt->symScopes().size()-bup_scope_idx-1));
            
            vsc::dm::ITypeExprFieldRef *ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                vsc::dm::ITypeExprFieldRef::RootRefKind::BottomUpScope,
                bup_scope_idx);

            if (lscope) {
                // If the lowest local scope is an Exec scope, then 
                // we need to remap variable indices
                DEBUG("Must transform first element ii=%d idx=%d", ii, i->getTarget()->getPath().at(ii).idx);
                DEBUG("Children: %d locals: %d", lscope->getChildren().size(), lscope->getLocals().size());
                int32_t target_idx = -1;
                int32_t var_child_idx = i->getTarget()->getPath().at(ii).idx;
                for (uint32_t vi=0; vi<lscope->getLocals().size(); vi++) {
                    if (lscope->getLocals().at(vi) == lscope->getChildren().at(var_child_idx)) {
                        DEBUG("Found local @ %d", vi);
                        target_idx = vi;
                        break;
                    }
                }
                fflush(stdout);
                if (target_idx != -1) {
                    DEBUG("Remapped reference to local-var @ %d", target_idx);
                    ref->addPathElem(target_idx);
                } else {
                    FATAL("Failed to remap locals index");
                }
                ii++;
            } else {
                DEBUG("Target scope is not an Exec scope. No need to remap");
            }

            for (uint32_t li=ii; li<i->getTarget()->getPath().size(); li++) {
                ref->addPathElem(i->getTarget()->getPath().at(li).idx);
            }
            DEBUG("Ref has %d elements", ref->getPath().size());
            expr = ref;
        } else { // Error -- needs to be one
            DEBUG("TODO: See if this is a global");
            // 
            ERROR("Neither bottom-up or top-down");
        }

    zsp::parser::TaskResolveSymbolPathRef resolver(
            m_ctxt->getDebugMgr(),
            m_ctxt->getRoot());
    ast::IScopeChild *ast_scope = resolver.resolve(i->getTarget());
    for (uint32_t ii=0; ii<i->getHier_id()->getElems().size(); ii++) {
        DEBUG("Path[%d]: %d", ii, i->getHier_id()->getElems().at(ii)->getTarget());
    }

    vsc::dm::ITypeExprFieldRef *field_ref = 0;
    for (uint32_t ii=0; ii<i->getHier_id()->getElems().size(); ii++) {
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

            if (ii) {
                ast_scope = zsp::parser::TaskIndexField(
                    m_ctxt->getDebugMgr(),
                    m_ctxt->getRoot()).index(
                        ast_scope,
                        elem->getTarget());
            }

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
            if (ii) {
                DEBUG("TODO: field subref expr=%p idx=%d", expr, elem->getTarget());
                if (!field_ref) {
                    field_ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                        expr, 
                        elem->getTarget());

                    if (field_ref->getPath().size() == 0) {
                        ERROR("bottom-up reference path is empty");
                    }
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
    }
    m_expr = expr;
    } else {
        // Root is an expression
        DEBUG("Note: root is an expression");
    }


    DEBUG_LEAVE("visitExprRefPathContext %p", m_expr);
}
    
void TaskBuildExpr::visitExprRefPathElem(ast::IExprRefPathElem *i) { 
    DEBUG_ENTER("visitExprRefPathElem");

    DEBUG_LEAVE("visitExprRefPathElem");
}
    
void TaskBuildExpr::visitExprRefPathStaticRooted(ast::IExprRefPathStaticRooted *i) { 
    DEBUG_ENTER("visitExprRefPathStaticRooted");
    
    // First, build the root-reference expression
    m_expr = 0;
    m_ctxt->pushBaseExpr(m_expr);
    i->getRoot()->accept(m_this);
    m_ctxt->popBaseExpr();

    if (!m_expr) {
        ERROR("Building root expression failed");
        DEBUG_LEAVE("visitExprRefPathStaticRooted -- failed to build root expr");
        return;
    }

    m_ctxt->pushIsPyRef(i->getRoot()->getTarget()->getPyref_idx() != -1);
    for (std::vector<ast::IExprMemberPathElemUP>::const_iterator
        it=i->getLeaf()->getElems().begin();
        it!=i->getLeaf()->getElems().end(); it++) {
        DEBUG("Push BaseExpr %p", m_expr);
        m_ctxt->pushBaseExpr(m_expr);
        DEBUG("BaseExpr %p", m_ctxt->baseExpr());
        (*it)->accept(m_this);
        m_ctxt->popBaseExpr();
        DEBUG("Post Pop BaseExpr expr=%p", m_expr);
    }
    m_ctxt->popIsPyRef();

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
    if (i->getTarget()->getPyref_idx() != -1) {
        DEBUG("Path involves a Python-type reference");
        ast::IScopeChild *target_c = zsp::parser::TaskResolveSymbolPathRef(
            m_ctxt->getDebugMgr(),
            m_ctxt->getRoot()).resolve(i->getTarget());
        
        // Build an expression to represent the PSS portion of the path
        target_c->accept(m_this);

        // Now, build references for the remainder
        for (std::vector<ast::ITypeIdentifierElemUP>::const_iterator
            it=(i->getBase().begin()+i->getTarget()->getPyref_idx()+1);
            it!=i->getBase().end(); it++) {
            if ((*it)->getParams()) {
                // Python method call
                std::vector<vsc::dm::ITypeExpr *> params;
                /*
                for (std::vector<ast::IExprUP>::const_iterator
                    it_p=(*it)->getParams()->getValues().back();
                    it_p!=(*it)->getParams()->getValues().end();)
                m_expr = m_ctxt->ctxt()->mkTypeExprPythonMethodCall(
                    m_expr,
                    true,
                    (*it)->getId()->getId(),
                    params);
                 */
            } else {
                // Python field reference
            }
        }

    } else {
        ERROR("non-python static ref path unimplemented");
    }
    DEBUG_LEAVE("visitExprRefPathStatic");
}

void TaskBuildExpr::visitExprRefPathStaticFunc(ast::IExprRefPathStaticFunc *i) { 
    DEBUG_ENTER("visitExprRefPathStaticFunc");
    DEBUG("TODO: visitExprRefPathStaticFunc");
    DEBUG_LEAVE("visitExprRefPathStaticFunc");
}

void TaskBuildExpr::visitPyImportStmt(ast::IPyImportStmt *i) {
    DEBUG_ENTER("visitPyImportStmt");
    // Lookup specified package
    std::string name;
    for (std::vector<ast::IExprIdUP>::const_iterator
        it=i->getPath().begin();
        it!=i->getPath().end(); it++) {
        if (name.size()) {
            name.append(".");
        }
        name.append((*it)->getId());
    }
    DEBUG("Searching for python import \"%s\"", name.c_str());
    arl::dm::IPyImport *imp = m_ctxt->ctxt()->findPyImport(name);
    m_expr = m_ctxt->ctxt()->mkTypeExprPyImportRef(imp);
    DEBUG_LEAVE("visitPyImportStmt");
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
