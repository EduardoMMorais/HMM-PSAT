/*
 * File:   c++11_warning.h
 * Author: eduardo
 *
 * Created on 2 de Março de 2012, 15:52
 */

#ifndef CPP11_TESTED
#define CPP11_TESTED


#if (__cplusplus <= 199711L) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
   #error This software requires c++11 (AKA c++0x)
#endif

#endif //  CPP11_TESTED
