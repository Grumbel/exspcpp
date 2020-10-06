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

#include <filesystem>
#include <optional>

#include <string.h>

namespace exsp {

std::optional<std::filesystem::path>
find_exe(std::string_view name)
{
  char* path_c = getenv("PATH");
  if (!path_c) {
    return {};
  } else {
    const char* delim = ":";
    char* path = strdup(path_c);
    char* state;
    for(char* p = strtok_r(path, delim, &state); p != nullptr; p = strtok_r(nullptr, delim, &state)) {
      std::filesystem::path exepath(p);
      exepath /= name;
      try {
        auto const& status = std::filesystem::status(exepath);
        if ((status.type() == std::filesystem::file_type::regular) &&
            ((status.permissions() & std::filesystem::perms::owner_exec) !=
             std::filesystem::perms::none))
        {
          free(path);
          return exepath;
        }
      } catch(...) {
        // ignore errors
      }
    }
    free(path);

    return {};
  }
}

} // namespace exsp

/* EOF */
