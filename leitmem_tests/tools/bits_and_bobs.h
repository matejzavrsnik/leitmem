/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bits_and_bobs.h
 * Author: matej
 *
 * Created on March 3, 2019, 7:22 PM
 */

#ifndef BITS_AND_BOBS_H
#define BITS_AND_BOBS_H

inline std::tm create_some_time()
{
   std::tm some_time;
   some_time.tm_gmtoff = 0;	
   some_time.tm_hour = 19;	
   some_time.tm_isdst = 0;	
   some_time.tm_mday = 10;	
   some_time.tm_min = 20;
   some_time.tm_mon = 1;
   some_time.tm_sec = 21;	
   some_time.tm_wday = 0;
   some_time.tm_yday = 40;	
   some_time.tm_year = 119;	
   some_time.tm_zone = "GMT";
   return some_time;
}

#endif /* BITS_AND_BOBS_H */

