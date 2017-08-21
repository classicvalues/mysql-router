/*
  Copyright (c) 2017, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MYSQL_HARNESS_LOGGER_HANDLER_INCLUDED
#define MYSQL_HARNESS_LOGGER_HANDLER_INCLUDED

#include "mysql/harness/logging/logging.h"
#include "harness_export.h"

#include <iostream>
#include <fstream>
#include <mutex>
#include <string>

namespace mysql_harness {

namespace logging {

/**
 * Base class for log message handler.
 *
 * This class is used to implement a log message handler. You need
 * to implement the `do_log` primitive to process the log
 * record. If, for some reason, the implementation is unable to log
 * the record, and exception can be thrown that will be caught by
 * the harness.
 */
class HARNESS_EXPORT Handler {
 public:
  /**
   * Default identifier
   *
   * Every handler provides a default name which could be used as key in
   * registry to uniquely identify it. There is no obligation to use it, it
   * is only supplied for convenience. In case of many instances of the same
   * handler, using a key derived from this default (such as
   * "my_handler:instance1") is suggested.
   *
   * This field should be set in derived classes
   */
  static constexpr const char* kDefaultName = nullptr;

  virtual ~Handler() = default;

  void handle(const Record& record);

  void set_level(LogLevel level) { level_ = level; }
  LogLevel get_level() const { return level_; }

 protected:
  std::string format(const Record& record) const;

  explicit Handler(bool format_messages, LogLevel level);

 private:
  /**
   * Log message handler primitive.
   *
   * This member function is implemented by subclasses to properly log
   * a record wherever it need to be logged.  If it is not possible to
   * log the message properly, an exception should be thrown and will
   * be caught by the caller.
   *
   * @param record Record containing information about the message.
   */
  virtual void do_log(const Record& record) = 0;

  /**
   * Flags if log messages should be formatted (prefixed with log level,
   * timestamp, etc) before logging.
   */
  bool format_messages_;

  /**
   * Log level set for the handler.
   */
  LogLevel level_;
};

/**
 * Handler to write to an output stream.
 *
 * @code
 * Logger logger("my_module");
 * ...
 * logger.add_handler(StreamHandler(std::clog));
 * @endcode
 */
class HARNESS_EXPORT StreamHandler : public Handler {
 public:
  static constexpr const char* kDefaultName = "stream";

  explicit StreamHandler(std::ostream& stream,
                         bool format_messages = true,
                         LogLevel level = LogLevel::kNotSet);

 protected:
  std::ostream& stream_;
  std::mutex stream_mutex_;

 private:
  void do_log(const Record& record) override;
};

/**
 * Handler that writes to a file.
 *
 * @code
 * Logger logger("my_module");
 * ...
 * logger.add_handler(FileHandler("/var/log/router.log"));
 * @endcode
 */
class HARNESS_EXPORT FileHandler : public StreamHandler {
 public:
  static constexpr const char* kDefaultName = "file";

  explicit FileHandler(const Path& path,
                       bool format_messages = true,
                       LogLevel level = LogLevel::kNotSet);
  ~FileHandler();

 private:
  std::ofstream fstream_;
};


}  // namespace logging

}  // namespace mysql_harness

#endif /* MYSQL_HARNESS_LOGGER_HANDLER_INCLUDED */
