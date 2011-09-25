
/* $Id: d_mos.model,v 25.95 2006/08/26 01:23:57 al Exp $ -*- C++ -*-
 * Copyright (C) 2001 Albert Davis
 * Author: Albert Davis <aldavis@ieee.org>
 *
 * This file is part of "Gnucap", the Gnu Circuit Analysis Package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *------------------------------------------------------------------
 * data structures and defaults for mos model.
 * internal units all mks (meters)
 * but some user input parameters are in cm.
 *
 * netlist syntax:
 * device:  mxxxx d g s b mname <device args> <model card args>
 * model:   .model mname NMOS <args>
 *	or  .model mname PMOS <args>
 */
/* This file is automatically generated. DO NOT EDIT */

#include "u_limit.h"
#include "d_cap.h"
#include "d_admit.h"
#include "d_res.h"
#include "d_mos_base.h"
#include "ap.h"
#include "d_mos.h"
/*--------------------------------------------------------------------------*/
const double NA(NOT_INPUT);
const double INF(BIGBIG);
/*--------------------------------------------------------------------------*/
int DEV_MOS::_count = 0;
int COMMON_MOS::_count = -1;
static COMMON_MOS Default_MOS(CC_STATIC);
/*--------------------------------------------------------------------------*/
COMMON_MOS::COMMON_MOS(int c)
  :COMMON_COMPONENT(c),
   m(1.0),
   l_in(NA),
   w_in(NA),
   ad_in(NA),
   as_in(NA),
   pd(0.0),
   ps(0.0),
   nrd(1.0),
   nrs(1.0),
   _sdp(0),
   _db(0),
   _sb(0)
{
  ++_count;
}
/*--------------------------------------------------------------------------*/
COMMON_MOS::COMMON_MOS(const COMMON_MOS& p)
  :COMMON_COMPONENT(p),
   m(p.m),
   l_in(p.l_in),
   w_in(p.w_in),
   ad_in(p.ad_in),
   as_in(p.as_in),
   pd(p.pd),
   ps(p.ps),
   nrd(p.nrd),
   nrs(p.nrs),
   _sdp(0),
   _db(0),
   _sb(0)
{
  ++_count;
}
/*--------------------------------------------------------------------------*/
COMMON_MOS::~COMMON_MOS()
{
  detach_common(&_db);
  detach_common(&_sb);
  --_count;
  delete _sdp;
}
/*--------------------------------------------------------------------------*/
bool COMMON_MOS::operator==(const COMMON_COMPONENT& x)const
{
  const COMMON_MOS* p = dynamic_cast<const COMMON_MOS*>(&x);
  return (p
    && m == p->m
    && l_in == p->l_in
    && w_in == p->w_in
    && ad_in == p->ad_in
    && as_in == p->as_in
    && pd == p->pd
    && ps == p->ps
    && nrd == p->nrd
    && nrs == p->nrs
    && _sdp == p->_sdp
    && COMMON_COMPONENT::operator==(x));
}
/*--------------------------------------------------------------------------*/
void COMMON_MOS::parse(CS& cmd)
{
  parse_modelname(cmd);
  int here = cmd.cursor();
  do{
    ONE_OF
    || get(cmd, "M", &m)
    || get(cmd, "L", &l_in)
    || get(cmd, "W", &w_in)
    || get(cmd, "AD", &ad_in)
    || get(cmd, "AS", &as_in)
    || get(cmd, "PD", &pd)
    || get(cmd, "PS", &ps)
    || get(cmd, "NRD", &nrd)
    || get(cmd, "NRS", &nrs)
    ;
  }while (cmd.more() && !cmd.stuck(&here));
  cmd.check(bWARNING, "what's this?");
}
/*--------------------------------------------------------------------------*/
void COMMON_MOS::print(OMSTREAM& o)const
{
  o << "  " << modelname();
  o.setfloatwidth(7);
  if (m != 1.)
    o << "  m=" << m;
  o << "  l=" << l_in;
  o << "  w=" << w_in;
  if (ad_in != NA)
    o << "  ad=" << ad_in;
  if (as_in != NA)
    o << "  as=" << as_in;
  if (pd != 0.)
    o << "  pd=" << pd;
  if (ps != 0.)
    o << "  ps=" << ps;
  if (nrd != 1.)
    o << "  nrd=" << nrd;
  if (nrs != 1.)
    o << "  nrs=" << nrs;
  o << '\n';
}
/*--------------------------------------------------------------------------*/
void COMMON_MOS::elabo3(const COMPONENT* d)
{
  COMMON_MOS* c = this;
  const MODEL_MOS_BASE* m = dynamic_cast<const MODEL_MOS_BASE*>(attach_model(d));
  if (!m) {
    error(bERROR, d->long_label() + ": model " + modelname()
          + " is not a mosfet\n");
  }

  assert(c);
  const CARD_LIST* par_scope = d->scope();
  assert(par_scope);
    // final adjust: code_pre
    // final adjust: override
    // final adjust: raw
    this->m.e_val(1.0, par_scope);
    this->l_in.e_val(NA, par_scope);
    this->w_in.e_val(NA, par_scope);
    this->ad_in.e_val(NA, par_scope);
    this->as_in.e_val(NA, par_scope);
    this->pd.e_val(0.0, par_scope);
    this->ps.e_val(0.0, par_scope);
    this->nrd.e_val(1.0, par_scope);
    this->nrs.e_val(1.0, par_scope);
    // final adjust: mid
    // final adjust: calculated
    // final adjust: post
    // final adjust: done

  // size dependent
  delete _sdp;
  _sdp = m->new_sdp(this);
  assert(_sdp);
  const SDP_MOS_BASE* s = dynamic_cast<const SDP_MOS_BASE*>(_sdp);
  assert(s);

  // subcircuit commons, recursive
  COMMON_DIODE* db = new COMMON_DIODE;
  db->m = c->m;
  db->area = s->ad;
  db->perim = c->pd;
  db->is_raw = s->idsat;
  db->cj_raw = m->cbd;
  db->cjsw_raw = NA;
  db->off = true;
  db->set_modelname(modelname());
  db->attach(model());
  attach_common(db, &_db);

  COMMON_DIODE* sb = new COMMON_DIODE;
  sb->m = c->m;
  sb->area = s->as;
  sb->perim = c->ps;
  sb->is_raw = s->issat;
  sb->cj_raw = m->cbs;
  sb->cjsw_raw = NA;
  sb->off = true;
  sb->set_modelname(modelname());
  sb->attach(model());
  attach_common(sb, &_sb);

}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
static EVAL_MOS_Cgb Eval_Cgb(CC_STATIC);
void EVAL_MOS_Cgb::tr_eval(ELEMENT* d)const
{
  assert(d);
  DEV_MOS* p = prechecked_cast<DEV_MOS*>(d->owner());
  assert(p);
  const COMMON_MOS* c = prechecked_cast<const COMMON_MOS*>(p->common());
  assert(c);
  const SDP_MOS_BASE* s = prechecked_cast<const SDP_MOS_BASE*>(c->sdp());
  assert(s);
  const MODEL_MOS_BASE* m = prechecked_cast<const MODEL_MOS_BASE*>(c->model());
  assert(m);

    DEV_CAPACITANCE* brh = prechecked_cast<DEV_CAPACITANCE*>(d);
    assert(brh);

    double cap = brh->value();

    if (m->cmodel != 0) {
      if (p->vgst < - s->phi) { 		/* accumulation */
	cap += s->cgate;
      }else if (p->vgst < 0.) {			/* depletion */
	cap += s->cgate * (-p->vgst) / s->phi;
      }else{					/* active, overlap only */
      }
    }
    brh->_y0.f1 = cap;
    if (SIM::phase == SIM::pTRAN) {
      cap = (brh->_y0.f1 + brh->_q[1].f1) / 2;
      brh->_y0.f0 = (brh->_y0.x - brh->_q[1].x) * cap + brh->_q[1].f0;
    }else{
      brh->_y0.f0 = brh->_y0.x * brh->_y0.f1;
    }
    brh->_y0 *= c->m;
    trace3(brh->long_label().c_str(), brh->_y0.x, brh->_y0.f0, brh->_y0.f1);
}
/*--------------------------------------------------------------------------*/
static EVAL_MOS_Cgd Eval_Cgd(CC_STATIC);
void EVAL_MOS_Cgd::tr_eval(ELEMENT* d)const
{
  assert(d);
  DEV_MOS* p = prechecked_cast<DEV_MOS*>(d->owner());
  assert(p);
  const COMMON_MOS* c = prechecked_cast<const COMMON_MOS*>(p->common());
  assert(c);
  const SDP_MOS_BASE* s = prechecked_cast<const SDP_MOS_BASE*>(c->sdp());
  assert(s);
  const MODEL_MOS_BASE* m = prechecked_cast<const MODEL_MOS_BASE*>(c->model());
  assert(m);

    DEV_CAPACITANCE* brh = prechecked_cast<DEV_CAPACITANCE*>(d);
    assert(brh);

    double cap = 0;
    if (m->cmodel != 0) {
      assert(p->vdsat >= 0.);
      assert(p->vds >= 0.);
      double vbs    = (m->cmodel == 3) ? 0. : p->vbs;
      double vdbsat = p->vdsat - vbs;
      double vdb    = p->vds   - vbs;
      double ddif   = 2. * vdbsat - vdb;
      
      if (!p->reversed) { // treat as Cgs
	if (p->vgst >= 0.) {
	  if (p->vdsat > p->vds) {		/* linear */
	    cap = (2./3.) * s->cgate * (1. - (vdbsat*vdbsat)/(ddif*ddif));
	    if (p->vgst <= .1) {
	      cap *= 10. * p->vgst;	// smooth discontinuity
	    }
	  }
	}
      }else{ // treat as Cgs
	if (p->vgst >= -s->phi/2.) {		/* depletion  or active */
	  cap = (2./3.) * s->cgate;
	  if (p->vdsat > p->vds) {			/* linear */
	    double ndif   = p->vdsat - p->vds;
	    cap *= 1. - (ndif*ndif)/(ddif*ddif);
	  }
	  if (p->vgst <= 0) {
	    cap *= 1. + p->vgst / (s->phi);
	    cap *= 1. + p->vgst / (s->phi);
	  }
	}
      }
    }
    cap += brh->value();		/* else overlap only */
    
    brh->_y0.f1 = cap;
    if (SIM::phase == SIM::pTRAN) {
      cap = (brh->_y0.f1 + brh->_q[1].f1) / 2;
      brh->_y0.f0 = (brh->_y0.x - brh->_q[1].x) * cap + brh->_q[1].f0;
    }else{
      brh->_y0.f0 = brh->_y0.x * brh->_y0.f1;
    }
    brh->_y0 *= c->m;
    trace3(brh->long_label().c_str(), brh->_y0.x, brh->_y0.f0, brh->_y0.f1);
}
/*--------------------------------------------------------------------------*/
static EVAL_MOS_Cgs Eval_Cgs(CC_STATIC);
void EVAL_MOS_Cgs::tr_eval(ELEMENT* d)const
{
  assert(d);
  DEV_MOS* p = prechecked_cast<DEV_MOS*>(d->owner());
  assert(p);
  const COMMON_MOS* c = prechecked_cast<const COMMON_MOS*>(p->common());
  assert(c);
  const SDP_MOS_BASE* s = prechecked_cast<const SDP_MOS_BASE*>(c->sdp());
  assert(s);
  const MODEL_MOS_BASE* m = prechecked_cast<const MODEL_MOS_BASE*>(c->model());
  assert(m);

    DEV_CAPACITANCE* brh = prechecked_cast<DEV_CAPACITANCE*>(d);
    assert(brh);

    double cap = 0;
    if (m->cmodel != 0) {
      assert(p->vdsat >= 0.);
      assert(p->vds >= 0.);
      double vbs    = (m->cmodel == 3) ? 0. : p->vbs;
      double vdbsat = p->vdsat - vbs;
      double vdb    = p->vds   - vbs;
      double ddif   = 2. * vdbsat - vdb;
      
      if (p->reversed) { // treat as Cgd
	if (p->vgst >= 0.) {
	  if (p->vdsat > p->vds) {		/* linear */
	    cap = (2./3.) * s->cgate * (1. - (vdbsat*vdbsat)/(ddif*ddif));
	    if (p->vgst <= .1) {
	      cap *= 10. * p->vgst;	// smooth discontinuity
	    }
	  }
	}
      }else{ // treat as Cgs
	if (p->vgst >= -s->phi/2.) {		/* depletion  or active */
	  cap = (2./3.) * s->cgate;
	  if (p->vdsat > p->vds) {			/* linear */
	    double ndif   = p->vdsat - p->vds;
	    cap *= 1. - (ndif*ndif)/(ddif*ddif);
	  }
	  if (p->vgst <= 0) {
	    cap *= 1. + p->vgst / (s->phi);
	    cap *= 1. + p->vgst / (s->phi);
	  }
	}
      }
    }
    cap += brh->value();		/* else overlap only */
    
    brh->_y0.f1 = cap;
    if (SIM::phase == SIM::pTRAN) {
      cap = (brh->_y0.f1 + brh->_q[1].f1) / 2;
      brh->_y0.f0 = (brh->_y0.x - brh->_q[1].x) * cap + brh->_q[1].f0;
    }else{
      brh->_y0.f0 = brh->_y0.x * brh->_y0.f1;
    }
    brh->_y0 *= c->m;
    trace3(brh->long_label().c_str(), brh->_y0.x, brh->_y0.f0, brh->_y0.f1);
}
/*--------------------------------------------------------------------------*/
DEV_MOS::DEV_MOS()
  :BASE_SUBCKT(),
   // input parameters,
   // calculated parameters,
   ids(0.),
   idsxxx(NA),
   gds(0.),
   gmf(0.),
   gmr(0.),
   gmbf(0.),
   gmbr(0.),
   idb(0.),
   idbxxx(0.),
   gdbdb(0.),
   gdbds(0.),
   gdbgs(0.),
   gdbbs(0.),
   isb(0.),
   isbxxx(0.),
   gsbsb(0.),
   gsbsd(0.),
   gsbgd(0.),
   gsbbd(0.),
   qgate(0.),
   cgs(0.),
   cggb(0.),
   cgsb(0.),
   cgdb(0.),
   qgs(0.),
   cgsgs(0.),
   cgsgb(0.),
   cgssb(0.),
   cgsdb(0.),
   qgd(0.),
   cgdgd(0.),
   cgdgb(0.),
   cgdsb(0.),
   cgddb(0.),
   qdrn(0.),
   cdsds(0.),
   cdgb(0.),
   cdsb(0.),
   cddb(0.),
   qbulk(0.),
   cbs(0.),
   cbgb(0.),
   cbsb(0.),
   cbdb(0.),
   qbs(0.),
   cbsbs(0.),
   cbsgb(0.),
   cbssb(0.),
   cbsdb(0.),
   qbd(0.),
   cbdbd(0.),
   cbdgb(0.),
   cbdsb(0.),
   cbddb(0.),
   gtau(0.),
   cqgb(0.),
   cqsb(0.),
   cqdb(0.),
   cqbb(0.),
   vgs(0.),
   vds(0.),
   vbs(0.),
   vdsat(0.),
   vgst(0.),
   von(0.),
   reversed(false),
   cutoff(false),
   subthreshold(false),
   saturated(false),
   sbfwd(false),
   punchthru(false),
   // netlist,
   _Rs(0),
   _Rd(0),
   _Ddb(0),
   _Dsb(0),
   _Cgs(0),
   _Cgd(0),
   _Cgb(0),
   _Ids(0),
   _Idb(0),
   _Isb(0)
{
  _n = _nodes + int_nodes();
  attach_common(&Default_MOS);
  ++_count;
  // overrides
}
/*--------------------------------------------------------------------------*/
DEV_MOS::DEV_MOS(const DEV_MOS& p)
  :BASE_SUBCKT(p),
   // input parameters,
   // calculated parameters,
   ids(p.ids),
   idsxxx(p.idsxxx),
   gds(p.gds),
   gmf(p.gmf),
   gmr(p.gmr),
   gmbf(p.gmbf),
   gmbr(p.gmbr),
   idb(p.idb),
   idbxxx(p.idbxxx),
   gdbdb(p.gdbdb),
   gdbds(p.gdbds),
   gdbgs(p.gdbgs),
   gdbbs(p.gdbbs),
   isb(p.isb),
   isbxxx(p.isbxxx),
   gsbsb(p.gsbsb),
   gsbsd(p.gsbsd),
   gsbgd(p.gsbgd),
   gsbbd(p.gsbbd),
   qgate(p.qgate),
   cgs(p.cgs),
   cggb(p.cggb),
   cgsb(p.cgsb),
   cgdb(p.cgdb),
   qgs(p.qgs),
   cgsgs(p.cgsgs),
   cgsgb(p.cgsgb),
   cgssb(p.cgssb),
   cgsdb(p.cgsdb),
   qgd(p.qgd),
   cgdgd(p.cgdgd),
   cgdgb(p.cgdgb),
   cgdsb(p.cgdsb),
   cgddb(p.cgddb),
   qdrn(p.qdrn),
   cdsds(p.cdsds),
   cdgb(p.cdgb),
   cdsb(p.cdsb),
   cddb(p.cddb),
   qbulk(p.qbulk),
   cbs(p.cbs),
   cbgb(p.cbgb),
   cbsb(p.cbsb),
   cbdb(p.cbdb),
   qbs(p.qbs),
   cbsbs(p.cbsbs),
   cbsgb(p.cbsgb),
   cbssb(p.cbssb),
   cbsdb(p.cbsdb),
   qbd(p.qbd),
   cbdbd(p.cbdbd),
   cbdgb(p.cbdgb),
   cbdsb(p.cbdsb),
   cbddb(p.cbddb),
   gtau(p.gtau),
   cqgb(p.cqgb),
   cqsb(p.cqsb),
   cqdb(p.cqdb),
   cqbb(p.cqbb),
   vgs(p.vgs),
   vds(p.vds),
   vbs(p.vbs),
   vdsat(p.vdsat),
   vgst(p.vgst),
   von(p.von),
   reversed(p.reversed),
   cutoff(p.cutoff),
   subthreshold(p.subthreshold),
   saturated(p.saturated),
   sbfwd(p.sbfwd),
   punchthru(p.punchthru),
   // netlist,
   _Rs(0),
   _Rd(0),
   _Ddb(0),
   _Dsb(0),
   _Cgs(0),
   _Cgd(0),
   _Cgb(0),
   _Ids(0),
   _Idb(0),
   _Isb(0)
{
  _n = _nodes + int_nodes();
  for (int ii = -int_nodes(); ii < max_nodes(); ++ii) {
    _n[ii] = p._n[ii];
  }
  ++_count;
  // overrides
}
/*--------------------------------------------------------------------------*/
void DEV_MOS::elabo1()
{
  BASE_SUBCKT::elabo1();
  const COMMON_MOS* c = prechecked_cast<const COMMON_MOS*>(common());
  assert(c);
  const MODEL_MOS_BASE* m = prechecked_cast<const MODEL_MOS_BASE*>(c->model());
  assert(m);
  const SDP_MOS_BASE* s = prechecked_cast<const SDP_MOS_BASE*>(c->sdp());
  assert(s);
  if (!subckt()) {
    new_subckt();
  }

  // local nodes
  {if (!OPT::rstray || s->rd == 0.) {
    _n[n_idrain] = _n[n_drain];
  }else{
    _n[n_idrain].new_model_node();
  }}
  {if (!OPT::rstray || s->rs == 0.) {
    _n[n_isource] = _n[n_source];
  }else{
    _n[n_isource].new_model_node();
  }}

  // clone subckt elements
  {if (!OPT::rstray || s->rs == 0.) {
    if (_Rs) {
      untested();
      subckt()->erase(_Rs);
      _Rs = NULL;
    }
  }else{
    if (!_Rs) {
      _Rs = new DEV_RESISTANCE;
      subckt()->push_front(_Rs);
    }
    {{
      node_t nodes[] = {_n[n_source], _n[n_isource]};
      _Rs->set_parameters("Rs", this, NULL, s->rs/c->m, 0, 0, 2, nodes);
    }}
  }}
  {if (!OPT::rstray || s->rd == 0.) {
    if (_Rd) {
      untested();
      subckt()->erase(_Rd);
      _Rd = NULL;
    }
  }else{
    if (!_Rd) {
      _Rd = new DEV_RESISTANCE;
      subckt()->push_front(_Rd);
    }
    {{
      node_t nodes[] = {_n[n_drain], _n[n_idrain]};
      _Rd->set_parameters("Rd", this, NULL, s->rd/c->m, 0, 0, 2, nodes);
    }}
  }}
  {if (_n[n_bulk].n_() == _n[n_drain].n_() || s->idsat == 0.) {
    if (_Ddb) {
      untested();
      subckt()->erase(_Ddb);
      _Ddb = NULL;
    }
  }else{
    if (!_Ddb) {
      _Ddb = new DEV_DIODE;
      subckt()->push_front(_Ddb);
    }
    {if (m->polarity==pP) {
      node_t nodes[] = {_n[n_idrain], _n[n_bulk]};
      _Ddb->set_parameters("Ddb", this, c->_db, 0., 0, 0, 2, nodes);
    }else{
      node_t nodes[] = {_n[n_bulk], _n[n_idrain]};
      _Ddb->set_parameters("Ddb", this, c->_db, 0., 0, 0, 2, nodes);
    }}
  }}
  {if (_n[n_bulk].n_() == _n[n_source].n_() || s->issat == 0.) {
    if (_Dsb) {
      untested();
      subckt()->erase(_Dsb);
      _Dsb = NULL;
    }
  }else{
    if (!_Dsb) {
      _Dsb = new DEV_DIODE;
      subckt()->push_front(_Dsb);
    }
    {if (m->polarity==pP) {
      node_t nodes[] = {_n[n_isource], _n[n_bulk]};
      _Dsb->set_parameters("Dsb", this, c->_sb, 0., 0, 0, 2, nodes);
    }else{
      node_t nodes[] = {_n[n_bulk], _n[n_isource]};
      _Dsb->set_parameters("Dsb", this, c->_sb, 0., 0, 0, 2, nodes);
    }}
  }}
  {if (!OPT::cstray || _n[n_gate].n_() == _n[n_source].n_()) {
    if (_Cgs) {
      untested();
      subckt()->erase(_Cgs);
      _Cgs = NULL;
    }
  }else{
    if (!_Cgs) {
      _Cgs = new DEV_CAPACITANCE;
      subckt()->push_front(_Cgs);
    }
    {{
      node_t nodes[] = {_n[n_gate], _n[n_isource]};
      _Cgs->set_parameters("Cgs", this, &Eval_Cgs, s->cgso, 0, 0, 2, nodes);
    }}
  }}
  {if (!OPT::cstray || _n[n_gate].n_() == _n[n_drain].n_()) {
    if (_Cgd) {
      untested();
      subckt()->erase(_Cgd);
      _Cgd = NULL;
    }
  }else{
    if (!_Cgd) {
      _Cgd = new DEV_CAPACITANCE;
      subckt()->push_front(_Cgd);
    }
    {{
      node_t nodes[] = {_n[n_gate], _n[n_idrain]};
      _Cgd->set_parameters("Cgd", this, &Eval_Cgd, s->cgdo, 0, 0, 2, nodes);
    }}
  }}
  {if (!OPT::cstray || _n[n_bulk].n_() == _n[n_gate].n_()) {
    if (_Cgb) {
      untested();
      subckt()->erase(_Cgb);
      _Cgb = NULL;
    }
  }else{
    if (!_Cgb) {
      _Cgb = new DEV_CAPACITANCE;
      subckt()->push_front(_Cgb);
    }
    {{
      node_t nodes[] = {_n[n_gate], _n[n_bulk]};
      _Cgb->set_parameters("Cgb", this, &Eval_Cgb, s->cgbo, 0, 0, 2, nodes);
    }}
  }}
  {{
    if (!_Ids) {
      _Ids = new DEV_CPOLY_G;
      subckt()->push_front(_Ids);
    }
    {{
      node_t nodes[] = {_n[n_idrain], _n[n_isource], _n[n_gate], _n[n_isource], _n[n_idrain], _n[n_gate], _n[n_bulk], _n[n_isource], _n[n_idrain], _n[n_bulk]};
      _Ids->set_parameters("Ids", this, NULL, 0., 6, &idsxxx, 10, nodes);
    }}
  }}
  {if (!(m->needs_isub) || _n[n_drain].n_() == _n[n_bulk].n_()) {
    if (_Idb) {
      untested();
      subckt()->erase(_Idb);
      _Idb = NULL;
    }
  }else{
    if (!_Idb) {
      _Idb = new DEV_CPOLY_G;
      subckt()->push_front(_Idb);
    }
    {{
      node_t nodes[] = {_n[n_idrain], _n[n_bulk], _n[n_idrain], _n[n_isource], _n[n_gate], _n[n_isource], _n[n_bulk], _n[n_isource]};
      _Idb->set_parameters("Idb", this, NULL, 0., 5, &idbxxx, 8, nodes);
    }}
  }}
  {if (!(m->needs_isub) || _n[n_source].n_() == _n[n_bulk].n_()) {
    if (_Isb) {
      untested();
      subckt()->erase(_Isb);
      _Isb = NULL;
    }
  }else{
    if (!_Isb) {
      _Isb = new DEV_CPOLY_G;
      subckt()->push_front(_Isb);
    }
    {{
      node_t nodes[] = {_n[n_isource], _n[n_bulk], _n[n_isource], _n[n_idrain], _n[n_gate], _n[n_idrain], _n[n_bulk], _n[n_idrain]};
      _Isb->set_parameters("Isb", this, NULL, 0., 5, &isbxxx, 8, nodes);
    }}
  }}
  subckt()->elabo2();
  assert(!is_constant());
  subckt()->set_slave();
}
/*--------------------------------------------------------------------------*/
double DEV_MOS::tr_probe_num(CS& cmd)const
{
  const COMMON_MOS* c = prechecked_cast<const COMMON_MOS*>(common());
  assert(c);
  const MODEL_MOS_BASE* m = prechecked_cast<const MODEL_MOS_BASE*>(c->model());
  assert(m);
  const SDP_MOS_BASE* s = prechecked_cast<const SDP_MOS_BASE*>(c->sdp());
  assert(s);

  {if (cmd.pmatch("VDS")) {
    return  _n[n_drain].v0() - _n[n_source].v0();
  }else if (cmd.pmatch("VGS")) {
    return  _n[n_gate].v0() - _n[n_source].v0();
  }else if (cmd.pmatch("VBS")) {
    return  _n[n_bulk].v0() - _n[n_source].v0();
  }else if (cmd.pmatch("VDSInt")) {
    return  vds;
  }else if (cmd.pmatch("VGSInt")) {
    return  vgs;
  }else if (cmd.pmatch("VBSInt")) {
    return  vbs;
  }else if (cmd.pmatch("VGD")) {
    return  _n[n_gate].v0() - _n[n_drain].v0();
  }else if (cmd.pmatch("VBD")) {
    return  _n[n_bulk].v0() - _n[n_drain].v0();
  }else if (cmd.pmatch("VSD")) {
    return  _n[n_source].v0() - _n[n_drain].v0();
  }else if (cmd.pmatch("VDM")) {
    return  ( _n[n_drain].v0() - _n[n_source].v0() + _n[n_drain].v0() - _n[n_drain].v0() ) / 2.;
  }else if (cmd.pmatch("VGM")) {
    return  ( _n[n_gate].v0() - _n[n_source].v0() + _n[n_gate].v0() - _n[n_drain].v0() ) / 2.;
  }else if (cmd.pmatch("VBM")) {
    return  ( _n[n_bulk].v0() - _n[n_source].v0() + _n[n_bulk].v0() - _n[n_drain].v0() ) / 2.;
  }else if (cmd.pmatch("VSM")) {
    return  ( _n[n_source].v0() - _n[n_source].v0() + _n[n_source].v0() - _n[n_drain].v0() ) / 2.;
  }else if (cmd.pmatch("VDG")) {
    return  _n[n_drain].v0() - _n[n_gate].v0();
  }else if (cmd.pmatch("VBG")) {
    return  _n[n_bulk].v0() - _n[n_gate].v0();
  }else if (cmd.pmatch("VSG")) {
    return  _n[n_source].v0() - _n[n_gate].v0();
  }else if (cmd.pmatch("VDB")) {
    return  _n[n_drain].v0() - _n[n_bulk].v0();
  }else if (cmd.pmatch("VGB")) {
    return  _n[n_gate].v0() - _n[n_bulk].v0();
  }else if (cmd.pmatch("VSB")) {
    return  _n[n_source].v0() - _n[n_bulk].v0();
  }else if (cmd.pmatch("VD")) {
    return  _n[n_drain].v0();
  }else if (cmd.pmatch("VG")) {
    return  _n[n_gate].v0();
  }else if (cmd.pmatch("VB")) {
    return  _n[n_bulk].v0();
  }else if (cmd.pmatch("VS")) {
    return  _n[n_source].v0();
  }else if (cmd.pmatch("Id")) {
    return  (_Rd) ? CARD::probe(_Rd,"I") : CARD::probe(_Ids,"I") - CARD::probe(_Cgd,"I") + CARD::probe(_Ddb,"I") * m->polarity;
  }else if (cmd.pmatch("IS")) {
    return  (_Rs) ? CARD::probe(_Rs,"I") : - CARD::probe(_Ids,"I") - CARD::probe(_Cgs,"I") + CARD::probe(_Dsb,"I") * m->polarity;
  }else if (cmd.pmatch("IG")) {
    return  CARD::probe(_Cgs,"I") + CARD::probe(_Cgd,"I") + CARD::probe(_Cgb,"I");
  }else if (cmd.pmatch("IB")) {
    return  - CARD::probe(_Ddb,"I") * m->polarity - CARD::probe(_Dsb,"I") * m->polarity - CARD::probe(_Cgb,"I");
  }else if (cmd.pmatch("IBD")) {
    return  CARD::probe(_Ddb,"I");
  }else if (cmd.pmatch("IBS")) {
    return  CARD::probe(_Dsb,"I");
  }else if (cmd.pmatch("CGSOvl")) {
    return  CARD::probe(_Cgs,"NV");
  }else if (cmd.pmatch("CGDOvl")) {
    return  CARD::probe(_Cgd,"NV");
  }else if (cmd.pmatch("CGBOvl")) {
    return  CARD::probe(_Cgb,"NV");
  }else if (cmd.pmatch("CGST")) {
    return  CARD::probe(_Cgs,"EV");
  }else if (cmd.pmatch("CGDT")) {
    return  CARD::probe(_Cgd,"EV");
  }else if (cmd.pmatch("CGBT")) {
    return  CARD::probe(_Cgb,"EV");
  }else if (cmd.pmatch("CGSm")) {
    return  CARD::probe(_Cgs,"EV") - CARD::probe(_Cgs,"NV");
  }else if (cmd.pmatch("CGDm")) {
    return  CARD::probe(_Cgd,"EV") - CARD::probe(_Cgd,"NV");
  }else if (cmd.pmatch("CGBm")) {
    return  CARD::probe(_Cgb,"EV") - CARD::probe(_Cgb,"NV");
  }else if (cmd.pmatch("CBD")) {
    return  CARD::probe(_Ddb,"Cap");
  }else if (cmd.pmatch("CBS")) {
    return  CARD::probe(_Dsb,"Cap");
  }else if (cmd.pmatch("CGATE")) {
    return  s->cgate;
  }else if (cmd.pmatch("GM")) {
    return  (reversed) ? gmr : gmf;
  }else if (cmd.pmatch("GMBs")) {
    return  (reversed) ? gmbr : gmbf;
  }else if (cmd.pmatch("GBD")) {
    return  CARD::probe(_Ddb,"G");
  }else if (cmd.pmatch("GBS")) {
    return  CARD::probe(_Dsb,"G");
  }else if (cmd.pmatch("VDSAT")) {
    return  vdsat * m->polarity;
  }else if (cmd.pmatch("VTH")) {
    return  von * m->polarity;
  }else if (cmd.pmatch("IDS")) {
    return  m->polarity * ((reversed) ? -ids : ids);
  }else if (cmd.pmatch("IDSTray")) {
    return  - CARD::probe(_Cgd,"I") + CARD::probe(_Ddb,"I") * m->polarity;
  }else if (cmd.pmatch("P")) {
    return  CARD::probe(_Rs,"P") + CARD::probe(_Rd,"P") + CARD::probe(_Ddb,"P") + CARD::probe(_Dsb,"P") + CARD::probe(_Cgs,"P") + CARD::probe(_Cgd,"P") + CARD::probe(_Cgb,"P") + CARD::probe(_Ids,"P");
  }else if (cmd.pmatch("PD")) {
    return  CARD::probe(_Rs,"PD") + CARD::probe(_Rd,"PD") + CARD::probe(_Ddb,"PD") + CARD::probe(_Dsb,"PD") + CARD::probe(_Cgs,"PD") + CARD::probe(_Cgd,"PD") + CARD::probe(_Cgb,"PD") + CARD::probe(_Ids,"PD");
  }else if (cmd.pmatch("PS")) {
    return  CARD::probe(_Rs,"PS") + CARD::probe(_Rd,"PS") + CARD::probe(_Ddb,"PS") + CARD::probe(_Dsb,"PS") + CARD::probe(_Cgs,"PS") + CARD::probe(_Cgd,"PS") + CARD::probe(_Cgb,"PS") + CARD::probe(_Ids,"PS");
  }else if (cmd.pmatch("REgion")) {
    return  static_cast<double>((!cutoff) + (!subthreshold * 2) + (saturated * 4) + (sbfwd * 10) + ((vbs > vds) * 20) + (punchthru * 40)) * ((reversed)? -1 : 1);
  }else if (cmd.pmatch("SUBthreshold")) {
    return  static_cast<double>(subthreshold);
  }else if (cmd.pmatch("CUToff")) {
    return  static_cast<double>(cutoff);
  }else if (cmd.pmatch("SATurated")) {
    return  static_cast<double>(saturated);
  }else if (cmd.pmatch("TRIode")) {
    return  static_cast<double>(!saturated && !subthreshold);
  }else if (cmd.pmatch("SBFwd")) {
    return  static_cast<double>(sbfwd);
  }else if (cmd.pmatch("DBFwd")) {
    return  static_cast<double>(vbs > vds);
  }else if (cmd.pmatch("REVersed")) {
    return  static_cast<double>(reversed);
  }else if (cmd.pmatch("Status")) {
    return  static_cast<double>(converged() * 2);
  }else if (cmd.pmatch("IDS")) {
    return ids;
  }else if (cmd.pmatch("IDSXXX")) {
    return idsxxx;
  }else if (cmd.pmatch("GDS")) {
    return gds;
  }else if (cmd.pmatch("GMF")) {
    return gmf;
  }else if (cmd.pmatch("GMR")) {
    return gmr;
  }else if (cmd.pmatch("GMBF")) {
    return gmbf;
  }else if (cmd.pmatch("GMBR")) {
    return gmbr;
  }else if (cmd.pmatch("IDB")) {
    return idb;
  }else if (cmd.pmatch("IDBXXX")) {
    return idbxxx;
  }else if (cmd.pmatch("GDBDB")) {
    return gdbdb;
  }else if (cmd.pmatch("GDBDS")) {
    return gdbds;
  }else if (cmd.pmatch("GDBGS")) {
    return gdbgs;
  }else if (cmd.pmatch("GDBBS")) {
    return gdbbs;
  }else if (cmd.pmatch("ISB")) {
    return isb;
  }else if (cmd.pmatch("ISBXXX")) {
    return isbxxx;
  }else if (cmd.pmatch("GSBSB")) {
    return gsbsb;
  }else if (cmd.pmatch("GSBSD")) {
    return gsbsd;
  }else if (cmd.pmatch("GSBGD")) {
    return gsbgd;
  }else if (cmd.pmatch("GSBBD")) {
    return gsbbd;
  }else if (cmd.pmatch("QGATE")) {
    return qgate;
  }else if (cmd.pmatch("CGS")) {
    return cgs;
  }else if (cmd.pmatch("CGGB")) {
    return cggb;
  }else if (cmd.pmatch("CGSB")) {
    return cgsb;
  }else if (cmd.pmatch("CGDB")) {
    return cgdb;
  }else if (cmd.pmatch("QGS")) {
    return qgs;
  }else if (cmd.pmatch("CGSGS")) {
    return cgsgs;
  }else if (cmd.pmatch("CGSGB")) {
    return cgsgb;
  }else if (cmd.pmatch("CGSSB")) {
    return cgssb;
  }else if (cmd.pmatch("CGSDB")) {
    return cgsdb;
  }else if (cmd.pmatch("QGD")) {
    return qgd;
  }else if (cmd.pmatch("CGDGD")) {
    return cgdgd;
  }else if (cmd.pmatch("CGDGB")) {
    return cgdgb;
  }else if (cmd.pmatch("CGDSB")) {
    return cgdsb;
  }else if (cmd.pmatch("CGDDB")) {
    return cgddb;
  }else if (cmd.pmatch("QDRN")) {
    return qdrn;
  }else if (cmd.pmatch("CDSDS")) {
    return cdsds;
  }else if (cmd.pmatch("CDGB")) {
    return cdgb;
  }else if (cmd.pmatch("CDSB")) {
    return cdsb;
  }else if (cmd.pmatch("CDDB")) {
    return cddb;
  }else if (cmd.pmatch("QBULK")) {
    return qbulk;
  }else if (cmd.pmatch("CBS")) {
    return cbs;
  }else if (cmd.pmatch("CBGB")) {
    return cbgb;
  }else if (cmd.pmatch("CBSB")) {
    return cbsb;
  }else if (cmd.pmatch("CBDB")) {
    return cbdb;
  }else if (cmd.pmatch("QBS")) {
    return qbs;
  }else if (cmd.pmatch("CBSBS")) {
    return cbsbs;
  }else if (cmd.pmatch("CBSGB")) {
    return cbsgb;
  }else if (cmd.pmatch("CBSSB")) {
    return cbssb;
  }else if (cmd.pmatch("CBSDB")) {
    return cbsdb;
  }else if (cmd.pmatch("QBD")) {
    return qbd;
  }else if (cmd.pmatch("CBDBD")) {
    return cbdbd;
  }else if (cmd.pmatch("CBDGB")) {
    return cbdgb;
  }else if (cmd.pmatch("CBDSB")) {
    return cbdsb;
  }else if (cmd.pmatch("CBDDB")) {
    return cbddb;
  }else if (cmd.pmatch("GTAU")) {
    return gtau;
  }else if (cmd.pmatch("CQGB")) {
    return cqgb;
  }else if (cmd.pmatch("CQSB")) {
    return cqsb;
  }else if (cmd.pmatch("CQDB")) {
    return cqdb;
  }else if (cmd.pmatch("CQBB")) {
    return cqbb;
  }else if (cmd.pmatch("VGS")) {
    return vgs;
  }
  /*}else */if (cmd.pmatch("VDS")) {
    return vds;
  }else if (cmd.pmatch("VBS")) {
    return vbs;
  }else if (cmd.pmatch("VDSAT")) {
    return vdsat;
  }else if (cmd.pmatch("VGST")) {
    return vgst;
  }else if (cmd.pmatch("VON")) {
    return von;
  }else if (cmd.pmatch("REVERSED")) {
    return reversed;
  }else if (cmd.pmatch("CUTOFF")) {
    return cutoff;
  }else if (cmd.pmatch("SUBTHRESHOLD")) {
    return subthreshold;
  }else if (cmd.pmatch("SATURATED")) {
    return saturated;
  }else if (cmd.pmatch("SBFWD")) {
    return sbfwd;
  }else if (cmd.pmatch("PUNCHTHRU")) {
    return punchthru;
  }else {
    return BASE_SUBCKT::tr_probe_num(cmd);
  }}
}
/*--------------------------------------------------------------------------*/
void DEV_MOS::reverse_if_needed()
{

    if (vds < 0) {
      error(bTRACE, long_label() + ": reversing\n");
      error(bTRACE, "before: vds=%g vgs=%g vbs=%g\n", vds, vgs, vbs);
      reversed = !reversed;
      vgs -= vds;
      vbs -= vds;
      vds = -vds;
      error(bTRACE, "after: vds=%g vgs=%g vbs=%g\n", vds, vgs, vbs);
      if (OPT::dampstrategy & dsREVERSE) {
	SIM::fulldamp = true;
	untested();
	error(bTRACE, long_label() + ":reverse damp\n");
      }
      if (!(OPT::mosflags & 0040)) {
	vbs = std::min(vbs,0.);
      }else{
	untested();
      }
    }
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
bool DEV_MOS::tr_needs_eval()const
{
  if (is_q_for_eval()) {
    untested();
    return false;
  }else if (!converged()) {
    return true;
  }else{
    const COMMON_MOS* c = prechecked_cast<const COMMON_MOS*>(common());
    assert(c);
    const MODEL_MOS_BASE* m=prechecked_cast<const MODEL_MOS_BASE*>(c->model());
    assert(m);
    polarity_t polarity = m->polarity;
    node_t eff_source((reversed) ? _n[n_idrain] : _n[n_isource]);
    node_t eff_drain((reversed) ? _n[n_isource] : _n[n_idrain]);
    return !(conchk(vds,polarity*(eff_drain.v0()-eff_source.v0()),OPT::vntol)
	     && conchk(vgs, polarity*(_n[n_gate].v0()-eff_source.v0()),
		       OPT::vntol)
	     && conchk(vbs, polarity*(_n[n_bulk].v0()-eff_source.v0()),
		       OPT::vntol));
  }
}
/*--------------------------------------------------------------------------*/
bool DEV_MOS::do_tr()
{
  const COMMON_MOS* c = prechecked_cast<const COMMON_MOS*>(common());
  assert(c);
  const MODEL_MOS_BASE* m = prechecked_cast<const MODEL_MOS_BASE*>(c->model());
  assert(m);

  bool was_cutoff = cutoff;
  bool was_subthreshold = subthreshold;
  bool was_saturated = saturated;
  bool was_reversed = reversed;
  bool was_sbfwd = sbfwd;
  polarity_t polarity = m->polarity;

  if (initial_step(SIM::mode)) {
    reversed = false;
    vds = vgs = vbs = 0.;
  }else{
    double Vds, Vgs, Vbs;
    if (reversed) {
      Vds = polarity * volts_limited(_n[n_isource],_n[n_idrain]);
      Vgs = polarity * volts_limited(_n[n_gate],_n[n_idrain]);
      Vbs = polarity * volts_limited(_n[n_bulk],_n[n_idrain]);
    }else{
      Vds = polarity * volts_limited(_n[n_idrain],_n[n_isource]);
      Vgs = polarity * volts_limited(_n[n_gate],_n[n_isource]);
      Vbs = polarity * volts_limited(_n[n_bulk],_n[n_isource]);
    }
    vgs = fet_limit_vgs(Vgs, vgs, von);
    if (_n[n_drain].n_() == _n[n_gate].n_()) {
      vds = Vds + (vgs - Vgs);
    }else{
      // Spice hacks Vds here, but my tests show that it often makes
      // convergence worse, and never improves it.
      // I am guessing that it does help when drain and gate are connected,
      // and Spice does it here in case they are and cannot be determined
      // whether they are or not.
      // The hack maintains Vdg after Vgs limiting.
      //Vds = Vds + (vgs - Vgs);
      vds = fet_limit_vds(Vds, vds);
    }
    vbs = std::min(Vbs, 0.);
    //vbs = pnj_limit(double Vbs, double vbs, double vt, double vcrit);
    //vds = Vds;
    //vgs = Vgs;
    //vbs = Vbs;
  }

  assert(qgate == qgate);
  assert(qgs == qgs);
  assert(qgd == qgd);
  assert(qdrn == qdrn);
  assert(qbulk == qbulk);
  assert(qbs == qbs);
  assert(qbd == qbd);

  m->tr_eval(this);

  assert(qgate == qgate);
  assert(qgs == qgs);
  assert(qgd == qgd);
  assert(qdrn == qdrn);
  assert(qbulk == qbulk);
  assert(qbs == qbs);
  assert(qbd == qbd);

  if (reversed) {
    idsxxx = ids + vds*gds + vgs*gmr + vbs*gmbr;
    isbxxx = isb - vds*gsbsd - vgs*gsbgd - vbs*gsbbd;
    idbxxx = 0.;
  }else{
    idsxxx = ids - vds*gds - vgs*gmf - vbs*gmbf;
    idbxxx = idb - vds*gdbds - vgs*gdbgs - vbs*gdbbs;
    isbxxx = 0.;
  }
  ids *= polarity;
  idsxxx *= polarity;
  if (c->m != 1.) {
    ids *= c->m;
    idsxxx *= c->m;
    gds *= c->m;
    gmf *= c->m;
    gmr *= c->m;
    gmbf *= c->m;
    gmbr *= c->m;

    idb *= c->m;
    idbxxx *= c->m;
    gdbds *= c->m;
    gdbgs *= c->m;
    gdbbs *= c->m;
    isb *= c->m;
    isbxxx *= c->m;
    gsbsd *= c->m;
    gsbgd *= c->m;
    gsbbd *= c->m;

    qgate *= c->m;
    cggb *= c->m;
    cgsb *= c->m;
    cgdb *= c->m;
    qdrn *= c->m;
    cddb *= c->m;
    cdgb *= c->m;
    cdsb *= c->m;
  }
  assert(subckt());
  set_converged(subckt()->do_tr());
  
  trace3(long_label().c_str(), vds, vgs, vbs);
  trace4("", ids, gmf, gds, gmbf);
  trace4("", ids, gmr, gds, gmbr);
  if (was_cutoff != cutoff  ||  was_subthreshold != subthreshold  
  	||  was_saturated != saturated  ||  was_reversed != reversed  
	||  was_sbfwd != sbfwd) {
    if (OPT::dampstrategy & dsDEVREGION) {
      SIM::fulldamp = true;
    }
    #if defined(DO_TRACE)
      error(bTRACE,"%s:%d: region change\n", long_label().c_str(), evaliter());
    #endif
  }
  return converged();
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
