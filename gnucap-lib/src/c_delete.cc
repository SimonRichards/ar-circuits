/*$Id: c_delete.cc,v 25.94 2006/08/08 03:22:25 al Exp $ -*- C++ -*-
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
 * delete and clear commands
 */
//testing=script,complete 2006.07.16
//BUG:: If someone deletes an element inside an instance of a subckt
// (like ".delete r1.x1", there is no error message, and the deleted
// element will reappear next time an elaboration occurs, which is 
// usually before anything else.
#include "e_card.h"
#include "s__.h"
#include "c_comand.h"
/*--------------------------------------------------------------------------*/
//	void	CMD::clear(CS&);
//	void	CMD::del(CS&);
/*--------------------------------------------------------------------------*/
/* cmd_clear: clear the whole circuit, including faults, etc
 *   equivalent to unfault; unkeep; delete all; title = (blank)
 */
void CMD::clear(CS&)
{
  {CS nil("");      unfault(nil);}
  {CS nil("");      unmark(nil);}
  {CS clr("clear"); ic(clr);}
  {CS clr("clear"); nodeset(clr);}
  {CS clr("clear"); alarm(clr);}
  {CS clr("clear"); plot(clr);}
  {CS clr("clear"); print(clr);}
  {CS All("all");   del(All);}
  {CS q("'");       title(q);}
}
/*--------------------------------------------------------------------------*/
static bool delete_one_name(const std::string& name, CARD_LIST* scope)
{
  assert(scope);

  std::string::size_type dotplace = name.find_last_of(".");
  if (dotplace != std::string::npos) {
    // has a dot, look deeper
    // Split the name into two parts:
    // "container" -- where to look (all following the dot)
    // "dev_name" -- what to look for (all before the dot)
    std::string container = name.substr(dotplace+1, std::string::npos);
    std::string dev_name = name.substr(0, dotplace);
    // container name must be exact match
    CARD* card = (*scope)[container];
    if (!card) {
      // can't find "container" (probably .subckt) - no match
      return false;
    }else if (!card->subckt()) {
      // found a match, but it isn't a container (subckt)
      return false;
    }else{
      // found the container, look inside
      return delete_one_name(dev_name, card->subckt());
    }
    unreachable();
  }else{
    // no dots, look here
    if (name.find_first_of("*?") != std::string::npos) {
      // there's a wild card.  do linear search for all matches
      bool didit = false;
      {
	CARD_LIST::iterator i = scope->begin();
	while (i != scope->end()) {
	  CARD_LIST::iterator old_i = i++;
	  // ^^^^^^^^^^^^ move i past the item being deleted
	  if (wmatch((**old_i).short_label(), name)) {
	    scope->erase(old_i);
	    didit = true;
	  }
	}
      }
      return didit;
    }else{
      // no wild card.  fast search for one exact match
      CARD_LIST::iterator i = scope->find(name);
      if (i != scope->end()) {
	scope->erase(i);
	return true;
      }else{
	return false;
      }
    }
    unreachable();
  }
  unreachable();
}
/*--------------------------------------------------------------------------*/
/* cmd_delete:  delete command
 */
void CMD::del(CS& cmd)
{
  SIM::uninit();
  if (cmd.pmatch("ALL")) {
    CARD_LIST::card_list.erase_all();
  }else{
    while (cmd.more()) {
      int mark = cmd.cursor();
      bool didit = delete_one_name(cmd.ctos(), &CARD_LIST::card_list);
      if (!didit) {
	cmd.warn(bWARNING, mark, "no match");
      }
    }
  }
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
