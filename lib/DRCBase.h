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

#ifndef __DRCBASE_H__
#define __DRCBASE_H__

#include <boost/foreach.hpp>
#include <tr1/memory>
#include <list>
#include <LogicModel.h>
#include <DRCVContainer.h>

namespace degate {

  /**
   * An enum for several types of Design Rule Check problem types.
   */
  enum DRC_SEVERITY {
    DRC_UNDEFINED = 0,
    DRC_ERROR = 1,
    DRC_WARNING = 2
  };


  /**
   * Base class for Design Rule Checks.
   */

  class DRCBase {
  public:

    typedef DRCVContainer container_type;


  private:

    std::string _class_name;
    std::string _description;
    DRC_SEVERITY _severity;

    container_type drc_violations;

  public:

    /**
     * The constructor.
     * @param short_name Short name for the DRC class.
     * @param description A decription of what the DRC basically checks.
     */
    DRCBase(std::string const& class_name,
	    std::string const& description,
	    DRC_SEVERITY severity = DRC_ERROR) :
      _class_name(class_name),
      _description(description),
      _severity(severity) {
    }

    virtual ~DRCBase() {}

    /**
     * The run method is abstract and must be implemented in derived
     * classes. The implementation should check for design rule violations.
     * Each DRC violation must be stored via method add_drc_violation().
     * Note: Because run() can be called multiple times, at the beginning of
     * run() you must clear the list of detected violations.
     */
    virtual void run(LogicModel_shptr lmodel) = 0;

    /**
     * Get the list of DRC violations.
     */

    container_type get_drc_violations() const {
      return drc_violations;
    }

    /**
     * Get the class name of a DRC violation.
     * @return Returns the DRC violation class name as a string.
     */
    std::string get_drc_class_name() const {
      return _class_name;
    }

    DRC_SEVERITY get_severity() const {
      return _severity;
    }

  protected:

    /**
     * Add a DRC violation to the list of already detected violations.
     */
    void add_drc_violation(DRCViolation_shptr violation) {
      drc_violations.push_back(violation);
    }

    /**
     * Clear list of detected violations.
     */
    void clear_drc_violations() {
      drc_violations.clear();
    }
  };

  typedef std::tr1::shared_ptr<DRCBase> DRCBase_shptr;


}

#include <DRCViolation.h>

#endif
