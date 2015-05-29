SET(SQLITE_SOURCES
	#sqlite/shell.c
	sqlite/sqlite3.c
	sqlite/sqlite3.h
	sqlite/sqlite3ext.h
)
RO_SOURCE_GROUP(sqlite ${SQLITE_SOURCES})

SET(SQLITEPP_SOURCES
	sqlitepp/binders.cpp
	sqlitepp/binders.hpp
	sqlitepp/converters.hpp
	sqlitepp/exception.cpp
	sqlitepp/exception.hpp
	sqlitepp/into.hpp
	sqlitepp/query.cpp
	sqlitepp/query.hpp
	sqlitepp/session.cpp
	sqlitepp/session.hpp
	sqlitepp/sqlitepp.hpp
	sqlitepp/statement.cpp
	sqlitepp/statement.hpp
	sqlitepp/string.cpp
	sqlitepp/string.hpp
	sqlitepp/transaction.cpp
	sqlitepp/transaction.hpp
	sqlitepp/unicode.hpp
	sqlitepp/use.hpp
)
RO_SOURCE_GROUP(sqlitepp ${SQLITEPP_SOURCES})

ADD_LIBRARY(sqlitepp STATIC ${SQLITE_SOURCES} ${SQLITEPP_SOURCES} CMakeLists.txt)
COTIRE(sqlitepp)
