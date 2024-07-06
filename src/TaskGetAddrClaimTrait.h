/**
 * TaskGetAddrClaimTrait.h
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
#pragma once
#include "dmgr/IDebugMgr.h"
#include "dmgr/impl/DebugMacros.h"
#include "zsp/ast/impl/VisitorBase.h"
#include "zsp/parser/impl/TaskResolveSymbolPathRef.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskGetAddrClaimTrait : public ast::VisitorBase {
public:

    TaskGetAddrClaimTrait(
        dmgr::IDebugMgr         *dmgr,
        ast::ISymbolScope       *root) : m_dbg(0), m_dmgr(dmgr), m_root(root) {
        DEBUG_INIT("zsp::fe::parser::TaskGetAddrClaimTrait", dmgr);
    }

    virtual ~TaskGetAddrClaimTrait() { }

    ast::IScopeChild *get(ast::IScopeChild *claim_t) {
        DEBUG_ENTER("get");
        m_depth = 0;
        m_trait_t = 0;
        claim_t->accept(m_this);
        DEBUG_LEAVE("get %p", m_trait_t);
        return m_trait_t;
    }

    virtual void visitDataTypeUserDefined(ast::IDataTypeUserDefined *i) override {
        DEBUG_ENTER("visitDataTypeUserDefined %s", i->getType_id()->getElems().at(0)->getId()->getId().c_str());
        ast::IScopeChild *type_t = zsp::parser::TaskResolveSymbolPathRef(
            m_dmgr, m_root).resolve(i->getType_id()->getTarget());
        if (m_depth) {
            m_trait_t = type_t;
        } else {
            type_t->accept(m_this);
        }
        DEBUG_LEAVE("visitDataTypeUserDefined");
    }

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override {
        DEBUG_ENTER("visitSymbolTypeScope %s", i->getName().c_str());
        i->getTarget()->accept(m_this);
        DEBUG_LEAVE("visitSymbolTypeScope %s", i->getName().c_str());
    }

    virtual void visitTypeScope(ast::ITypeScope *i) override {
        DEBUG_ENTER("visitTypeScope %s", i->getName()->getId().c_str());
        m_depth++;
        if (i->getSuper_t()) {
            ast::IScopeChild *super_t = zsp::parser::TaskResolveSymbolPathRef(
                m_dmgr, m_root).resolve(i->getSuper_t()->getTarget());
            super_t->accept(m_this);
        }

        if (i->getParams() && !m_trait_t) {
            // Root parameterized type. Should be able to grab pval from here
            i->getParams()->getParams().at(0)->accept(m_this);
        }

        m_depth--;
        DEBUG_LEAVE("visitTypeScope %s", i->getName()->getId().c_str());
    }

    virtual void visitTemplateGenericTypeParamDecl(ast::ITemplateGenericTypeParamDecl *i) override {
        DEBUG_ENTER("visitTemplateGenericTypeParamDecl");
        i->getDflt()->accept(m_this);
        DEBUG_LEAVE("visitTemplateGenericTypeParamDecl");
    }

    virtual void visitTemplateCategoryTypeParamDecl(ast::ITemplateCategoryTypeParamDecl *i) override {
        DEBUG_ENTER("visitTemplateCategoryTypeParamDecl");
        i->getDflt()->accept(m_this);
        DEBUG_LEAVE("visitTemplateCategoryTypeParamDecl");
    }

    virtual void visitTemplateParamTypeValue(ast::ITemplateParamTypeValue *i) override {
        DEBUG_ENTER("visitTemplateParamTypeValue");
        i->getValue()->accept(m_this);
        DEBUG_LEAVE("visitTemplateParamTypeValue");
    }

    virtual void visitTemplateParamExprValue(ast::ITemplateParamExprValue *i) override {
        DEBUG_ENTER("visitTemplateParamExprValue");
        DEBUG_LEAVE("visitTemplateParamExprValue");
    }
    

private:
    dmgr::IDebug                *m_dbg;
    dmgr::IDebugMgr             *m_dmgr;
    ast::ISymbolScope           *m_root;
    ast::IScopeChild            *m_trait_t;
    int32_t                     m_depth;

};

} /* namespace parser */
} /* namespace fe */
} /* namespace zsp */


