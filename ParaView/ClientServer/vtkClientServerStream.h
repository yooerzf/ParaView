/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkClientServerStream.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkClientServerStream - Store messages for the interpreter.
// .SECTION Description
// vtkClientServerStream will store zero or more almost arbitrary
// messages in a platform-independent manner.  The stream's data may
// be copied from one platform to another via GetData/SetData methods,
// and the message represented will remain unchanged.  Messages are
// used to represent both commands and results for
// vtkClientServerInterpreter, but they may be used for any purpose.

#ifndef __vtkClientServerStream_h
#define __vtkClientServerStream_h

#include "vtkClientServerID.h"

class vtkClientServerStreamInternals;

class VTK_CLIENT_SERVER_EXPORT vtkClientServerStream
{
public:
  // Description:
  // Constructor/Destructor manage references of vtk objects stored in
  // the stream along with the rest of the stream data.
  vtkClientServerStream();
  ~vtkClientServerStream();

  // Description:
  // Copy constructor and assignment operator copy all stream data.
  vtkClientServerStream(const vtkClientServerStream&);
  vtkClientServerStream& operator=(const vtkClientServerStream&);

  // Description:
  // Enumeration of message types that may be stored in a stream.
  // This must be kept in sync with the string table in this class's
  // .cxx file.
  enum Commands { New, Invoke, Delete, Assign,
                  Reply, Error, EndOfCommands};

  // Description:
  // Enumeration of data types that may be stored in a stream.  This
  // must be kept in sync with the string table in this class's .cxx
  // file.
  enum Types {
    int8_value, int8_array,
    int16_value, int16_array,
    int32_value, int32_array,
    int64_value, int64_array,
    uint8_value, uint8_array,
    uint16_value, uint16_array,
    uint32_value, uint32_array,
    uint64_value, uint64_array,
    float32_value, float32_array,
    float64_value, float64_array,
    string_value,
    id_value,
    vtk_object_pointer,
    stream_value,
    LastResult,
    End
  };

  // Description:
  // Ask the stream to allocate at least the given size in memory to
  // avoid too many reallocations during stream construction.
  void Reserve(size_t size);

  // Description:
  // Reset the stream to an empty state.
  void Reset();

  // Description:
  // Copy the stream contents from another stream.
  void Copy(const vtkClientServerStream* source);

  //--------------------------------------------------------------------------
  // Stream reading methods:

  // Description:
  // Get the number of complete messages currently stored in the
  // stream.
  int GetNumberOfMessages() const;

  // Description:
  // Get the command in the message with the given index.  Returns
  // EndOfCommands if the given index is out of range.
  vtkClientServerStream::Commands GetCommand(int message) const;

  // Description:
  // Get the number of arguments in the message with the given index.
  // Returns a value less than 0 if the given index is out of range.
  int GetNumberOfArguments(int message) const;

  // Description:
  // Get the type of the given argument in the given message.  Returns
  // End if either index is out of range.
  vtkClientServerStream::Types GetArgumentType(int message, int argument) const;

  // Description:
  // Get the value of the given argument in the given message.
  // Returns whether the argument could be converted to the requested
  // type.
  int GetArgument(int message, int argument, signed char* value) const;
  int GetArgument(int message, int argument, char* value) const;
  int GetArgument(int message, int argument, short* value) const;
  int GetArgument(int message, int argument, int* value) const;
  int GetArgument(int message, int argument, long* value) const;
  int GetArgument(int message, int argument, unsigned char* value) const;
  int GetArgument(int message, int argument, unsigned short* value) const;
  int GetArgument(int message, int argument, unsigned int* value) const;
  int GetArgument(int message, int argument, unsigned long* value) const;
  int GetArgument(int message, int argument, float* value) const;
  int GetArgument(int message, int argument, double* value) const;
#ifdef VTK_TYPE_INT64_NOT_STANDARD
  int GetArgument(int message, int argument, vtkTypeInt64* value) const;
  int GetArgument(int message, int argument, vtkTypeUInt64* value) const;
#endif
  int GetArgument(int message, int argument, signed char* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, char* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, short* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, int* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, long* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, unsigned char* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, unsigned short* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, unsigned int* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, unsigned long* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, float* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, double* value, vtkTypeUInt32 length) const;
#ifdef VTK_TYPE_INT64_NOT_STANDARD
  int GetArgument(int message, int argument, vtkTypeInt64* value, vtkTypeUInt32 length) const;
  int GetArgument(int message, int argument, vtkTypeUInt64* value, vtkTypeUInt32 length) const;
#endif
  int GetArgument(int message, int argument, const char** value) const;
  int GetArgument(int message, int argument, char** value) const;
  int GetArgument(int message, int argument, vtkClientServerStream* value) const;
  int GetArgument(int message, int argument, vtkClientServerID* value) const;
  int GetArgument(int message, int argument, vtkObjectBase** value) const;

  // Description:
  // Get the length of an argument of an array type.  Returns whether
  // the argument is really an array type.
  int GetArgumentLength(int message, int argument, vtkTypeUInt32* length) const;

  // Description:
  // Get the given argument in the given message as an object of a
  // particular vtkObjectBase type.  Returns whether the argument is
  // really of the requested type.
  int GetArgumentObject(int message, int argument, vtkObjectBase** value,
                        const char* type) const;

  // Description:
  // Proxy-object returned by the two-argument form of GetArgument.
  // This is suitable to be stored in another stream.
  struct Argument
  {
    const unsigned char* Data;
    size_t Size;
  };

  // Description:
  // Get the given argument of the given message in a form that can be
  // sent to another stream.  Returns an empty argument if it either
  // index is out of range.
  vtkClientServerStream::Argument GetArgument(int message,
                                              int argument) const;

  // Description:
  // Get a pointer to the stream data and its length.  The values are
  // suitable for passing to another stream's SetData method, but are
  // invalidated when any further writing to the stream is done.
  // Returns whether the stream is currently valid.
  int GetData(const unsigned char** data, size_t* length) const;

  //--------------------------------------------------------------------------
  // Stream writing methods:

  // Description:
  // Proxy-object returned by InsertArray and used to insert
  // array data into the stream.
  struct Array
  {
    Types Type;
    vtkTypeUInt32 Length;
    vtkTypeUInt32 Size;
    const void* Data;
  };

  // Description:
  // Stream operators for special types.
  vtkClientServerStream& operator << (vtkClientServerStream::Commands);
  vtkClientServerStream& operator << (vtkClientServerStream::Types);
  vtkClientServerStream& operator << (vtkClientServerStream::Argument);
  vtkClientServerStream& operator << (vtkClientServerStream::Array);
  vtkClientServerStream& operator << (const vtkClientServerStream&);
  vtkClientServerStream& operator << (vtkClientServerID);
  vtkClientServerStream& operator << (vtkObjectBase*);

  // Description:
  // Stream operators for native types.
  vtkClientServerStream& operator << (char value);
  vtkClientServerStream& operator << (short value);
  vtkClientServerStream& operator << (int value);
  vtkClientServerStream& operator << (long value);
  vtkClientServerStream& operator << (signed char value);
  vtkClientServerStream& operator << (unsigned char value);
  vtkClientServerStream& operator << (unsigned short value);
  vtkClientServerStream& operator << (unsigned int value);
  vtkClientServerStream& operator << (unsigned long value);
#ifdef VTK_TYPE_INT64_NOT_STANDARD
  vtkClientServerStream& operator << (vtkTypeInt64 value);
  vtkClientServerStream& operator << (vtkTypeUInt64 value);
#endif
  vtkClientServerStream& operator << (float value);
  vtkClientServerStream& operator << (double value);
  vtkClientServerStream& operator << (const char *value);

  // Description:
  // Allow arrays to be passed into the stream.
  static vtkClientServerStream::Array InsertArray(const char*, int);
  static vtkClientServerStream::Array InsertArray(const short*, int);
  static vtkClientServerStream::Array InsertArray(const int*, int);
  static vtkClientServerStream::Array InsertArray(const long*, int);
  static vtkClientServerStream::Array InsertArray(const signed char*, int);
  static vtkClientServerStream::Array InsertArray(const unsigned char*, int);
  static vtkClientServerStream::Array InsertArray(const unsigned short*, int);
  static vtkClientServerStream::Array InsertArray(const unsigned int*, int);
  static vtkClientServerStream::Array InsertArray(const unsigned long*, int);
#ifdef VTK_TYPE_INT64_NOT_STANDARD
  static vtkClientServerStream::Array InsertArray(const vtkTypeInt64*, int);
  static vtkClientServerStream::Array InsertArray(const vtkTypeUInt64*, int);
#endif
  static vtkClientServerStream::Array InsertArray(const float*, int);
  static vtkClientServerStream::Array InsertArray(const double*, int);

  // Description:
  // Construct the entire stream from the given data.  This destroys
  // any data already in the stream.  Returns whether the stream is
  // deemed valid.  In the case of 0, the stream will have been reset.
  int SetData(const unsigned char* data, size_t length);

  //--------------------------------------------------------------------------
  // Utility methods:

  // Description:
  // Get a string describing the given type.  Returns "unknown" if the
  // type value is invalid.  If the type has multiple possible names,
  // the second argument can be used to specify the index of the name
  // to use.  The higher the index, the more shorthand the name.  If
  // the index is too high, the last name is used.
  static const char* GetStringFromType(vtkClientServerStream::Types type);
  static const char* GetStringFromType(vtkClientServerStream::Types type,
                                       int index);

  // Description:
  // Get the type named by the given string.  Returns
  // vtkClientServerStream::End if the type string is not recognized.
  static vtkClientServerStream::Types GetTypeFromString(const char* name);

  // Description:
  // Get a string describing the given command.  Returns "unknown" if
  // the command value is invalid.
  static const char* GetStringFromCommand(vtkClientServerStream::Commands cmd);

  // Description:
  // Get the command named by the given string.  Returns
  // vtkClientServerStream::EndOfCommands if the string is not
  // recognized.
  static
  vtkClientServerStream::Commands GetCommandFromString(const char* name);

  // Description:
  // Print the contents of the stream in a human-readable form.
  void Print(ostream&) const;
  void Print(ostream&, vtkIndent) const;
  void PrintMessage(ostream&, int message) const;
  void PrintMessage(ostream&, int message, vtkIndent) const;
  void PrintArgument(ostream&, int message, int argument) const;
  void PrintArgument(ostream&, int message, int argument, vtkIndent) const;
  void PrintArgumentValue(ostream&, int message, int argument) const;

  // Description:
  // Convert the stream to a string-based encoding.
  const char* StreamToString() const;
  void StreamToString(ostream& os) const;

  // Description:
  // Set the stream by parsing the given string.  The syntax of the
  // string must be that produced by the StreamToString method.
  // Returns 1 if the string is successfully parsed and 0 otherwise.
  int StreamFromString(const char* str);

protected:
  // Write arbitrary data to the stream.  Used internally.
  vtkClientServerStream& Write(const void* data, size_t length);

  // Data parsing utilities for SetData.
  int ParseData();
  unsigned char* ParseCommand(int order, unsigned char* data,
                              unsigned char* begin, unsigned char* end);
  void ParseEnd();
  unsigned char* ParseType(int order, unsigned char* data,
                           unsigned char* begin, unsigned char* end,
                           vtkClientServerStream::Types* type);
  unsigned char* ParseValue(int order, unsigned char* data,
                            unsigned char* end, unsigned int wordSize);
  unsigned char* ParseArray(int order, unsigned char* data,
                            unsigned char* end, unsigned int wordSize);
  unsigned char* ParseString(int order, unsigned char* data,
                             unsigned char* end);
  unsigned char* ParseStream(int order, unsigned char* data,
                             unsigned char* end);

  // Enumeration of possible byte orderings of data in the stream.
  enum { BigEndian, LittleEndian };

  // Byte swap data in the given byte order to match the current
  // machine's byte order.
  void PerformByteSwap(int dataByteOrder, unsigned char* data,
                       unsigned int numWords, unsigned int wordSize);

  // Get a pointer to the given value within the given message.
  // Returns 0 if either index is out of range.
  const unsigned char* GetValue(int message, int value) const;

  // Get the number of values in the given message.  The count
  // includes the Command and End portions of the message.  Returns 0
  // if the given index is out of range.
  int GetNumberOfValues(int message) const;

  // Internal implementation shared between PrintArgument and
  // PrintArgumentValue.
  void PrintArgumentInternal(ostream&, int message, int argument,
                             int annotate, vtkIndent) const;

  // String writing routines.
  void StreamToString(ostream& os, vtkIndent indent) const;
  void MessageToString(ostream& os, int m) const;
  void MessageToString(ostream& os, int m, vtkIndent indent) const;
  void ArgumentToString(ostream& os, int m, int a) const;
  void ArgumentToString(ostream& os, int m, int a, vtkIndent indent) const;
  void ArgumentValueToString(ostream& os, int m, int a,
                             vtkIndent indent) const;

  // Allow strings without null terminators to be passed into the stream.
  static vtkClientServerStream::Array InsertString(const char* begin,
                                                   const char* end);

  // String reading routines.
  static vtkClientServerStream::Types GetTypeFromString(const char* begin,
                                                        const char* end);
  static
  vtkClientServerStream::Commands GetCommandFromString(const char* begin,
                                                       const char* end);

  int StreamFromStringInternal(const char* begin, const char* end);
  int AddMessageFromString(const char* begin, const char* end,
                           const char** next);
  int AddArgumentFromString(const char* begin, const char* end,
                            const char** next);
private:
  vtkClientServerStreamInternals* Internal;
  friend class vtkClientServerStreamInternals;
};

// Description:
// Get the given argument of the given message as a pointer to a
// vtkObjectBase instance of a specific type.  Returns whether the
// argument was really of the requested type.
template <class T>
int
vtkClientServerStreamGetArgumentObject(const vtkClientServerStream& msg,
                                       int message, int argument,
                                       T** result, const char* type)
{
  vtkObjectBase* obj;
  if(msg.GetArgumentObject(message, argument, &obj, type))
    {
    *result = reinterpret_cast<T*>(obj);
    return 1;
    }
  return 0;
}

#endif
