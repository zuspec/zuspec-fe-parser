/*
 * TaskBuildEnumType.cpp
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
#include "zsp/ast/IEnumDecl.h"
#include "TaskBuildEnumType.h"

#define DEBUG_ENTER(fmt, ...) \
	fprintf(stdout, "--> TaskBuildEnumType::"); \
	fprintf(stdout, fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n")

#define DEBUG(fmt, ...) \
	fprintf(stdout, "TaskBuildEnumType: "); \
	fprintf(stdout, fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n")

#define DEBUG_LEAVE(fmt, ...) \
	fprintf(stdout, "<-- TaskBuildEnumType::"); \
	fprintf(stdout, fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n")

namespace zsp {
namespace fe {
namespace parser {


TaskBuildEnumType::TaskBuildEnumType(arl::IContext *ctxt) : m_ctxt(ctxt) {

}

TaskBuildEnumType::~TaskBuildEnumType() {

}

vsc::IDataTypeEnum *TaskBuildEnumType::build(
    std::string             &ns_prefix,
    ast::ISymbolEnumScope   *ast_enum) {
    DEBUG_ENTER("build");
    m_ns_prefix = ns_prefix;
    m_dt_enum = m_ctxt->mkDataTypeEnum(ns_prefix + ast_enum->getName(), true);
    m_ctxt->addDataTypeEnum(m_dt_enum);
    for (std::vector<ast::IScopeChild *>::const_iterator
        it=ast_enum->getChildren().begin();
        it!=ast_enum->getChildren().end(); it++) {
        (*it)->accept(this);
    }
    DEBUG_LEAVE("build");
    return m_dt_enum;
}

void TaskBuildEnumType::visitEnumItem(ast::IEnumItem *i) {
    DEBUG_ENTER("visitEnumItem");
    m_dt_enum->addEnumerator(
        i->getName()->getId(),
        0 // TODO: must support value
    );
    DEBUG_LEAVE("visitEnumItem");
}

}
}
}
