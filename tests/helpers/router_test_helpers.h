/*
  Copyright (c) 2015, 2017, Oracle and/or its affiliates. All rights reserved.

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

#ifndef ROUTER_TESTS_TEST_HELPERS_INCLUDED
#define ROUTER_TESTS_TEST_HELPERS_INCLUDED

#define SKIP_GIT_TESTS(COND)\
  if(COND) {\
     std::cout << "[  SKIPPED ] Tests using Git repository skipped" << std::endl;\
     return;\
  }

#define ASSERT_THROW_LIKE(expr, exc, msg) try { \
      expr;\
      FAIL() << "Expected exception of type " #exc << " but got none\n";\
    } catch (exc &e) {\
      if (std::string(e.what()).find(msg) == std::string::npos) {\
          FAIL() << "Expected exception with message: " << msg << "\nbut got: " << e.what() << "\n";\
      }\
    } catch (...) {\
      FAIL() << "Expected exception of type " #exc << " but got another\n";\
    }

#define EXPECT_THROW_LIKE(expr, exc, msg) try { \
      expr;\
      ADD_FAILURE() << "Expected exception of type " #exc << " but got none\n";\
    } catch (exc &e) {\
      if (std::string(e.what()).find(msg) == std::string::npos) {\
          ADD_FAILURE() << "Expected exception with message: " << msg << "\nbut got: " << e.what() << "\n";\
      }\
    } catch (...) {\
      ADD_FAILURE() << "Expected exception of type " #exc << " but got another\n";\
    }

#include "filesystem.h"

/** @brief Returns the CMake source root folder
 *
 * @return mysql_harness::Path
 */
mysql_harness::Path get_cmake_source_dir();

/** @brief Gets environment variable as path
 *
 * Gets environment envvar and returns it as Path. When the environment
 * variable is not set, the alternative is tried.
 *
 * Throws runtime_error when the folder is not available.
 *
 * @param envvar Name of the environment variable
 * @param Path Alternative Path when environment variable is not available
 * @return mysql_harness::Path
 */
mysql_harness::Path get_envvar_path(const std::string &envvar, mysql_harness::Path alternative);

/** @brief Returns the current working directory
 *
 * Uses `getcwd()` and returns the current working directory as as std::string.
 *
 * Throws std::runtime_error on errors.
 *
 * @return std::string
 */
const std::string get_cwd();

/** @brief Changes the current working directory
 *
 * Uses `chdir()` to change the current working directory. When succesfully
 * change to the folder, the old working directory is returned.
 *
 * Throws std::runtime_error on errors.
 *
 * @return std::string
 */
const std::string change_cwd(std::string &dir);

/** @brief Checks whether string ends with the specified suffix
 *
 * Returns true if the string ends with the given suffix.
 *
 * @return bool
 */
bool ends_with(const std::string &str, const std::string &suffix);

/** @brief Checks whether string starts with the specified prefix
 *
 * Returns true if the string begins with the given prefix.
 *
 * @return bool
 */
bool starts_with(const std::string &str, const std::string &prefix);

/** @brief Reads a specified number of bytes from a non-blocking socket
 *
 * reads a non-blocking socket until one of three things happen:
 *   1. specified number of bytes have been read - returns this number
 *   2. timeout expires - throws, describing the error
 *   3. read() fails    - throws, describing the error
 *
 * Returns number of bytes read (should be the number of bytes requested,
 * can be less on EOF).  Throws std::runtime_error on I/O error or timeout;
 * the reason can be extracted from the thrown object with what() method.
 *
 * @param socket file decriptor
 * @param buffer to store read bytes
 * @param number of bytes to read
 * @param timeout expressed in milliseconds
 *
 * @return number of bytes read
 */
size_t read_bytes_with_timeout(int sockfd, void* buffer, size_t n_bytes, uint64_t timeout_in_ms);

#ifdef _WIN32
std::string get_last_error(int err_code);
#endif

/** @brief Initializes Windows sockets (no-op on other OSes)
 *
 * Exits program with error upon failure.
 */
void init_windows_sockets();

#endif // ROUTER_TESTS_TEST_HELPERS_INCLUDED
