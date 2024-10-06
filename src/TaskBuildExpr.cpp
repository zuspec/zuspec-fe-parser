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
#include <map>
#include "dmgr/impl/DebugMacros.h"
#include "vsc/dm/impl/ValRefBool.h"
#include "zsp/parser/impl/ScopeUtil.h"
#include "zsp/parser/impl/TaskGetName.h"
#include "zsp/parser/impl/TaskGetSubscriptSymbolScope.h"
#include "zsp/parser/impl/TaskIndexField.h"
#include "zsp/parser/impl/TaskIsPyRef.h"
#include "zsp/parser/impl/TaskResolveSymbolPathRef.h"
#include "TaskBuildDataTypeFunction.h"
#include "TaskBuildExpr.h"
#include "TaskCalculateFieldOffset.h"
#include "TaskResolveGlobalRef.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildExpr::TaskBuildExpr(IAst2ArlContext *ctxt) : m_ctxt(ctxt),
    m_val(ctxt->ctxt()->mkModelValU(0, 32)) {
    DEBUG_INIT("zsp::fe::parser::TaskBuildExpr", ctxt->getDebugMgr());
}

TaskBuildExpr::~TaskBuildExpr() {

}

vsc::dm::ITypeExpr *TaskBuildExpr::build(ast::IExpr *e) {
    DEBUG_ENTER("build");
    vsc::dm::ITypeExpr *e_dm = expr(e);
    if (!e_dm) {
        DEBUG_ERROR("Failed to build expression");
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
        m_ctxt->ctxt()->mkValRefBool(i->getValue()));
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
                m_ctxt->ctxt()->mkTypeExprPythonFieldRef(
                    m_ctxt->baseExpr(),
                    true,
                    i->getId()->getId()),
                true,
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
    ast::ISymbolScope *scope = m_ctxt->rootSymScopeT<ast::ISymbolScope>();
    int32_t type_scope_idx = -1;
    for (uint32_t ii=0; ii<i->getTarget()->getPath().size(); ii++) {
        DEBUG("Scope: %s ;   ii=%d", scope->getName().c_str(), i->getTarget()->getPath().at(ii));
        ast::IScopeChild *c = scope->getChildren().at(
            i->getTarget()->getPath().at(ii).idx).get();
        
        DEBUG("Scope=%p typeScope=%p symScope=%p",
            c, m_ctxt->typeScope(), m_ctxt->symScope());

        if (c == m_ctxt->typeScope()) {
            type_scope_idx = ii+1;
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
                0,
                -1 /* TODO */);
                /*
            for (uint32_t ii=type_scope_idx+1; ii<i->getTarget()->getPath().size(); ii++) {
                ref->addPathElem(i->getTarget()->getPath().at(ii).idx);
            }
             */
            m_expr = ref;

            // TODO: determine if this is actually a static reference
        } else {
            DEBUG("Bottom-up scope reference");
            vsc::dm::ITypeExprFieldRef *ref = m_ctxt->ctxt()->mkTypeExprFieldRef(
                vsc::dm::ITypeExprFieldRef::RootRefKind::BottomUpScope,
                (m_ctxt->symScopes().size()-i->getTarget()->getPath().size()),
                -1 /* TODO */);
            /*
            ref->addPathElem(i->getTarget()->getPath().back().idx);
             */
            m_expr = ref;
        }
    } else {
        DEBUG("Static (type) reference, since we didn't encounter the type context");
    }

    DEBUG_LEAVE("visitExprRefPathId");
}
    
void TaskBuildExpr::visitExprRefPathContext(ast::IExprRefPathContext *i) { 
    DEBUG_ENTER("visitExprRefPathContext elem[0]=%s", i->getHier_id()->getElems().at(0)->getId()->getId().c_str());

    vsc::dm::ITypeExprFieldRef *field_ref = 0;

    if (!m_ctxt->baseExpr()) {
        if (!i->getTarget()) {
            DEBUG_ERROR("expression target is null");
            DEBUG_LEAVE("visitExprRefPathContext");
            return;
        }

        i->getHier_id()->getElems().front()->setTarget(
            i->getTarget()->getPath().back().idx
        );

        if (DEBUG_EN) {
            for (uint32_t ii=0; ii<i->getHier_id()->getElems().size(); ii++) {
                DEBUG("  Path[%d] %d", ii, i->getHier_id()->getElems().at(ii)->getTarget());
            }
        }

        // First, characterize the expression
        // - has function call as a non-terminal element
        // - has subscript as a non-terminal element
        bool has_fcall = false;
        bool has_index = false;
        bool has_pyref = false;
        bool has_nonterminal_fcall = false;
        bool has_nonterminal_index = false;
        bool has_nonterminal_pyref = false;

        zsp::parser::TaskIsPyRef is_pyref(m_ctxt->getDebugMgr(), m_ctxt->getRoot());
        for (std::vector<ast::IExprMemberPathElemUP>::const_iterator
            it=i->getHier_id()->getElems().begin();
            it!=i->getHier_id()->getElems().end(); it++) {
            if ((*it)->getParams()) {
                has_fcall = true;
                has_nonterminal_fcall = (it+1 != i->getHier_id()->getElems().end());
//                has_pyref |= is_pyref.check()
            } else if ((*it)->getSubscript().size()) {
                has_index = true;
                has_nonterminal_index = (it+1 != i->getHier_id()->getElems().end());
            }
        }

        DEBUG("has_fcall=%d has_nonterminal_fcall=%d has_index=%d has_nonterminal_index=%d",
            has_fcall, has_nonterminal_fcall,
            has_index, has_nonterminal_index);

        RootRefT root_ref = mkRootFieldRef(i);
        zsp::parser::TaskResolveSymbolPathRef resolver(
            m_ctxt->getDebugMgr(),
            m_ctxt->getRoot(),
            m_ctxt->inlineCtxt());
        ast::IScopeChild *ast_scope = resolver.resolve(i->getTarget());

        DEBUG("ast_scope: %s", zsp::parser::TaskGetName().get(ast_scope, true).c_str());

        if (!root_ref.first) {
            DEBUG_ERROR("mkRootFieldRef returns null root");
        }

        if (i->getHier_id()->getElems().size() > 1) {
            DEBUG("Building out result of mkRootFieldRef(idx=%d size=%d)");
            m_expr = buildRefExpr(
                root_ref.first,
                i,
                root_ref.second, // idx
                ast_scope);

        /*
        if (!has_nonterminal_fcall && !has_nonterminal_index) {
            // Just a plain hierarchical path
            DEBUG("Plain old hierarchical field path...");

        } else {
            // Something more complex
            DEBUG("Something more complex...");
        }
         */
        } else {
            DEBUG("Using return of mkRootFieldRef");
            m_expr = root_ref.first;

            if (i->getHier_id()->getElems().at(0)->getSubscript().size()) {
                ast::IExpr *idx = i->getHier_id()->getElems().at(0)->getSubscript().at(0).get();
                DEBUG("Adding a subscript");
                m_expr = m_ctxt->ctxt()->mkTypeExprArrIndex(
                    m_expr,
                    true,
                    TaskBuildExpr(m_ctxt).build(idx),
                    true);
            }
        }
    } else {
        // Root is an expression
        DEBUG_ERROR("Note: root is an expression");
    }

    if (!m_expr) {
        DEBUG_ERROR("Failed to build RefPathContext expression");
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
        DEBUG_ERROR("Building root expression failed");
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
    vsc::dm::ValRefInt val_i(m_ctxt->ctxt()->mkValRefInt(i->getValue(), false, i->getWidth()));
    DEBUG("val: %lld", val_i.get_val_s());
    m_expr = m_ctxt->ctxt()->mkTypeExprVal(val_i);
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
        DEBUG_ERROR("non-python static ref path unimplemented");
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
    if (!m_expr) {
        DEBUG_ERROR("Failed to build expression");
    }
    DEBUG_LEAVE("expr");
    return m_expr;
}

TaskBuildExpr::RootRefT TaskBuildExpr::mkRootFieldRef(ast::IExprRefPathContext *i) {
    DEBUG_ENTER("mkRootFieldRef");
    vsc::dm::ITypeExpr *field_ref = 0;

    if (DEBUG_EN) {
        for (std::vector<ast::SymbolRefPathElem>::const_iterator
            it=i->getTarget()->getPath().begin();
            it!=i->getTarget()->getPath().end(); it++) {
            DEBUG("it: kind=%d idx=%d", it->kind, it->idx);
        }
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
    zsp::parser::ScopeUtil scope(m_ctxt->rootSymScopeT<ast::ISymbolScope>());
    int32_t type_scope_idx=-1, bup_scope_idx=-1;
    ast::ISymbolScope *inline_ctxt = 0;
    uint32_t ii;
    if (DEBUG_EN) {
        for (ii=0; ii<i->getTarget()->getPath().size(); ii++) {
            DEBUG("Path[%d]: %d::%d", ii, 
                i->getTarget()->getPath().at(ii).kind,
                i->getTarget()->getPath().at(ii).idx);
        }
    }
    for (ii=0; ii<i->getTarget()->getPath().size(); ii++) {
        DEBUG("Scope: %s ; ii=%d idx=%d kind=%d children=%d", 
            scope.getName().c_str(), 
            ii,
            i->getTarget()->getPath().at(ii).idx,
            i->getTarget()->getPath().at(ii).kind,
            scope.getNumChildren());
        ast::IScopeChild *c = 0;
            
        switch (i->getTarget()->getPath().at(ii).kind) {
            case ast::SymbolRefPathElemKind::ElemKind_ArgIdx: {
                // scope is a function and we need to look in the
                // parameters scope
                ast::ISymbolFunctionScope *func = scope.getT<ast::ISymbolFunctionScope>();
                c = func->getPlist()->getChildren().at(
                    i->getTarget()->getPath().at(ii).idx).get();
            } break;
            case ast::SymbolRefPathElemKind::ElemKind_Inline: {
                DEBUG("ElemKind_Inline: %p", m_ctxt->inlineCtxt());
                inline_ctxt = m_ctxt->inlineCtxt();
            } break;
            default: {
                int32_t idx = i->getTarget()->getPath().at(ii).idx;
                DEBUG("Default: ii=%d kind=%d idx=%d size=%d",
                    ii, i->getTarget()->getPath().at(ii).kind, idx,
                    scope.getNumChildren());
                c = scope.getChild(idx);
            } break;
        }

        ast::ISymbolScope *ts = m_ctxt->typeScope();
        ast::ISymbolChildrenScope *ss = m_ctxt->symScope();
        DEBUG("Scope=%s typeScope=%s symScope=%s c=%p inline_ctxt=%p",
            scope.getName().c_str(), 
            ts->getName().c_str(),
            (ss)?ss->getName().c_str():"<none>",
            c,
            inline_ctxt);
        if (inline_ctxt) {
            DEBUG("Found inline type scope @ %d ", ii);
            break;
        } else if (c == m_ctxt->typeScope()) {
            DEBUG("Found type scope @ %d (%s)", ii,
                m_ctxt->typeScope()->getName().c_str());
            // we're looking at the *next* entry, so adjust 
            // the index accordingly
            type_scope_idx = ii+1;

            // Don't halt, since a bottom-up reference initially
            // looks like a top-down reference
        }
        if (ii+1 < i->getTarget()->getPath().size()) {

            // Search for bottom-up scopes until the penultimate
            // item. For a bottom-up reference, the last item
            // will always be a variable reference within the scope
            int32_t bup_scope_idx_t = m_ctxt->findBottomUpScope(scope.get());
            if (bup_scope_idx_t != -1) {
                // Find the first scope on the top-down path
                // that matches a bottom-up scope.
                DEBUG("bottom-up scope %d", bup_scope_idx_t);
                bup_scope_idx = bup_scope_idx_t;
                break;
            }

            if (!scope.init(c)) {
                DEBUG_ERROR("Failed to convert c to scope");
            }
        }
    }

    if (inline_ctxt) {
        field_ref = m_ctxt->ctxt()->mkTypeExprRefInline();
        DEBUG("inline_ctxt: field_ref=%p (ii=%d/%d)", 
            field_ref, ii, i->getTarget()->getPath().size());
        ii++;
        for (; ii<i->getTarget()->getPath().size(); ii++) {
            switch (i->getTarget()->getPath().at(ii).kind) {
                case ast::SymbolRefPathElemKind::ElemKind_ChildIdx: {
                    field_ref = m_ctxt->ctxt()->mkTypeExprSubField(
                        field_ref,
                        true,
                        i->getTarget()->getPath().at(ii).idx
                    );
                } break;
                default:
                    DEBUG_ERROR("Unexpected elem-kind %d", i->getTarget()->getPath().at(ii).kind);
                    break;
            }
        }
    } else {
        DEBUG("type_scope_idx=%d bup_scope_idx=%d ii=%d path.size=%d", 
            type_scope_idx, bup_scope_idx, ii, i->getTarget()->getPath().size());

        // Determine how to get to the root identifier
        // - It's a field within the current action
        // - It's a field relative to where we are
        //
        // Then, determine how to proceed
        // - 
        vsc::dm::ITypeExpr *expr = 0;
        if (bup_scope_idx != -1) {
            // First, keep going along the path to find the last
            // bottom-up scope on the path
            for (; ii<i->getTarget()->getPath().size()-1; ii++) {
                int32_t t_bup_scope_idx;
                int32_t idx = i->getTarget()->getPath().at(ii).idx;
                ast::IScopeChild *c = scope.getChild(idx);
                scope.init(c);

                if ((t_bup_scope_idx=m_ctxt->findBottomUpScope(scope.get())) != -1) {
                    DEBUG("Found bottom-up scope %d @ path index %d", t_bup_scope_idx, ii);
                    bup_scope_idx = t_bup_scope_idx;
                } else {
                    DEBUG("Reached the end of bottom-up scopes @ %d", ii);
                    break;
                }
            }

            DEBUG("Processing bottom-up reference");

            if (ii < i->getTarget()->getPath().size()) {
                DEBUG("Path elem: %d", i->getTarget()->getPath().at(ii).kind);
                switch (i->getTarget()->getPath().at(ii).kind) {
                    case ast::SymbolRefPathElemKind::ElemKind_ArgIdx: {
                        // The parameters scope will be above the function scope
                        DEBUG("Expression reference a parameter ; look one level above");
                        bup_scope_idx += 1;
                    } break;
                }
            }

            field_ref = m_ctxt->ctxt()->mkTypeExprRefBottomUp(
                bup_scope_idx,
                i->getTarget()->getPath().at(ii).idx);

            DEBUG("Bottom-up Ref: scope_offset=%d subfield_idx=%d", 
                bup_scope_idx,
                i->getTarget()->getPath().at(ii).idx);

            ii = 1;
        } else if (type_scope_idx != -1) {
            DEBUG("type_scope_idx=%d (PathSize-1)=%d",
                type_scope_idx,
                (i->getTarget()->getPath().size()-1));
    //        if (type_scope_idx == (i->getTarget()->getPath().size()-1)) {
                // Reference is to a field within the active type
            DEBUG("Type-context reference");
            field_ref = m_ctxt->ctxt()->mkTypeExprSubField(
                m_ctxt->ctxt()->mkTypeExprRefTopDown(),
                true,
                i->getTarget()->getPath().back().idx);
            ii = 1;

                // TODO: determine if this is actually a static reference
    //        } else {
    //            DEBUG_ERROR("Failed consistency check type_scope_idx=%d path.size=%d",
    //                type_scope_idx,
    //                i->getTarget()->getPath().size());
    //        }
        } else { // Neither top-down nor bottom-up context based
            if (i->getHier_id()->getElems().at(0)->getParams()) {
                DEBUG("Global function call");
                zsp::parser::TaskResolveSymbolPathRef resolver(
                    m_ctxt->getDebugMgr(),
                    m_ctxt->getRoot());
                ast::IScopeChild *ast_scope = resolver.resolve(i->getTarget());

                field_ref = buildRefExpr(
                    0,
                    i,
                    0,
                    ast_scope
                );
                ii = 1;
            } else {
                DEBUG_ERROR("Neither bottom-up or top-down");
            }
        }
    } // end !inline

    DEBUG_LEAVE("mkRootFieldRef {%p, %d}", field_ref, ii);
    return {field_ref, ii};
}

void TaskBuildExpr::extendHierFieldRef(
        vsc::dm::ITypeExprFieldRef      *root_ref,
        ast::IExprRefPathContext        *i) {
#ifdef UNDEFINED
    zsp::parser::TaskResolveSymbolPathRef resolver(
            m_ctxt->getDebugMgr(),
            m_ctxt->getRoot());
    ast::IScopeChild *ast_scope = resolver.resolve(i->getTarget());
    if (DEBUG_EN) {
        for (uint32_t ii=0; ii<i->getHier_id()->getElems().size(); ii++) {
            DEBUG("Path[%d]: %d", ii, i->getHier_id()->getElems().at(ii)->getTarget());
        }
    }

    // Now that we have a root, we can deal with building out the
    // full expression
    for (uint32_t ii=0; ii<i->getHier_id()->getElems().size(); ii++) {
        ast::IExprMemberPathElem *elem = i->getHier_id()->getElems().at(ii).get();
        int32_t target = elem->getTarget();

        if (elem->getSuper() > 0) {
            DEBUG("TODO: handle super of %d", elem->getSuper());
        }

        if (elem->getParams()) {
            // Func
            DEBUG("Elem[%d] has parameters (%s)", ii, elem->getId()->getId().c_str());

            if (ii) {
                ast_scope = zsp::parser::TaskIndexField(
                    m_ctxt->getDebugMgr(),
                    m_ctxt->getRoot()).index(
                        ast_scope,
                        elem->getTarget(),
                        elem->getSuper());
            }

            zsp::ast::IScopeChild *func_t = ast_scope;

            std::vector<vsc::dm::ITypeExpr *> params;

            for (std::vector<ast::IExprUP>::const_iterator
                it=elem->getParams()->getParameters().begin();
                it!=elem->getParams()->getParameters().end(); it++) {
                params.push_back(TaskBuildExpr(m_ctxt).build(it->get()));
            }

            if (zsp::parser::TaskIsPyRef(m_ctxt->getDebugMgr(), m_ctxt->getRoot()).check(func_t)) {
                DEBUG("Is a Python ref");

                vsc::dm::ITypeExpr *target;

                if (ii == 0) {
//                    target = expr;
                } else {
                    /*
                    target = m_ctxt->ctxt()->mkTypeExprPythonFieldRef(
                        expr,
                        true, 
                        elem->getId()->getId());
                     */
                }
                arl::dm::ITypeExprPythonMethodCall *call = m_ctxt->ctxt()->mkTypeExprPythonMethodCall(
                    target,
                    true,
                    params);
//                expr = call;
            } else {
                std::string fname = zsp::parser::TaskGetName().get(func_t, true);
                DEBUG("FunctionCallStatic: name=%s", fname.c_str());
                arl::dm::IDataTypeFunction *func = m_ctxt->ctxt()->findDataTypeFunction(fname);

                if (!func) {
                    DEBUG_ERROR("failed to find function %s", fname.c_str());
                }

                if (!expr) {
                    // Static function call
                    DEBUG("Elem %d: Static function call", ii);

                    DEBUG("Function Name: %s", fname.c_str());
                    arl::dm::ITypeExprMethodCallStaticUP call_e(
                        m_ctxt->ctxt()->mkTypeExprMethodCallStatic(
                        func,
                        params));
//                    expr = call_e.release();
                } else {
                    DEBUG("Elem %d: Context function call", ii);

                    // Context method call
                    /*
                    arl::dm::ITypeExprMethodCallContextUP call_e(
                        m_ctxt->ctxt()->mkTypeExprMethodCallContext(
                            func,
                            expr,
                            params
                        )
                    );
                    expr = call_e.release();
                     */
                }
            }
        } else if (elem->getSubscript()) {
            DEBUG("TODO: array subscript reference");
            if (!root_ref) {
                
            }
        } else {
            if (ii) {
                DEBUG("TODO: field subref expr=%p idx=%d", expr, elem->getTarget());
                TaskCalculateFieldOffset::Res res = TaskCalculateFieldOffset(m_ctxt).calculate(
                    ast_scope,
                    elem->getTarget(),
                    elem->getSuper());
                DEBUG("field_idx=%d super_idx=%d ; res.field_idx=%d",
                    elem->getTarget(),
                    elem->getSuper(),
                    res.index);

                if (!res.target) {
                    DEBUG_ERROR("null target");
                }

                if (!root_ref) {
                    /*
                    root_ref = m_ctxt->ctxt()->mkTypeExprFieldRef(expr, res.index);

                    if (root_ref->getPath().size() == 0) {
                        DEBUG_ERROR("bottom-up reference path is empty");
                    }
                    expr = field_ref;
                     */
                } else {
                    root_ref->addPathElem(res.index);
                }
                ast_scope = res.target;
                /*
                ast_scope = zsp::parser::TaskIndexField(
                    m_ctxt->getDebugMgr(),
                    m_ctxt->getRoot()).index(
                        ast_scope,
                        elem->getTarget(),
                        elem->getSuper());
                 */
            }
        }
    }
#endif /* UNDEFINED */
}

vsc::dm::ITypeExpr *TaskBuildExpr::buildRefExpr(
        vsc::dm::ITypeExpr              *root,
        ast::IExprRefPathContext        *i,
        int32_t                         idx,
        ast::IScopeChild                *ast_scope) {
    DEBUG_ENTER("buildRefExpr idx=%d offset=%d", 
        idx, 
        i->getHier_id()->getElems().at(idx)->getTarget());

    // Build this expression level
    ast::IExprMemberPathElem *elem = i->getHier_id()->getElems().at(idx).get();
    DEBUG("elem: %s", elem->getId()->getId().c_str());

    vsc::dm::ITypeExpr *expr = 0;

    if (elem->getParams()) {
        // Function call
        std::vector<vsc::dm::ITypeExpr *> params;

        if (idx > 0) {
            ast_scope = zsp::parser::TaskIndexField(
                m_ctxt->getDebugMgr(),
                m_ctxt->getRoot()).index(
                    ast_scope,
                    elem->getTarget(),
                    elem->getSuper());
        }

        zsp::ast::IScopeChild *func_t = ast_scope;

        for (std::vector<ast::IExprUP>::const_iterator
            it=elem->getParams()->getParameters().begin();
            it!=elem->getParams()->getParameters().end(); it++) {
            params.push_back(TaskBuildExpr(m_ctxt).build(it->get()));
        }

        std::string fname = zsp::parser::TaskGetName().get(func_t, true);
        arl::dm::IDataTypeFunction *func = m_ctxt->ctxt()->findDataTypeFunction(fname);

        if (!func) {
            DEBUG_ERROR("failed to find function %s", fname.c_str());
        }

        vsc::dm::ITypeExpr *ctxt = root;

        if (!ctxt) {
            // Static function call
            DEBUG("Elem %d: Static function call", idx);

            DEBUG("Function Name: %s", fname.c_str());
            expr = m_ctxt->ctxt()->mkTypeExprMethodCallStatic(
                func,
                params);
        } else {
            DEBUG("Elem %d: Context function call", idx);

            // Context method call
            expr = m_ctxt->ctxt()->mkTypeExprMethodCallContext(
                func,
                ctxt,
                params);
        }
    } else {
        // Just a regular subfield reference
        TaskCalculateFieldOffset::Res res = TaskCalculateFieldOffset(m_ctxt).calculate(
            ast_scope,
            elem->getTarget(),
            elem->getSuper());

        DEBUG("SubField: %d", res.index);

//        if (idx > 1) {
            std::string ast_scope_name = (ast_scope)?zsp::parser::TaskGetName().get(ast_scope, true):"<initial>";
            std::string ast_scope_name_n = (res.target)?zsp::parser::TaskGetName().get(res.target, true):"<unknown>";
            DEBUG("SubField: scope=%s target=%d super=%d new_scope=%s",
                ast_scope_name.c_str(), 
                elem->getTarget(),
                elem->getSuper(),
                ast_scope_name_n.c_str());
//        }

        expr = m_ctxt->ctxt()->mkTypeExprSubField(
            root,
            true,
            res.index);

        DEBUG("Source ast_scope: %s ; next_ast_scope: %s",
            (ast_scope)?zsp::parser::TaskGetName().get(ast_scope).c_str():"<unknown>",
            (res.target)?zsp::parser::TaskGetName().get(res.target).c_str():"<unknown>");
        ast_scope = res.target;

        if (elem->getSubscript().size()) {
            DEBUG("Subscript @ idx=%d", idx);
            // Array subscript
            for (std::vector<ast::IExprUP>::const_iterator
                s_it=elem->getSubscript().begin();
                s_it!=elem->getSubscript().end(); s_it++) {

                expr = m_ctxt->ctxt()->mkTypeExprArrIndex(
                    expr,
                    true,
                    TaskBuildExpr(m_ctxt).build(s_it->get()),
                    true);
                ast_scope = zsp::parser::TaskGetSubscriptSymbolScope(
                    m_ctxt->getDebugMgr(),
                    m_ctxt->getRoot()).resolve(ast_scope);
            }
        }
    }

    if (!expr) {
        DEBUG_ERROR("Failed to build ref-path expression");
    }

    if ((idx+1) < i->getHier_id()->getElems().size()) {
        expr = buildRefExpr(
            expr,
            i,
            idx+1,
            ast_scope);
    }

    DEBUG_LEAVE("buildRefExpr idx=%d", idx);
    return expr;
}

dmgr::IDebug *TaskBuildExpr::m_dbg = 0;

}
}
}
