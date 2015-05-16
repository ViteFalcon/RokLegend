#pragma once
#ifndef ROKLEGEND_EXCEPTION_H
#define ROKLEGEND_EXCEPTION_H

#include "RoDefines.h"
#include <boost/exception/all.hpp>
#include <exception>

struct RoException : virtual std::exception, virtual boost::exception
{
    RoException(){}
    RoException(const char* typeName)
#if _WIN32
        : std::exception(typeName)
#endif
    {
    }

    virtual const char* what() const throw();
};

#define roEXCEPTION_NAME(name) roCOMBINE(Ro,name)
#define roDEFINE_EXCEPTION(name) struct roEXCEPTION_NAME(name) : virtual RoException\
{\
    roEXCEPTION_NAME(name)()\
        : RoException(roSTRINGIFY(name))\
    {\
    }\
}

roDEFINE_EXCEPTION(FileNotFound);
roDEFINE_EXCEPTION(ItemNotFound);
roDEFINE_EXCEPTION(InvalidGrfException);
roDEFINE_EXCEPTION(InvalidOperation);

#define roTHROW(exception) boost::exception_detail::throw_exception_(exception,BOOST_CURRENT_FUNCTION,roFILENAME,__LINE__)
#define roTHROW_IF(condition, exception) if (condition) { roTHROW(exception); }

#define roTRY try {
#define roEXIT_GAME roPOST_MSG(ExitGame, RoPropertyMap::Empty)
#define roCATCH_BOOST_EXCEPTION } catch (boost::exception& boostException) \
{\
    roLOG_CRIT << boost::diagnostic_information_what(boostException);
#define roCATCH_STD_EXCEPTION } catch (std::exception& exception) \
{\
    roLOG_CRIT << "[EXCEPTION]: " << exception.what();
#define roCATCH_UNKNOWN_EXCEPTION } catch (...) \
{\
    roLOG_CRIT << "[UNKNOWN EXCEPTION]: An unknown exception type was caught!";
#define roCATCH_ANY_EXCEPTION } catch (...) {
#define roCATCH_END }
#define roCATCH_EXCEPTIONS roCATCH_STD_EXCEPTION roCATCH_BOOST_EXCEPTION roCATCH_UNKNOWN_EXCEPTION roCATCH_END
#define roCATCH_EXCEPTIONS_AND_EXIT roCATCH_STD_EXCEPTION roEXIT_GAME; roCATCH_BOOST_EXCEPTION roEXIT_GAME; roCATCH_UNKNOWN_EXCEPTION roEXIT_GAME; roCATCH_END

#endif // ROKLEGEND_EXCEPTION_H
