/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DrawCall.h
Module:         Graphics
Purpose:        Class for managing draw calls
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

struct DrawCall
{
  // METHODS ////////////////////////////////////////////////////////
  //constructor
  DrawCall( )
  {
    data_ = 0;
    buffer_ = 0;
    texture_ = 0;
    index_ = 0;

    DataStart_ = (unsigned long long*)&DataStart_ + 4;
  }

  //get the whole data
  unsigned long long GetData( )
  {
    return (unsigned long long)((&DataStart_)[ 1 ]);
  }

  //access individual data...


  // MEMBERS ////////////////////////////////////////////////////////
  unsigned long long *DataStart_;
  //in 64 bit, these need to add up to 16. in 32, add up to 80
  int data_ : 16;      //this is LSB
  int buffer_ : 16;
  int texture_ : 16;
  int index_ : 16;     //MSB

  //if in win32, use this to ensure bit alignment to 64 bits
  //int index2 : 16; //MSB

  //disable these because it will cause so many problems
private:
  DrawCall( const DrawCall& );
  DrawCall &operator=( const DrawCall& );
};