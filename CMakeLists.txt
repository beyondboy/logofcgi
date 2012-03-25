#    Copyright (C) 2011 Raphters authors,
#    
#    This file is part of Raphters.
#    
#    Raphters is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Raphters is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(simple)
#SET(RaphtersSources dispatcher request response raphters)
#INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}" "/usr/local/include")
LINK_DIRECTORIES("/usr/local/lib")
#ADD_LIBRARY(raphters_dynamic SHARED ${RaphtersSources})
#LINK_DIRECTORIES("/home/beyond_boy/CV_OpenSource/fcgi-2.4.0/libfcgi")
#SET_TARGET_PROPERTIES(raphters_dynamic PROPERTIES LINKER_LANGUAGE C)
#TARGET_LINK_LIBRARIES(raphters_dynamic fcgi)
#FIND_PACKAGE( Fcgi REQUIRED)
LINK_LIBRARIES(fcgi++)
FIND_PACKAGE( OpenCV REQUIRED)
#ADD_LIBRARY(raphters_static STATIC ${RaphtersSources})
#SET_TARGET_PROPERTIES(raphters_static PROPERTIES LINKER_LANGUAGE C)

ADD_EXECUTABLE(simple logofcgi.cpp)
TARGET_LINK_LIBRARIES(simple fcgi ${OpenCV_LIBS} fcgi++)
