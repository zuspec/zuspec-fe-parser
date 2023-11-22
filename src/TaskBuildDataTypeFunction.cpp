/*
 * TaskBuildDataTypeFunction.cpp
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
#include "zsp/parser/impl/TaskGetName.h"
#include "TaskBuildDataType.h"
#include "TaskBuildDataTypeFunction.h"
#include "TaskBuildTypeExecStmt.h"
#include "TaskBuildExpr.h"


namespace zsp {
namespace fe {
namespace parser {

static std::map<ast::ParamDir, arl::dm::ParamDir> param_dir_m = {
    {ast::ParamDir::ParamDir_Default, arl::dm::ParamDir::In},
    {ast::ParamDir::ParamDir_In, arl::dm::ParamDir::In},
    {ast::ParamDir::ParamDir_Out, arl::dm::ParamDir::Out},
    {ast::ParamDir::ParamDir_InOut, arl::dm::ParamDir::InOut} 
};


TaskBuildDataTypeFunction::TaskBuildDataTypeFunction(IAst2ArlContext *ctxt) 
    : m_ctxt(ctxt) {
    DEBUG_INIT("zsp::fe::parser::TaskBuildDataTypeFunction", ctxt->getDebugMgr());
}

TaskBuildDataTypeFunction::~TaskBuildDataTypeFunction() {

}

zsp::arl::dm::IDataTypeFunction *TaskBuildDataTypeFunction::build(
    ast::ISymbolFunctionScope *i) {
    std::string fname = zsp::parser::TaskGetName().get(i, true);
    DEBUG_ENTER("build %s", fname.c_str());

    ast::IFunctionPrototype *proto = i->getPrototypes().at(0);
    arl::dm::DataTypeFunctionFlags flags = arl::dm::DataTypeFunctionFlags::NoFlags;
    bool is_target = proto->getIs_target();
    bool is_solve  = proto->getIs_solve();

    if (!i->getDefinition()) {
        for (std::vector<ast::IFunctionImport *>::const_iterator
            it=i->getImport_specs().begin();
            it!=i->getImport_specs().end(); it++) {
            if ((*it)->getPlat() == ast::PlatQual::PlatQual_Target) {
                is_target = true;
            }
            if ((*it)->getPlat() == ast::PlatQual::PlatQual_Solve) {
                is_solve = true;
            }
        }

        flags = flags | arl::dm::DataTypeFunctionFlags::Import;

        if (is_target && is_solve) {
            is_target = false;
            is_solve = false;
        } else {
            if (is_target) {
                flags = flags | arl::dm::DataTypeFunctionFlags::Target;
            }
            if (is_solve) {
                flags = flags | arl::dm::DataTypeFunctionFlags::Solve;
            }
        }
    }
//    ast::IScopeChild *rtype = i->getDefinition()->getProto()->getRtype();
    ast::IScopeChild *rtype = proto->getRtype();
    arl::dm::IDataTypeFunction *func = m_ctxt->ctxt()->mkDataTypeFunction(
        fname,
        rtype?TaskBuildDataType(m_ctxt).build(rtype):0,
        false,
        flags);

    // Bring across the function parameters
    for (std::vector<ast::IFunctionParamDeclUP>::const_iterator
        it=proto->getParameters().begin();
        it!=proto->getParameters().end(); it++) {
        std::string name = (*it)->getName()->getId();
        arl::dm::ParamDir dir = param_dir_m.find((*it)->getDir())->second;
        vsc::dm::IDataType *type = TaskBuildDataType(m_ctxt).build((*it)->getType());
        vsc::dm::ITypeExpr *dflt = ((*it)->getDflt())?TaskBuildExpr(m_ctxt).build((*it)->getDflt()):0;
        arl::dm::IDataTypeFunctionParamDecl *param = 
        m_ctxt->ctxt()->mkDataTypeFunctionParamDecl(
            name,
            dir,
            type,
            false,
            dflt
        );
        func->addParameter(param);
    }
    m_ctxt->ctxt()->addDataTypeFunction(func);

    if (i->getDefinition()) {
        // Local implementation
        DEBUG("PSS-native function");

        m_ctxt->pushSymScope(i);


        m_ctxt->pushSymScope(i->getBody());
        for (std::vector<ast::IExecStmtUP>::const_iterator
            it=i->getDefinition()->getBody()->getChildren().begin();
            it!= i->getDefinition()->getBody()->getChildren().end(); it++) {
            TaskBuildTypeExecStmt(m_ctxt).build(
                func->getBody(),
                it->get());
        }

        m_ctxt->popSymScope();
        m_ctxt->popSymScope();
    } else {
        DEBUG("Import function");
        func->addImportSpec(m_ctxt->ctxt()->mkDataTypeFunctionImport(
            "", is_target, is_solve));
    }

    DEBUG_LEAVE("build %s", fname.c_str());
    return func;
}

dmgr::IDebug *TaskBuildDataTypeFunction::m_dbg = 0;

}
}
}
