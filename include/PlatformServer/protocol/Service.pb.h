// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Service.proto

#ifndef PROTOBUF_Service_2eproto__INCLUDED
#define PROTOBUF_Service_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace DataLock {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Service_2eproto();
void protobuf_AssignDesc_Service_2eproto();
void protobuf_ShutdownFile_Service_2eproto();

class ServiceJoinMessage;
class ServiceOutMessage;

// ===================================================================

class ServiceJoinMessage : public ::google::protobuf::Message {
 public:
  ServiceJoinMessage();
  virtual ~ServiceJoinMessage();

  ServiceJoinMessage(const ServiceJoinMessage& from);

  inline ServiceJoinMessage& operator=(const ServiceJoinMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ServiceJoinMessage& default_instance();

  void Swap(ServiceJoinMessage* other);

  // implements Message ----------------------------------------------

  ServiceJoinMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ServiceJoinMessage& from);
  void MergeFrom(const ServiceJoinMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 typeid = 1;
  inline bool has_typeid_() const;
  inline void clear_typeid_();
  static const int kTypeidFieldNumber = 1;
  inline ::google::protobuf::int32 typeid_() const;
  inline void set_typeid_(::google::protobuf::int32 value);

  // required string name = 2;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 2;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  inline void set_allocated_name(::std::string* name);

  // required int32 serial = 3;
  inline bool has_serial() const;
  inline void clear_serial();
  static const int kSerialFieldNumber = 3;
  inline ::google::protobuf::int32 serial() const;
  inline void set_serial(::google::protobuf::int32 value);

  // required int32 number = 4;
  inline bool has_number() const;
  inline void clear_number();
  static const int kNumberFieldNumber = 4;
  inline ::google::protobuf::int32 number() const;
  inline void set_number(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:DataLock.ServiceJoinMessage)
 private:
  inline void set_has_typeid_();
  inline void clear_has_typeid_();
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_serial();
  inline void clear_has_serial();
  inline void set_has_number();
  inline void clear_has_number();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* name_;
  ::google::protobuf::int32 typeid__;
  ::google::protobuf::int32 serial_;
  ::google::protobuf::int32 number_;
  friend void  protobuf_AddDesc_Service_2eproto();
  friend void protobuf_AssignDesc_Service_2eproto();
  friend void protobuf_ShutdownFile_Service_2eproto();

  void InitAsDefaultInstance();
  static ServiceJoinMessage* default_instance_;
};
// -------------------------------------------------------------------

class ServiceOutMessage : public ::google::protobuf::Message {
 public:
  ServiceOutMessage();
  virtual ~ServiceOutMessage();

  ServiceOutMessage(const ServiceOutMessage& from);

  inline ServiceOutMessage& operator=(const ServiceOutMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ServiceOutMessage& default_instance();

  void Swap(ServiceOutMessage* other);

  // implements Message ----------------------------------------------

  ServiceOutMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ServiceOutMessage& from);
  void MergeFrom(const ServiceOutMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 typeid = 1;
  inline bool has_typeid_() const;
  inline void clear_typeid_();
  static const int kTypeidFieldNumber = 1;
  inline ::google::protobuf::int32 typeid_() const;
  inline void set_typeid_(::google::protobuf::int32 value);

  // required int32 serial = 2;
  inline bool has_serial() const;
  inline void clear_serial();
  static const int kSerialFieldNumber = 2;
  inline ::google::protobuf::int32 serial() const;
  inline void set_serial(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:DataLock.ServiceOutMessage)
 private:
  inline void set_has_typeid_();
  inline void clear_has_typeid_();
  inline void set_has_serial();
  inline void clear_has_serial();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 typeid__;
  ::google::protobuf::int32 serial_;
  friend void  protobuf_AddDesc_Service_2eproto();
  friend void protobuf_AssignDesc_Service_2eproto();
  friend void protobuf_ShutdownFile_Service_2eproto();

  void InitAsDefaultInstance();
  static ServiceOutMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// ServiceJoinMessage

// required int32 typeid = 1;
inline bool ServiceJoinMessage::has_typeid_() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ServiceJoinMessage::set_has_typeid_() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ServiceJoinMessage::clear_has_typeid_() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ServiceJoinMessage::clear_typeid_() {
  typeid__ = 0;
  clear_has_typeid_();
}
inline ::google::protobuf::int32 ServiceJoinMessage::typeid_() const {
  // @@protoc_insertion_point(field_get:DataLock.ServiceJoinMessage.typeid)
  return typeid__;
}
inline void ServiceJoinMessage::set_typeid_(::google::protobuf::int32 value) {
  set_has_typeid_();
  typeid__ = value;
  // @@protoc_insertion_point(field_set:DataLock.ServiceJoinMessage.typeid)
}

// required string name = 2;
inline bool ServiceJoinMessage::has_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ServiceJoinMessage::set_has_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ServiceJoinMessage::clear_has_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ServiceJoinMessage::clear_name() {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& ServiceJoinMessage::name() const {
  // @@protoc_insertion_point(field_get:DataLock.ServiceJoinMessage.name)
  return *name_;
}
inline void ServiceJoinMessage::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set:DataLock.ServiceJoinMessage.name)
}
inline void ServiceJoinMessage::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(value);
  // @@protoc_insertion_point(field_set_char:DataLock.ServiceJoinMessage.name)
}
inline void ServiceJoinMessage::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:DataLock.ServiceJoinMessage.name)
}
inline ::std::string* ServiceJoinMessage::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    name_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:DataLock.ServiceJoinMessage.name)
  return name_;
}
inline ::std::string* ServiceJoinMessage::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void ServiceJoinMessage::set_allocated_name(::std::string* name) {
  if (name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete name_;
  }
  if (name) {
    set_has_name();
    name_ = name;
  } else {
    clear_has_name();
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:DataLock.ServiceJoinMessage.name)
}

// required int32 serial = 3;
inline bool ServiceJoinMessage::has_serial() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ServiceJoinMessage::set_has_serial() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ServiceJoinMessage::clear_has_serial() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ServiceJoinMessage::clear_serial() {
  serial_ = 0;
  clear_has_serial();
}
inline ::google::protobuf::int32 ServiceJoinMessage::serial() const {
  // @@protoc_insertion_point(field_get:DataLock.ServiceJoinMessage.serial)
  return serial_;
}
inline void ServiceJoinMessage::set_serial(::google::protobuf::int32 value) {
  set_has_serial();
  serial_ = value;
  // @@protoc_insertion_point(field_set:DataLock.ServiceJoinMessage.serial)
}

// required int32 number = 4;
inline bool ServiceJoinMessage::has_number() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ServiceJoinMessage::set_has_number() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ServiceJoinMessage::clear_has_number() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ServiceJoinMessage::clear_number() {
  number_ = 0;
  clear_has_number();
}
inline ::google::protobuf::int32 ServiceJoinMessage::number() const {
  // @@protoc_insertion_point(field_get:DataLock.ServiceJoinMessage.number)
  return number_;
}
inline void ServiceJoinMessage::set_number(::google::protobuf::int32 value) {
  set_has_number();
  number_ = value;
  // @@protoc_insertion_point(field_set:DataLock.ServiceJoinMessage.number)
}

// -------------------------------------------------------------------

// ServiceOutMessage

// required int32 typeid = 1;
inline bool ServiceOutMessage::has_typeid_() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ServiceOutMessage::set_has_typeid_() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ServiceOutMessage::clear_has_typeid_() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ServiceOutMessage::clear_typeid_() {
  typeid__ = 0;
  clear_has_typeid_();
}
inline ::google::protobuf::int32 ServiceOutMessage::typeid_() const {
  // @@protoc_insertion_point(field_get:DataLock.ServiceOutMessage.typeid)
  return typeid__;
}
inline void ServiceOutMessage::set_typeid_(::google::protobuf::int32 value) {
  set_has_typeid_();
  typeid__ = value;
  // @@protoc_insertion_point(field_set:DataLock.ServiceOutMessage.typeid)
}

// required int32 serial = 2;
inline bool ServiceOutMessage::has_serial() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ServiceOutMessage::set_has_serial() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ServiceOutMessage::clear_has_serial() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ServiceOutMessage::clear_serial() {
  serial_ = 0;
  clear_has_serial();
}
inline ::google::protobuf::int32 ServiceOutMessage::serial() const {
  // @@protoc_insertion_point(field_get:DataLock.ServiceOutMessage.serial)
  return serial_;
}
inline void ServiceOutMessage::set_serial(::google::protobuf::int32 value) {
  set_has_serial();
  serial_ = value;
  // @@protoc_insertion_point(field_set:DataLock.ServiceOutMessage.serial)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace DataLock

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Service_2eproto__INCLUDED