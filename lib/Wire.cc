/*
 
 This file is part of the IC reverse engineering tool degate.
 
 Copyright 2008, 2009 by Martin Schobert
 
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

#include "globals.h"
#include "LogicModelObjectBase.h"
#include "Net.h"
#include "Layer.h"
#include "LogicModel.h"
#include "ConnectedLogicModelObject.h"
#include "degate_exceptions.h"
#include "Wire.h"

using namespace degate;

Wire::Wire(int _from_x, int _from_y, int _to_x, int _to_y, unsigned int _diameter) : 
  Line(_from_x, _from_y, _to_x, _to_y, _diameter) {
}

const std::string Wire::get_descriptive_identifier() const {
  if(has_name()) {
    boost::format fmter("%1% (%2%)");
    fmter % get_name() % get_object_id();
    return fmter.str();
  }
  else {
    boost::format fmter("wire (%1%)");
    fmter % get_object_id();
    return fmter.str();
  }
}

const std::string Wire::get_object_type_name() const {
  return std::string("Wire");
}

void Wire::print(std::ostream & os, int n_tabs) const {
}
