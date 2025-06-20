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
#include "zsp/fe/parser/IElemFactoryAssocData.h"
#include "TaskBuildActivity.h"
#include "TaskBuildDataType.h"
#include "TaskBuildDataTypeFunction.h"
#include "TaskBuildField.h"
#include "TaskBuildTypeConstraints.h"
#include "TaskBuildTypeExecs.h"
#include "TaskBuildTypeFunctions.h"
#include "TaskGetDataTypeAssocData.h"
#include "zsp/parser/impl/TaskResolveSymbolPathRef.h"
#include "zsp/parser/impl/TaskEvalExpr.h"


namespace zsp {
namespace fe {
namespace parser {

TaskBuildDataType::TaskBuildDataType(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("TaskBuildDataType", ctxt->getDebugMgr());
    m_type = 0;
    m_depth = 0;
}

TaskBuildDataType::~TaskBuildDataType() {

}

vsc::dm::IAccept *TaskBuildDataType::build(ast::IScopeChild *type) {
    DEBUG_ENTER("build");
    m_type = 0;
    m_depth = 0;

    type->accept(this);

    if (!m_type) {
        DEBUG_ERROR("Failed to produce a data type");
        m_type = m_ctxt->ctxt()->findDataTypeInt(true, 32);
    }

    DEBUG_LEAVE("build");
    return m_type;
}

vsc::dm::IAccept *TaskBuildDataType::build(ast::IExpr *type) {
    DEBUG_ENTER("build(expr)");
    m_type = 0;
    m_depth = 0;

    type->accept(this);

    if (!m_type) {
        DEBUG_ERROR("Failed to produce a data type");
        m_type = m_ctxt->ctxt()->findDataTypeInt(true, 32);
    }

    DEBUG_LEAVE("build(expr)");
    return m_type;
}

vsc::dm::IAccept *TaskBuildDataType::build(ast::ITypeIdentifier *type) {
    DEBUG_ENTER("build(type-id)");
    ast::IScopeChild *ref_t = zsp::parser::TaskResolveSymbolPathRef(
        m_ctxt->getDebugMgr(),
        m_ctxt->getRoot()).resolve(type->getTarget());

    ref_t->accept(m_this);

    if (!m_type) {
        DEBUG_ERROR("Failed to produce a data type");
        m_type = m_ctxt->ctxt()->findDataTypeInt(true, 32);
    }

    DEBUG_LEAVE("build(type-id) %p", m_type);
    return m_type;
}

void TaskBuildDataType::visitSymbolFunctionScope(ast::ISymbolFunctionScope *i) {
    DEBUG_ENTER("visitSymbolFunctionScope %s (%d)", i->getName().c_str(), m_depth);
    if (!m_depth) {
    char tmp[128];
    sprintf(tmp, "__xyz_%p", i);
    vsc::dm::ITypeField *field = m_ctxt->ctxt()->mkTypeFieldPhy(tmp,
        m_ctxt->ctxt()->getDataTypeCore(vsc::dm::DataTypeCoreE::Bool),
        false,
        vsc::dm::TypeFieldAttr::NoAttr,
        0);
    m_type_s.back()->addField(field);
    }
#ifdef UNDEFINED


//    TaskBuildDataTypeFunction(m_ctxt).build(i, true);
#endif // UNDEFINED
    DEBUG_LEAVE("visitSymbolFunctionScope");
}

void TaskBuildDataType::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope %s", i->getName().c_str());
    ast::ITypeScope *i_ts = dynamic_cast<ast::ITypeScope *>(i->getTarget());

    if (i_ts->getParams()) {
        DEBUG("Type has parameters");
        if (i_ts->getParams()->getSpecialized()) {
            DEBUG("Type is specialized (%p)", i->getTarget());
            m_ctxt->pushSymScopeStack(i);
            i->getTarget()->accept(this);
            m_ctxt->popSymScopeStack();
        } else {
            DEBUG("Type is unspecialized (%d specializations)", i->getSpec_types().size());
            for (std::vector<ast::ISymbolTypeScopeUP>::const_iterator
                it=i->getSpec_types().begin();
                it!=i->getSpec_types().end(); it++) {
                (*it)->accept(this);
            }
        }
    } else {
        m_ctxt->pushSymScopeStack(i);
        i->getTarget()->accept(this);
        m_ctxt->popSymScopeStack();
    }

    DEBUG_LEAVE("visitSymbolTypeScope");
}

void TaskBuildDataType::visitAction(ast::IAction *i) {
    DEBUG_ENTER("visitAction %s", i->getName()->getId().c_str());
    if (!m_depth && !(m_type=findType(m_ctxt->symScope()))) {
        // We're at top level and the type doesn't exist yet, so let's do it!
    
        std::string fullname;
        if (i->getParams()) {
            fullname = getNamespacePrefix();
            fullname = fullname.substr(0, fullname.size()-2);
        } else {
            fullname = getNamespacePrefix() + i->getName()->getId();
        }
        DEBUG("Building Action Type: %s", fullname.c_str());
        arl::dm::IDataTypeAction *action_t = m_ctxt->ctxt()->mkDataTypeAction(fullname);
        m_ctxt->ctxt()->addDataTypeStruct(action_t);

        m_ctxt->addType(m_ctxt->symScope(), action_t);

        buildType(action_t, dynamic_cast<ast::ISymbolTypeScope *>(m_ctxt->symScope()));

        // Get the created type and connect up to its component
        if (m_type_s.size()) {
            arl::dm::IDataTypeComponent *comp_t = dynamic_cast<arl::dm::IDataTypeComponent *>(m_type_s.back());
            comp_t->addActionType(action_t);
        }

        if (i->getSuper_t()) {
            DEBUG("Has a super type");
            vsc::dm::IDataType *super_t = TaskBuildDataType(m_ctxt).buildT<vsc::dm::IDataType>(i->getSuper_t());
            action_t->setSuper(dynamic_cast<vsc::dm::IDataTypeStruct *>(super_t));
        }

        m_type = action_t;
    }

    // Note: there won't be any other types declared inside an action
    DEBUG_LEAVE("visitAction %s", i->getName()->getId().c_str());
}

void TaskBuildDataType::visitActivityDecl(ast::IActivityDecl *i) {
    DEBUG_ENTER("visitActivityDecl");
    arl::dm::IDataTypeAction *action;
    arl::dm::IDataTypeComponent *comp = dynamic_cast<arl::dm::IDataTypeComponent *>(m_type_s.back());

    if ((action=dynamic_cast<arl::dm::IDataTypeAction *>(m_type_s.back()))) {
        action->addActivity(m_ctxt->ctxt()->mkTypeFieldActivity(
            "activity",
            TaskBuildActivity(m_ctxt).build(i),
            true));
    } else if ((comp=dynamic_cast<arl::dm::IDataTypeComponent *>(m_type_s.back()))) {
        comp->addActivity(m_ctxt->ctxt()->mkTypeFieldActivity(
            "activity",
            TaskBuildActivity(m_ctxt).build(i),
            true));
    } else {
        DEBUG_ERROR("Expected component or action type, but got %p", m_type_s.back());
        return;
    }
    DEBUG_LEAVE("visitActivityDecl");
}

void TaskBuildDataType::visitComponent(ast::IComponent *i) {
    DEBUG_ENTER("visitComponent m_depth=%d", m_depth);
//    if (!m_depth) {
    arl::dm::IDataTypeComponent *comp_t = 0;
    if (!m_depth && !(m_type=findType(m_ctxt->symScope()))) {
        // We're at top level and the type doesn't exist yet, so let's do it!
        zsp::ast::IAssocData *assoc_d = TaskGetDataTypeAssocData(m_ctxt).get(m_ctxt->symScope());
        IElemFactoryAssocData *elem_f = dynamic_cast<IElemFactoryAssocData *>(assoc_d);

        std::string fullname;
        if (i->getParams()) {
            fullname = getNamespacePrefix();
            fullname = fullname.substr(0, fullname.size()-2);
        } else {
            fullname = getNamespacePrefix() + i->getName()->getId();
        }
        DEBUG("Building Component Type: %s", fullname.c_str());
        if (elem_f && (comp_t=dynamic_cast<arl::dm::IDataTypeComponent *>(
                elem_f->mkDataType(m_ctxt, fullname, i)))) {
            DEBUG("Using elem-factory version");
        } else {
            comp_t = m_ctxt->ctxt()->mkDataTypeComponent(fullname);
        }

        m_ctxt->ctxt()->addDataTypeStruct(comp_t);
        m_ctxt->addType(m_ctxt->symScope(), comp_t);

        buildType(comp_t, dynamic_cast<ast::ISymbolTypeScope *>(m_ctxt->symScope()));

        // Now, back at depth 0, visit children to build out other types
        m_type_s.push_back(comp_t);
        for (std::vector<ast::IScopeChildUP>::const_iterator
            it=m_ctxt->symScope()->getChildren().begin();
            it!=m_ctxt->symScope()->getChildren().end(); it++) {
            (*it)->accept(this);
        }
        m_type_s.pop_back();

        if (i->getSuper_t()) {
            DEBUG("Has a super type");
            vsc::dm::IDataType *super_t = TaskBuildDataType(m_ctxt).buildT<vsc::dm::IDataType>(i->getSuper_t());
            comp_t->setSuper(dynamic_cast<vsc::dm::IDataTypeStruct *>(super_t));
        }

        m_type = comp_t;
    }

    DEBUG_LEAVE("visitComponent");
}

void TaskBuildDataType::visitDataTypeBool(ast::IDataTypeBool *i) { 
    DEBUG_ENTER("visitDataTypeBool");
    m_type = m_ctxt->ctxt()->getDataTypeCore(vsc::dm::DataTypeCoreE::Bool);
    DEBUG_LEAVE("visitDataTypeBool");
}

void TaskBuildDataType::visitDataTypeChandle(ast::IDataTypeChandle *i) { 
    DEBUG_ENTER("visitDataTypeChandle");

    DEBUG_LEAVE("visitDataTypeChandle");
}

void TaskBuildDataType::visitDataTypeEnum(ast::IDataTypeEnum *i) { 
    DEBUG_ENTER("visitDataTypeEnum");

    // TODO: handle base type (if present)


    DEBUG_LEAVE("visitDataTypeEnum");
}

void TaskBuildDataType::visitDataTypeInt(ast::IDataTypeInt *i) { 
    DEBUG_ENTER("visitDataTypeInt");
    // TODO:
    int32_t width = i->getIs_signed()?32:1;

    if (i->getWidth()) {
        zsp::parser::IVal *width_v = zsp::parser::TaskEvalExpr(
            m_ctxt->factory(),
            m_ctxt->getRoot()).eval(i->getWidth());

        if (width_v) {
            if (width_v->getKind() == zsp::parser::ValKind::Int) {
                width = dynamic_cast<zsp::parser::IValInt *>(width_v)->getValU();
            } else {
                DEBUG("Error: data-type width expression is %d, not int", width_v->getKind());
            }
        } else {
            DEBUG("Error: data-type width expression produced null result");
        }
    }

    vsc::dm::IDataTypeInt *t = m_ctxt->ctxt()->findDataTypeInt(
        i->getIs_signed(),
        width);

    if (!t) {
        DEBUG("Create int signed=%d width=%d (ctxt=%p %p)", 
            i->getIs_signed(),
            width,
            m_ctxt,
            m_ctxt->ctxt());
        t = m_ctxt->ctxt()->mkDataTypeInt(
            i->getIs_signed(),
            width);
        m_ctxt->ctxt()->addDataTypeInt(t);
    }

    m_type = t;

    DEBUG_LEAVE("visitDataTypeInt");
}

void TaskBuildDataType::visitDataTypePyObj(ast::IDataTypePyObj *i) {
    DEBUG_ENTER("visitDataTypePyObj");
    m_type = m_ctxt->ctxt()->getDataTypeCoreArl(arl::dm::DataTypeCoreE::PyObj);
    DEBUG_LEAVE("visitDataTypePyObj");
}

void TaskBuildDataType::visitDataTypeString(ast::IDataTypeString *i) {
    DEBUG_ENTER("visitDataTypeString");
    m_type = m_ctxt->ctxt()->getDataTypeCore(vsc::dm::DataTypeCoreE::String);
    DEBUG_LEAVE("visitDataTypeString");
}

void TaskBuildDataType::visitDataTypeUserDefined(ast::IDataTypeUserDefined *i) { 
    DEBUG_ENTER("visitDataTypeUserDefined");
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

    zsp::parser::TaskResolveSymbolPathRefResult result = resolveTypePath(i->getType_id()->getTarget());

    DEBUG("kind=%d", result.kind);
    fflush(stdout);

    switch (result.kind) {
        case zsp::parser::TaskResolveSymbolPathRefResult::SymbolTypeScope:
            result.val.ts->accept(m_this);
            break;
        case zsp::parser::TaskResolveSymbolPathRefResult::SymbolScope:
            DEBUG("SymbolScope: %s", result.val.ss->getName().c_str());
            result.val.ss->accept(m_this);
            break;
        case zsp::parser::TaskResolveSymbolPathRefResult::DataType:
            result.val.dt->accept(m_this);
            break;
        default:
            DEBUG("Unhandled case");
            break;
    }

    DEBUG_LEAVE("visitDataTypeUserDefined (%p)", m_type);
}

void TaskBuildDataType::visitSymbolEnumScope(ast::ISymbolEnumScope *i) {
    DEBUG_ENTER("visitEnumSymbolScope");
    std::string fullname = getNamespacePrefix() + i->getName().c_str();

    vsc::dm::IDataTypeEnum *dt = m_ctxt->ctxt()->findDataTypeEnum(fullname);

    if (!dt) {
        dt = m_ctxt->ctxt()->mkDataTypeEnum(fullname, true);
        m_ctxt->ctxt()->addDataTypeEnum(dt);
    }

    m_type = dt;

    DEBUG_LEAVE("visitEnumSymbolScope");
}

void TaskBuildDataType::visitExecBlock(ast::IExecBlock *i) {
    DEBUG_ENTER("visitExecBlock");
    DEBUG_LEAVE("visitExecBlock");
}

void TaskBuildDataType::visitExecScope(ast::IExecScope *i) {
    DEBUG_ENTER("visitExecScope");
    DEBUG_LEAVE("visitExecScope");
}

void TaskBuildDataType::visitStruct(ast::IStruct *i) {
    DEBUG_ENTER("visitStruct %s", i->getName()->getId().c_str());
    if (!m_depth && !(m_type=findType(m_ctxt->symScope()))) {
        if (!i->getParams() || i->getParams()->getSpecialized()) {

//        if (!i->getParams() || i->getParams())
        // We're at top level and the type doesn't exist yet, so let's do it!
    
        zsp::ast::IAssocData *assoc_d = TaskGetDataTypeAssocData(m_ctxt).get(m_ctxt->symScope());
        IElemFactoryAssocData *elem_f = dynamic_cast<IElemFactoryAssocData *>(assoc_d);

        vsc::dm::IDataType *type_t = 0;
        vsc::dm::IDataTypeStruct *struct_t = 0;
        std::string fullname;
        
        if (i->getParams()) {
            fullname = getNamespacePrefix();
            fullname = fullname.substr(0, fullname.size()-2);
        } else {
            fullname = getNamespacePrefix() + i->getName()->getId();
        }
        DEBUG("Fullname: %s (ns=%s)", fullname.c_str(), getNamespacePrefix().c_str());
        if (elem_f && (type_t=elem_f->mkDataType(m_ctxt, fullname, i))) {
            DEBUG("Using elem-factory version");
            struct_t = dynamic_cast<vsc::dm::IDataTypeStruct *>(type_t);
        } else {
            struct_t = m_ctxt->ctxt()->mkDataTypeStruct(fullname);
            type_t = struct_t;
        }
        if (struct_t) {
            m_ctxt->ctxt()->addDataTypeStruct(struct_t);
        }
        m_ctxt->addType(m_ctxt->symScope(), type_t);

        if (struct_t) {
            buildType(struct_t, dynamic_cast<ast::ISymbolTypeScope *>(m_ctxt->symScope()));
        }

        if (i->getSuper_t()) {
            DEBUG("Has a super type");
            vsc::dm::IDataType *super_t = TaskBuildDataType(m_ctxt).buildT<vsc::dm::IDataType>(i->getSuper_t());
            struct_t->setSuper(dynamic_cast<vsc::dm::IDataTypeStruct *>(super_t));
        }
        m_type = type_t;

        } else {
            DEBUG("Skip building type for unspecialized template");
        }
    } else {
        DEBUG("Not building (depth=%d type=%p)", m_depth, m_type);
    }

    // Note: there won't be any other types declared inside a struct

    DEBUG_LEAVE("visitStruct");
}

void TaskBuildDataType::visitTypeIdentifier(ast::ITypeIdentifier *i) {
    DEBUG_ENTER("visitTypeIdentifier");
    ast::IScopeChild *ref_t = zsp::parser::TaskResolveSymbolPathRef(
        m_ctxt->getDebugMgr(),
        m_ctxt->getRoot()).resolve(i->getTarget());

    ref_t->accept(m_this);

    if (!m_type) {
        DEBUG_ERROR("Failed to produce a data type");
        m_type = m_ctxt->ctxt()->findDataTypeInt(true, 32);
    }
    DEBUG_LEAVE("visitTypeIdentifier");
}

void TaskBuildDataType::visitTypeScope(ast::ITypeScope *i) {
    DEBUG_ENTER("visitTypeScope %s", i->getName()->getId().c_str());

    if (!m_depth && !(m_type=findType(m_ctxt->symScope()))) {
        zsp::ast::IAssocData *assoc_d = TaskGetDataTypeAssocData(m_ctxt).get(m_ctxt->symScope());
        IElemFactoryAssocData *elem_f = dynamic_cast<IElemFactoryAssocData *>(assoc_d);

        vsc::dm::IDataType *dt = 0;
        std::string fullname = getNamespacePrefix() + i->getName()->getId();
        if (elem_f && (dt=elem_f->mkDataType(m_ctxt, fullname, i))) {
            DEBUG("Using result of element factory");
            m_ctxt->addType(m_ctxt->symScope(), dt);

            m_type = dt;
        } else {
            DEBUG_ERROR("Bare TypeScope must provide an element factory");
        }
    }

    DEBUG_LEAVE("visitTypeScope %s", i->getName()->getId().c_str());
}

void TaskBuildDataType::visitField(ast::IField *i) { 
    DEBUG_ENTER("visitField %s %d", i->getName()->getId().c_str(), m_depth);

    if (m_depth) {
        // Note: we want to control when fields are built, so
        // we sequence that
        vsc::dm::ITypeField *field = TaskBuildField(m_ctxt).build(i);
        m_type_s.back()->addField(field, true);
    }

    DEBUG_LEAVE("visitField %s %d", i->getName()->getId().c_str(), m_depth);
}

void TaskBuildDataType::visitFieldCompRef(ast::IFieldCompRef *i) {
    DEBUG_ENTER("visitFieldCompRef");

    /*
    if (m_depth) {
        // Note: we want to control when fields are built, so
        // we sequence that
        vsc::dm::ITypeField *field = TaskBuildField(m_ctxt).build(i);
        if (field) {
            m_type_s.back()->addField(field, true);
        }
    }
     */

    DEBUG_LEAVE("visitFieldCompRef");
}

void TaskBuildDataType::visitFieldRef(ast::IFieldRef *i) {
    DEBUG_ENTER("visitFieldRef");
    if (m_depth) {
        // Note: we want to control when fields are built, so
        // we sequence that
        vsc::dm::ITypeField *field = TaskBuildField(m_ctxt).build(i);
        if (field) {
            m_type_s.back()->addField(field, true);
        }
    }

    DEBUG_LEAVE("visitFieldRef");
}

void TaskBuildDataType::visitFieldClaim(ast::IFieldClaim *i) {
    DEBUG_ENTER("visitFieldClaim");

    DEBUG_LEAVE("visitFieldClaim");
}
    
void TaskBuildDataType::buildType(
        vsc::dm::IDataTypeStruct    *arl_type,
        ast::ISymbolTypeScope       *ast_type) {
    DEBUG_ENTER("buildType %s (%d)", arl_type->name().c_str(), m_depth);

    m_depth++;

    m_field_off = 0;
    // Depth 1: build fields
    std::vector<int32_t> offset_l;
    m_type_s.push_back(arl_type);
    buildTypeFields(
        offset_l,
        arl_type,
        ast_type);

    std::map<std::string, ast::IConstraintScope *> c_inherit_m;

    buildTypeConstraints(
        c_inherit_m,
        arl_type,
        ast_type);

    std::map<std::string, ast::IConstraintScope *> f_inherit_m;
    buildTypeFunctions(
        f_inherit_m,
        arl_type,
        ast_type);

    m_type_s.pop_back();

    m_depth--;

    TaskBuildTypeExecs(m_ctxt).build(
        dynamic_cast<arl::dm::IDataTypeArlStruct *>(arl_type), 
        ast_type);

    DEBUG_LEAVE("buildType %s (%d)", arl_type->name().c_str(), m_depth);
}

void TaskBuildDataType::buildTypeConstraints(
    std::map<std::string, ast::IConstraintScope *>      &c_inherit_m,
    vsc::dm::IDataTypeStruct                            *arl_type,
    ast::ISymbolTypeScope                               *ast_type) {
    DEBUG_ENTER("buildTypeConstraints");

    TaskBuildTypeConstraints(m_ctxt, arl_type).build(ast_type);

    DEBUG_LEAVE("buildTypeConstraints");
}

void TaskBuildDataType::buildTypeFields(
    std::vector<int32_t>                &off_l,
    vsc::dm::IDataTypeStruct            *arl_type,
    ast::ISymbolTypeScope               *ast_type) {
    DEBUG_ENTER("buildTypeFields %d", m_depth);
        
    // Recurse first
    ast::ITypeScope *target_t = dynamic_cast<ast::ITypeScope *>(ast_type->getTarget());
    if (target_t->getSuper_t()) {
        if (target_t->getSuper_t()->getTarget()) {
            ast::IScopeChild *super_t_ast = resolvePath(target_t->getSuper_t()->getTarget());
//        vsc::dm::IDataType *super_t_arl = TaskBuildDataType(m_ctxt).build(super_t_ast);

            // Now, build the super-type fields
            buildTypeFields(off_l, arl_type, dynamic_cast<ast::ISymbolTypeScope *>(super_t_ast));
        } else {
            DEBUG_ERROR("Super type not resolved for %s", 
                target_t->getName()->getId().c_str());
        }
    }

    // Record how many fields are in 'super'
    off_l.push_back(arl_type->getFields().size());

    for (std::vector<ast::IScopeChildUP>::const_iterator
        it=ast_type->getChildren().begin();
        it!=ast_type->getChildren().end(); it++) {
        (*it)->accept(m_this);
    }
    DEBUG_LEAVE("buildTypeFields %d", m_depth);
}

void TaskBuildDataType::buildTypeExecs(
        std::vector<int32_t>        &off_l,
        vsc::dm::IDataTypeStruct    *arl_type,
        ast::ISymbolTypeScope       *ast_type) {
    DEBUG_ENTER("buildTypeExecs");

    DEBUG_LEAVE("buildTypeExecs");
}

void TaskBuildDataType::buildTypeFunctions(
        std::map<std::string, ast::IConstraintScope *>      &c_inherit_m,
        vsc::dm::IDataTypeStruct                            *arl_type,
        ast::ISymbolTypeScope                               *ast_type) {
    DEBUG_ENTER("buildTypeFunctions");

    TaskBuildTypeFunctions(m_ctxt, arl_type).build(ast_type);

    DEBUG_LEAVE("buildTypeFunctions");
}

std::string TaskBuildDataType::getNamespacePrefix() {
    std::string ret;
    for (std::vector<zsp::parser::ScopeUtil>::const_iterator
        it=m_ctxt->symScopes().begin();
        it+1!=m_ctxt->symScopes().end(); it++) {
        zsp::parser::ScopeUtil &util = const_cast<zsp::parser::ScopeUtil &>(*it);

        if (util.getName() != "") {
            ret += util.getName();
            ret += "::";
        }
    }
    return ret;
}

vsc::dm::IAccept *TaskBuildDataType::findType(ast::IScopeChild *ast_t) {
    return m_ctxt->findType(ast_t);
}

ast::IScopeChild *TaskBuildDataType::resolvePath(ast::ISymbolRefPath *ref) {
    ast::ISymbolScope *scope = m_ctxt->rootSymScopeT<ast::ISymbolScope>();
    return zsp::parser::TaskResolveSymbolPathRef(
        m_ctxt->getDebugMgr(), 
        scope).resolve(ref);
}

zsp::parser::TaskResolveSymbolPathRefResult TaskBuildDataType::resolveTypePath(ast::ISymbolRefPath *ref) {
    ast::ISymbolScope *scope = m_ctxt->rootSymScopeT<ast::ISymbolScope>();
    return zsp::parser::TaskResolveSymbolPathRef(
        m_ctxt->getDebugMgr(), 
        scope).resolveFull(ref);
}

dmgr::IDebug *TaskBuildDataType::m_dbg = 0;

}
}
}
