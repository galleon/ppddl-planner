/* 
   ppddl-planner - client for IPPC'08

   Copyright (C) 2008 Florent Teichteil-Koenigsbuch and Guillaume Infantes and Ugur Kuter

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  

*/

#ifndef SYMBOLIC_SEARCH_H_
#define SYMBOLIC_SEARCH_H_

#include "commons/declarations.h"
#include "models/symbolic_mdp.h"
#include "models/dd_node_ptr.h"
#include "heuristics/symbolic_actiongraph.h"


class symbolic_fwdsearch {
 private:
  SymbolicMDP* mdp;
  dd_node_ptr init;
  symbolic_actiongraph* agraph;
  int bound_type;

  bool donep (const dd_node_ptr& fringe);

 public:
  /* constructor(s) and destructor. */
  symbolic_fwdsearch();
  ~symbolic_fwdsearch(); 
  
  /* Methods */
  void set_boundtype(int btype);
  int get_boundtype() const;
  
  void initialize(SymbolicMDP* m, symbolic_actiongraph& ag, const dd_node_ptr& current_state);
  symbolic_actiongraph* generate_actiongraph();

};

#endif /*SEARCH_H_*/
