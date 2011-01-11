/* -*-c++-*-

  This file is part of the IC reverse engineering tool degate.

  Copyright 2008, 2009, 2010 by Martin Schobert

  Degate is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.

  Degate is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with degate. If not, see <http://www.gnu.org/licenses/>.

*/

#include <DRCBase.h>
#include <DRCOpenPorts.h>

using namespace degate;

DRCOpenPorts::DRCOpenPorts() :
  DRCBase("open_port", "Check for unconnected ports.", DRC_ERROR) {
}

void DRCOpenPorts::run(LogicModel_shptr lmodel) {
  clear_drc_violations();

  if(lmodel == NULL) return;

  // iterate over Gates
  debug(TM, "\tDRC: iterate over gates.");

  for(LogicModel::gate_collection::iterator g_iter = lmodel->gates_begin();
      g_iter != lmodel->gates_end(); ++g_iter) {

    Gate_shptr gate = g_iter->second;

    for(Gate::port_const_iterator p_iter = gate->ports_begin();
	p_iter != gate->ports_end(); ++p_iter) {

      GatePort_shptr port = *p_iter;
      assert(port != NULL);

      Net_shptr net = port->get_net();
      if(net == NULL || net->size() <= 1) {

	boost::format f("Port %1% is unconnected.");
	f % port->get_descriptive_identifier();

	debug(TM, "\tDRC: found a vioation.");
	add_drc_violation(DRCViolation_shptr(new DRCViolation(port, f.str(),
							      get_drc_class_name())));
      }
    }
  }
}

