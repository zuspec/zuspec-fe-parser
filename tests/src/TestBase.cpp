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
#include "arl/IArl.h"
#include "vsc/IVsc.h"
#include "TestBase.h"


namespace zsp {
namespace fe {
namespace parser {


TestBase::TestBase() {

}

TestBase::~TestBase() {

}

extern "C" zsp::IFactory *zsp_parser_getFactory();
extern "C" zsp::ast::IFactory *zsp_ast_getFactory();
extern "C" zsp::fe::parser::IFactory *zsp_fe_parser_getFactory();
extern "C" arl::IArl *iarl();
extern "C" vsc::IVsc *ivsc();

void TestBase::SetUp() {
    m_factory = zsp_fe_parser_getFactory();
    m_zsp_factory = zsp_parser_getFactory();

    m_zsp_factory->init(zsp_ast_getFactory());
    m_factory->init(m_zsp_factory);

    vsc::IContext *vsc_ctxt = ivsc()->mkContext();
    iarl()->init(vsc_ctxt->getDebugMgr());
    m_ctxt = arl::IContextUP(iarl()->mkContext(vsc_ctxt));

    fprintf(stdout, "TestBase::SetUp m_factory=%p m_zsp_factory=%p\n",
        m_factory, m_zsp_factory);

}

void TestBase::TearDown() {
    m_ctxt.reset();

}

ast::IGlobalScope *TestBase::parse(
        IMarkerListener         *marker_l,
        const std::string       &content,
        const std::string       &name) {
	std::stringstream s(content);

	zsp::ast::IGlobalScopeUP global(m_zsp_factory->getAstFactory()->mkGlobalScope(0));

	zsp::IAstBuilderUP ast_builder(m_zsp_factory->mkAstBuilder(marker_l));

	ast_builder->build(global.get(), &s);

	return global.release();
}

ast::ISymbolScope *TestBase::link(
        IMarkerListener                         *marker_l,
        const std::vector<ast::IGlobalScopeUP>  &files) {
	std::vector<zsp::ast::IGlobalScope *> files_p;

	for (std::vector<zsp::ast::IGlobalScopeUP>::const_iterator
		it=files.begin();
		it!=files.end(); it++) {
		files_p.push_back(it->get());
	}

	zsp::ILinkerUP linker(m_zsp_factory->mkAstLinker());
	zsp::ast::ISymbolScopeUP root(linker->link(
		marker_l,
		files_p
	));

	return root.release();
}

void TestBase::ast2Arl(
        IMarkerListener                         *marker_l,
        ast::ISymbolScope                       *root,
        arl::IContext                           *ctxt) {
    IAst2ArlBuilderUP builder(m_factory->mkAst2ArlBuilder());

    builder->build(
        marker_l,
        root,
        ctxt
    );
}

}
}
}
