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

#include <algorithm>
#include <cmath>

#include "graph_fsp.h"

GraphFSP::GraphFSP(const Problem& pb, double epsilon, double discount_factor, unsigned int plan_length, heuristic_t heuristic_type,
					determinization_t determinization_type, deterministic_planner_t deterministic_planner_type)
try : BaseAlgorithm(pb, epsilon, discount_factor),
		GraphHeuristicAlgorithm<GraphStateWTA<> >(pb, epsilon, discount_factor, plan_length, heuristic_type, determinization_type, deterministic_planner_type)
{
}
catch (BaseException& error)
{
	error.push_function_backtrace("GraphFSP::GraphFSP");
	throw;
}


void GraphFSP::solve_initialize(const PddlState& st)
{
	forward_reachable_states_.clear();
	backward_reachable_states_.clear();
	states_frontier_.clear();
	GraphHeuristicAlgorithm<GraphStateWTA<> >::solve_initialize(st);
	forward_reachable_states_.insert(initial_state_);
	states_frontier_.insert(initial_state_);
}


void GraphFSP::solve_progress()
{
	compute_backward_reachability();
	optimize(backward_reachable_states_);
	compute_forward_reachability();
}


bool GraphFSP::has_converged()
{
	return states_frontier_.empty();
}


void GraphFSP::compute_backward_reachability()
{
	backward_reachable_states_.clear();

	for (states_set_t::iterator Ist = states_frontier_.begin() ; Ist != states_frontier_.end() ; ++Ist)
		backward_reachable_states_.insert(*Ist);

	states_set_t forward_reachable_states_minus_frontier;
	std::set_difference(forward_reachable_states_.begin(), forward_reachable_states_.end(),
			states_frontier_.begin(), states_frontier_.end(),
			std::inserter(forward_reachable_states_minus_frontier, forward_reachable_states_minus_frontier.begin()));

	states_set_t backward_frontier = backward_reachable_states_;

	while (!backward_frontier.empty())
	{
		states_set_t new_backward_frontier;

		for (states_set_t::iterator Ist = backward_frontier.begin() ; Ist != backward_frontier.end() ; ++Ist)
		{
			for (GraphStateWTA<>::ancestors_map_t::iterator Ipst = (*Ist)->_ancestors_().begin() ; Ipst != (*Ist)->_ancestors_().end() ; ++Ipst)
			{
				GraphStateWTA<>& anc = *(Ipst->first);

				if (forward_reachable_states_minus_frontier.find(&anc) != forward_reachable_states_minus_frontier.end())
				{
					if (Ipst->second.find(&(Ipst->first->get_policy())) != Ipst->second.end())
					{
						std::pair<states_set_t::iterator, bool> res = backward_reachable_states_.insert(&anc);;

						if (res.second)
							new_backward_frontier.insert(&anc);
					}
				}
			}
		}

		backward_frontier = new_backward_frontier;
	}
}


void GraphFSP::compute_forward_reachability()
{
	states_set_t new_reachable_states;
	new_reachable_states.insert(initial_state_);
	states_set_t forward_frontier;
	forward_frontier.insert(initial_state_);
	states_frontier_.clear();

	while (!forward_frontier.empty())
	{
		states_set_t new_forward_frontier;

		for (states_set_t::iterator Ist = forward_frontier.begin() ; Ist != forward_frontier.end() ; ++Ist)
		{
			std::list<GraphTransition<GraphStateWTA<> >*>& effects = (*Ist)->_transitions_().find(&((*Ist)->get_policy()))->second;

			for (std::list<GraphTransition<GraphStateWTA<> >*>::iterator Ieff = effects.begin() ; Ieff != effects.end() ; ++Ieff)
			{
				GraphStateWTA<>& nst = (*Ieff)->_outcome_();
				std::pair<states_set_t::iterator, bool> res = new_reachable_states.insert(&nst);

				if (res.second)
				{
					if (forward_reachable_states_.find(&nst) != forward_reachable_states_.end())
						new_forward_frontier.insert(&nst);
					else
						states_frontier_.insert(&nst);
				}
			}
		}

		forward_frontier = new_forward_frontier;
	}

	forward_reachable_states_ = new_reachable_states;

	for (states_set_t::iterator Ist = states_frontier_.begin() ; Ist != states_frontier_.end() ; ++Ist)
	{
		if (!((*Ist)->expanded()))
			initialize(**Ist);
	}
}
