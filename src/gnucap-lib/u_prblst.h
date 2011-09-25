/*$Id: u_prblst.h,v 25.94 2006/08/08 03:22:25 al Exp $ -*- C++ -*-
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
 * list of probes
 */
//testing=script,complete 2006.07.14
#ifndef U_PRBLST_H
#define U_PRBLST_H
#include "mode.h"
#include "u_probe.h"
/*--------------------------------------------------------------------------*/
class CARD_LIST;
/*--------------------------------------------------------------------------*/
class PROBELIST {
private:
  typedef std::vector<PROBE> container;
  container bag;

  explicit PROBELIST(const PROBELIST&) {unreachable();incomplete();}
public:
  explicit PROBELIST() {}
  ~PROBELIST() {}

  typedef container::iterator	    iterator;
  typedef container::const_iterator const_iterator;
  void	   listing(const std::string&)const;
  void     clear();
  void	   remove_list(CS&);
  void     remove_one(CKT_BASE*);
  void     add_list(CS&);
  int	   size()const		{return bag.size();}
  const_iterator begin()const	{return bag.begin();}
  const_iterator end()const	{return bag.end();}
private:
  bool    add_branches(const std::string&,const std::string&,const CARD_LIST*);
  void    add_all_nodes(const std::string&);
};
/*--------------------------------------------------------------------------*/
class PROBE_LISTS {
public:
 static PROBELIST alarm[sCOUNT]; // list of alarm probes
 static PROBELIST plot[sCOUNT];  // list of plot probes
 static PROBELIST print[sCOUNT]; // list of print probes
 static PROBELIST store[sCOUNT]; // list of probes to store for postproc
 static void purge(CKT_BASE*);
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
