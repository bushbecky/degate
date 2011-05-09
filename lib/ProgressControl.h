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

#ifndef __PROGRESSCONTROL_H__
#define __PROGRESSCONTROL_H__

#include <tr1/memory>
#include <time.h>
#include <boost/thread.hpp>

namespace degate {

  /**
   */

  class ProgressControl {

  private:

    const static int averaging_buf_size = 5;

    double progress;
    bool canceled;

    double step_size;
    time_t time_started;

    time_t estimated[averaging_buf_size];
    int estimated_idx;

    std::string log_message;
    bool log_message_set;

    boost::recursive_mutex mtx;

  private:

    time_t get_time_left_averaged() {
      estimated[estimated_idx++] = get_time_left();
      estimated_idx %= averaging_buf_size;

      unsigned int sum = 0, valid_values = 0;
      for(int i = 0; i < averaging_buf_size; i++)
	if(estimated[i] != -1) {
	  sum += estimated[i];
	  valid_values++;
	}

      return valid_values > 0 ? sum / valid_values : get_time_left_averaged();
    }

  protected:

    /**
     * Set progress.
     */
    virtual void set_progress(double progress) {
      boost::recursive_mutex::scoped_lock(mtx);
      this->progress = progress;
    }

    /**
     * Set step size.
     */
    virtual void set_progress_step_size(double step_size) {
      boost::recursive_mutex::scoped_lock(mtx);
      this->step_size = step_size;
    }

    /**
     * Increase progress.
     */
    virtual void progress_step_done() {
      boost::recursive_mutex::scoped_lock(mtx);
      progress += step_size;
    }

    /**
     * Reset progress and cancel state.
     */
    virtual void reset_progress() {
      boost::recursive_mutex::scoped_lock(mtx);
      time_started = time(NULL);
      canceled = false;
      progress = 0;

      for(int i = 0; i < averaging_buf_size; i++) estimated[i] = -1;

      estimated_idx = 0;
    }

  public:

    /**
     * The constructor
     */

    ProgressControl() : log_message_set(false) {
      reset_progress();
    }

    /**
     * The destructor for a plugin.
     */

    virtual ~ProgressControl() {}

    /**
     * Check if the process is canceled.
     */

    virtual bool is_canceled() const {
      boost::recursive_mutex::scoped_lock(mtx);
      return canceled;
    }

    /**
     * Stop the processing.
     */

    virtual void cancel() {
      boost::recursive_mutex::scoped_lock(mtx);
      canceled = true;
    }


    /**
     * Get progress.
     * @return Returns a value between 0 and 100 percent.
     */

    virtual double get_progress() const {
      boost::recursive_mutex::scoped_lock(mtx);
      return progress;
    }

    /**
     * Get (real) time since the progress counter was resetted.
     */
    virtual time_t get_time_passed() const {
      boost::mutex::scoped_lock(m_mutex);
      return time(NULL) - time_started;
    }

    /**
     * Get estimated time left in seconds.
     * @return Returns the time to go in seconds or -1, if
     *   that time cannot be calculated.
     */
    virtual time_t get_time_left() const {
      boost::recursive_mutex::scoped_lock(mtx);
      if(progress < 1.0)
	return progress > 0 ? (1.0 - progress) * get_time_passed() / progress : -1;
      return 0;
    }

    virtual std::string get_time_left_as_string() {
      boost::recursive_mutex::scoped_lock(mtx);
      time_t time_left = get_time_left_averaged();
      if(time_left == -1) return std::string("-");
      else {
	char buf[100];
	if(time_left < 60)
	  snprintf(buf, sizeof(buf), "%d s", (int)time_left);
	else if(time_left < 60*60)
	  snprintf(buf, sizeof(buf), "%d:%02d m", (int)time_left / 60, (int)time_left % 60);
	else {
	  unsigned int minutes = (int)time_left / 60;
	  snprintf(buf, sizeof(buf), "%d:%02d h", minutes / 60, (int)time_left % 60);
	}
	return std::string(buf);
      }

    }


    virtual void set_log_message(std::string const& msg) {
      boost::recursive_mutex::scoped_lock(mtx);
      log_message = msg;
      log_message_set = true;
    }

    virtual std::string get_log_message() const {
      boost::recursive_mutex::scoped_lock(mtx);
      return log_message;
    }

    virtual bool has_log_message() const {
      boost::recursive_mutex::scoped_lock(mtx);
      return log_message_set;
    }
  };

  typedef std::tr1::shared_ptr<ProgressControl> ProgressControl_shptr;
}

#endif

