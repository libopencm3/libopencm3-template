# /*
# * Copyright (C) 2019 ChinhPC <chinhphancong@outlook.com>
# * 
# * Author: ChinhPC
# *
# * This file is free software: you can redistribute it and/or modify it
# * under the terms of the GNU General Public License as published by the
# * Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# * 
# * This file is distributed in the hope that it will be useful, but
# * WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# * See the GNU General Public License for more details.
# * 
# * You should have received a copy of the GNU General Public License along
# * with this program.  If not, see <http://www.gnu.org/licenses/>.
# */

U_SHARED_DIR := include
CFILES += Src/my-project.c

VPATH += $(U_SHARED_DIR)
INCLUDES += $(patsubst %,-I%,$(U_SHARED_DIR))
