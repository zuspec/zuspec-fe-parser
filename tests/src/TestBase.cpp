/*
 * TestBase.cpp
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
#include <sstream>
#include "dmgr/FactoryExt.h"
#include "vsc/dm/FactoryExt.h"
#include "zsp/arl/dm/FactoryExt.h"
#include "zsp/fe/parser/FactoryExt.h"
#include "zsp/parser/FactoryExt.h"
#include "TestBase.h"


namespace zsp {
namespace fe {
namespace parser {


TestBase::TestBase() {

}

TestBase::~TestBase() {

}

extern "C" zsp::ast::IFactory *ast_getFactory();

void TestBase::SetUp() {
    dmgr::IFactory *dmgr_f = dmgr_getFactory();
    vsc::dm::IFactory *vsc_dm_f = vsc_dm_getFactory();
    vsc_dm_f->init(dmgr_f->getDebugMgr());

    m_factory = zsp_fe_parser_getFactory();
    m_zsp_factory = zsp_parser_getFactory();

    m_zsp_factory->init(
        dmgr_f->getDebugMgr(),
        ast_getFactory());
    m_factory->init(
        dmgr_f->getDebugMgr(),
        m_zsp_factory);

    vsc::dm::IContext *vsc_ctxt = vsc_dm_f->mkContext();
    m_arl_dm_factory = zsp_arl_dm_getFactory();
    m_arl_dm_factory->init(dmgr_f->getDebugMgr());
    m_ctxt = zsp::arl::dm::IContextUP(m_arl_dm_factory->mkContext(vsc_dm_f->mkContext()));

    arl::dm::IFactory *arl_dm_factory = zsp_arl_dm_getFactory();

    fprintf(stdout, "TestBase::SetUp m_factory=%p m_zsp_factory=%p\n",
        m_factory, m_zsp_factory);

}

void TestBase::TearDown() {
    m_ctxt.reset();

}

ast::IGlobalScope *TestBase::parse(
        zsp::parser::IMarkerListener        *marker_l,
        const std::string                   &content,
        const std::string                   &name) {
	std::stringstream s(content);

	zsp::ast::IGlobalScopeUP global(m_zsp_factory->getAstFactory()->mkGlobalScope(0));

	zsp::parser::IAstBuilderUP ast_builder(m_zsp_factory->mkAstBuilder(marker_l));

	ast_builder->build(global.get(), &s);

	return global.release();
}

ast::ISymbolScope *TestBase::link(
        zsp::parser::IMarkerListener            *marker_l,
        const std::vector<ast::IGlobalScopeUP>  &files) {
	std::vector<zsp::ast::IGlobalScope *> files_p;

	for (std::vector<zsp::ast::IGlobalScopeUP>::const_iterator
		it=files.begin();
		it!=files.end(); it++) {
		files_p.push_back(it->get());
	}

	zsp::parser::ILinkerUP linker(m_zsp_factory->mkAstLinker());
	zsp::ast::ISymbolScopeUP root(linker->link(
		marker_l,
		files_p
	));

	return root.release();
}

void TestBase::ast2Arl(
        zsp::parser::IMarkerListener            *marker_l,
        ast::ISymbolScope                       *root,
        arl::dm::IContext                       *ctxt) {
    IAst2ArlBuilderUP builder(m_factory->mkAst2ArlBuilder());

    builder->build(
        marker_l,
        root,
        ctxt
    );
}

void TestBase::dumpJSON(const std::vector<vsc::dm::IAccept *> &elems) {
    fprintf(stdout, "JSON:\n%s\n", toJsonStr(elems).c_str());

}

std::string TestBase::toJsonStr(const std::vector<vsc::dm::IAccept *> &elems) {
    std::stringstream s;
    arl::dm::ITypeModelDumperUP dumper(m_arl_dm_factory->mkTypeModelDumperJSON(&s, 4));
    dumper->dumpTypeModel(elems);
    s.flush();

    return s.str();
}

}
}
}
