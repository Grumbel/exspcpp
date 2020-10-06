// exsp - Spawning subprocesses in C++
// Copyright (C) 2008-2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "spawn.hpp"

#include <iostream>
#include <string.h>
#include <unistd.h>

namespace exsp {

void spawn(std::filesystem::path const& exe_name, std::vector<std::string> const& args)
{
  // fork()/exev() makes it tricky to catch failures in spawning, so
  // just do some crude checks instead.
  auto const& status = std::filesystem::status(exe_name);

  switch (status.type()) {
    case std::filesystem::file_type::regular: {
      // ok
      break;
    }

    case std::filesystem::file_type::not_found: {
      std::ostringstream oss;
      oss << exe_name << ": not found";
      throw std::runtime_error(oss.str());
    }

    default: {
      std::ostringstream oss;
      oss << exe_name << ": invalid file type";
      throw std::runtime_error(oss.str());
    }
  }

  if ((status.permissions() & std::filesystem::perms::owner_exec) == std::filesystem::perms::none) {
    std::ostringstream oss;
    oss << exe_name << ": not executable";
    throw std::runtime_error(oss.str());
  }

  // do the actual spawning
  pid_t pid = fork();
  if (pid < 0) {
    std::ostringstream out;
    out << "failed to fork(): " << strerror(errno);
    throw std::runtime_error(out.str());
  } else if (pid == 0) {
    // child
    std::vector<char*> argv(args.size() + 2);
    argv.front() = strdup(exe_name.c_str());
    std::transform(args.begin(), args.end(), argv.begin() + 1,
                   [](std::string const& text){ return strdup(text.c_str()); });
    argv.back() = nullptr;

    if (execv(exe_name.c_str(), argv.data()) < 0) {
      std::cerr << "failed to execv(): " << strerror(errno) << std::endl;
      _exit(EXIT_FAILURE);
    }
  }
}

} // namespace exsp

/* EOF */
