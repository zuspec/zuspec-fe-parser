#
# core.pyx
#
# Copyright 2022 Matthew Ballance and Contributors
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may 
# not use this file except in compliance with the License.  
# You may obtain a copy of the License at:
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
# See the License for the specific language governing permissions and 
# limitations under the License.
#
# Created on:
#     Author: 
#
import ctypes
import os
import sys
from ctypes import CDLL
cimport zsp_arl_dm.core as arl_dm
from zsp_fe_parser cimport decl
cimport zsp_parser.core as zssp
from debug_mgr cimport core as dm
from zsp_parser cimport ast as ast
from zsp_parser cimport core as zspp
from libc.stdint cimport intptr_t
from libcpp cimport bool

cdef Factory _inst = None
cdef class Factory:

#    void init(dm.IDebugMgr *dmgr, zspp.IFactory *factory)

    cpdef dm.DebugMgr getDebugMgr(self):
        return dm.DebugMgr.mk(self._hndl.getDebugMgr(), False)

    cpdef Ast2ArlBuilder mkAst2ArlBuilder(self):
        return Ast2ArlBuilder.mk(self._hndl.mkAst2ArlBuilder(), True)

    cpdef Ast2ArlContext mkAst2ArlContext(
        self,
        arl_dm.Context              ctxt,
        zspp.MarkerListener         marker_l):
        return Ast2ArlContext.mk(self._hndl.mkAst2ArlContext(
            ctxt.asContext(),
            marker_l._hndl), True)

    @staticmethod
    def inst():
        cdef Factory factory
        cdef dm.DebugMgr dmgr
        cdef zspp.Factory zspp_factory
        global _inst

        if _inst is None:
            ext_dir = os.path.dirname(os.path.abspath(__file__))

            core_lib = os.path.join(ext_dir, "libzsp-fe-parser.so")
            if not os.path.isfile(core_lib):
                raise Exception("Extension library core \"%s\" doesn't exist" % core_lib)

            so = ctypes.cdll.LoadLibrary(core_lib)

            func = so.zsp_fe_parser_getFactory
            func.restype = ctypes.c_void_p

            hndl = <decl.IFactoryP>(<intptr_t>(func()))
            factory = Factory()
            factory._hndl = hndl

            dmgr = dm.Factory.inst().getDebugMgr()
            zspp_factory = zspp.Factory.inst()
            factory._hndl.init(
                dmgr._hndl,
                zspp_factory._hndl)

            _inst = factory
        return _inst

cdef class Ast2ArlBuilder:

    cpdef void build(self,
        ast.SymbolScope root,
        Ast2ArlContext  ctxt):
        self._hndl.build(root.asSymbolScope(), ctxt._hndl)

    @staticmethod
    cdef Ast2ArlBuilder mk(decl.IAst2ArlBuilder *hndl, bool owned=True):
        ret = Ast2ArlBuilder()
        ret._hndl = hndl
        ret._owned = owned
        return ret

cdef class Ast2ArlContext:

    @staticmethod
    cdef Ast2ArlContext mk(decl.IAst2ArlContext *hndl, bool owned=True):
        ret = Ast2ArlContext()
        ret._hndl = hndl
        ret._owned = owned
        return ret
