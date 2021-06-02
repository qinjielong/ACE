// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Log.proto

#ifndef PROTOBUF_Log_2eproto__INCLUDED
#define PROTOBUF_Log_2eproto__INCLUDED

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
namespace Log {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Log_2eproto();
void protobuf_AssignDesc_Log_2eproto();
void protobuf_ShutdownFile_Log_2eproto();

class UserLog;
class ClientLog;

// ===================================================================

class UserLog : public ::google::protobuf::Message {
 public:
  UserLog();
  virtual ~UserLog();

  UserLog(const UserLog& from);

  inline UserLog& operator=(const UserLog& from) {
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
  static const UserLog& default_instance();

  void Swap(UserLog* other);

  // implements Message ----------------------------------------------

  UserLog* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const UserLog& from);
  void MergeFrom(const UserLog& from);
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

  // required int32 userid = 1;
  inline bool has_userid() const;
  inline void clear_userid();
  static const int kUseridFieldNumber = 1;
  inline ::google::protobuf::int32 userid() const;
  inline void set_userid(::google::protobuf::int32 value);

  // required string username = 2;
  inline bool has_username() const;
  inline void clear_username();
  static const int kUsernameFieldNumber = 2;
  inline const ::std::string& username() const;
  inline void set_username(const ::std::string& value);
  inline void set_username(const char* value);
  inline void set_username(const char* value, size_t size);
  inline ::std::string* mutable_username();
  inline ::std::string* release_username();
  inline void set_allocated_username(::std::string* username);

  // required int32 type = 3;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 3;
  inline ::google::protobuf::int32 type() const;
  inline void set_type(::google::protobuf::int32 value);

  // required int32 leve = 4;
  inline bool has_leve() const;
  inline void clear_leve();
  static const int kLeveFieldNumber = 4;
  inline ::google::protobuf::int32 leve() const;
  inline void set_leve(::google::protobuf::int32 value);

  // required string content = 5;
  inline bool has_content() const;
  inline void clear_content();
  static const int kContentFieldNumber = 5;
  inline const ::std::string& content() const;
  inline void set_content(const ::std::string& value);
  inline void set_content(const char* value);
  inline void set_content(const char* value, size_t size);
  inline ::std::string* mutable_content();
  inline ::std::string* release_content();
  inline void set_allocated_content(::std::string* content);

  // required string time = 6;
  inline bool has_time() const;
  inline void clear_time();
  static const int kTimeFieldNumber = 6;
  inline const ::std::string& time() const;
  inline void set_time(const ::std::string& value);
  inline void set_time(const char* value);
  inline void set_time(const char* value, size_t size);
  inline ::std::string* mutable_time();
  inline ::std::string* release_time();
  inline void set_allocated_time(::std::string* time);

  // @@protoc_insertion_point(class_scope:DataLock.Log.UserLog)
 private:
  inline void set_has_userid();
  inline void clear_has_userid();
  inline void set_has_username();
  inline void clear_has_username();
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_leve();
  inline void clear_has_leve();
  inline void set_has_content();
  inline void clear_has_content();
  inline void set_has_time();
  inline void clear_has_time();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* username_;
  ::google::protobuf::int32 userid_;
  ::google::protobuf::int32 type_;
  ::std::string* content_;
  ::std::string* time_;
  ::google::protobuf::int32 leve_;
  friend void  protobuf_AddDesc_Log_2eproto();
  friend void protobuf_AssignDesc_Log_2eproto();
  friend void protobuf_ShutdownFile_Log_2eproto();

  void InitAsDefaultInstance();
  static UserLog* default_instance_;
};
// -------------------------------------------------------------------

class ClientLog : public ::google::protobuf::Message {
 public:
  ClientLog();
  virtual ~ClientLog();

  ClientLog(const ClientLog& from);

  inline ClientLog& operator=(const ClientLog& from) {
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
  static const ClientLog& default_instance();

  void Swap(ClientLog* other);

  // implements Message ----------------------------------------------

  ClientLog* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ClientLog& from);
  void MergeFrom(const ClientLog& from);
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

  // required string deviceCode = 1;
  inline bool has_devicecode() const;
  inline void clear_devicecode();
  static const int kDeviceCodeFieldNumber = 1;
  inline const ::std::string& devicecode() const;
  inline void set_devicecode(const ::std::string& value);
  inline void set_devicecode(const char* value);
  inline void set_devicecode(const char* value, size_t size);
  inline ::std::string* mutable_devicecode();
  inline ::std::string* release_devicecode();
  inline void set_allocated_devicecode(::std::string* devicecode);

  // required string deviceName = 2;
  inline bool has_devicename() const;
  inline void clear_devicename();
  static const int kDeviceNameFieldNumber = 2;
  inline const ::std::string& devicename() const;
  inline void set_devicename(const ::std::string& value);
  inline void set_devicename(const char* value);
  inline void set_devicename(const char* value, size_t size);
  inline ::std::string* mutable_devicename();
  inline ::std::string* release_devicename();
  inline void set_allocated_devicename(::std::string* devicename);

  // required int32 type = 3;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 3;
  inline ::google::protobuf::int32 type() const;
  inline void set_type(::google::protobuf::int32 value);

  // required int32 leve = 4;
  inline bool has_leve() const;
  inline void clear_leve();
  static const int kLeveFieldNumber = 4;
  inline ::google::protobuf::int32 leve() const;
  inline void set_leve(::google::protobuf::int32 value);

  // required string content = 5;
  inline bool has_content() const;
  inline void clear_content();
  static const int kContentFieldNumber = 5;
  inline const ::std::string& content() const;
  inline void set_content(const ::std::string& value);
  inline void set_content(const char* value);
  inline void set_content(const char* value, size_t size);
  inline ::std::string* mutable_content();
  inline ::std::string* release_content();
  inline void set_allocated_content(::std::string* content);

  // required string time = 6;
  inline bool has_time() const;
  inline void clear_time();
  static const int kTimeFieldNumber = 6;
  inline const ::std::string& time() const;
  inline void set_time(const ::std::string& value);
  inline void set_time(const char* value);
  inline void set_time(const char* value, size_t size);
  inline ::std::string* mutable_time();
  inline ::std::string* release_time();
  inline void set_allocated_time(::std::string* time);

  // @@protoc_insertion_point(class_scope:DataLock.Log.ClientLog)
 private:
  inline void set_has_devicecode();
  inline void clear_has_devicecode();
  inline void set_has_devicename();
  inline void clear_has_devicename();
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_leve();
  inline void clear_has_leve();
  inline void set_has_content();
  inline void clear_has_content();
  inline void set_has_time();
  inline void clear_has_time();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* devicecode_;
  ::std::string* devicename_;
  ::google::protobuf::int32 type_;
  ::google::protobuf::int32 leve_;
  ::std::string* content_;
  ::std::string* time_;
  friend void  protobuf_AddDesc_Log_2eproto();
  friend void protobuf_AssignDesc_Log_2eproto();
  friend void protobuf_ShutdownFile_Log_2eproto();

  void InitAsDefaultInstance();
  static ClientLog* default_instance_;
};
// ===================================================================


// ===================================================================

// UserLog

// required int32 userid = 1;
inline bool UserLog::has_userid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void UserLog::set_has_userid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void UserLog::clear_has_userid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void UserLog::clear_userid() {
  userid_ = 0;
  clear_has_userid();
}
inline ::google::protobuf::int32 UserLog::userid() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.UserLog.userid)
  return userid_;
}
inline void UserLog::set_userid(::google::protobuf::int32 value) {
  set_has_userid();
  userid_ = value;
  // @@protoc_insertion_point(field_set:DataLock.Log.UserLog.userid)
}

// required string username = 2;
inline bool UserLog::has_username() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void UserLog::set_has_username() {
  _has_bits_[0] |= 0x00000002u;
}
inline void UserLog::clear_has_username() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void UserLog::clear_username() {
  if (username_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_->clear();
  }
  clear_has_username();
}
inline const ::std::string& UserLog::username() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.UserLog.username)
  return *username_;
}
inline void UserLog::set_username(const ::std::string& value) {
  set_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_ = new ::std::string;
  }
  username_->assign(value);
  // @@protoc_insertion_point(field_set:DataLock.Log.UserLog.username)
}
inline void UserLog::set_username(const char* value) {
  set_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_ = new ::std::string;
  }
  username_->assign(value);
  // @@protoc_insertion_point(field_set_char:DataLock.Log.UserLog.username)
}
inline void UserLog::set_username(const char* value, size_t size) {
  set_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_ = new ::std::string;
  }
  username_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:DataLock.Log.UserLog.username)
}
inline ::std::string* UserLog::mutable_username() {
  set_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    username_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:DataLock.Log.UserLog.username)
  return username_;
}
inline ::std::string* UserLog::release_username() {
  clear_has_username();
  if (username_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = username_;
    username_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void UserLog::set_allocated_username(::std::string* username) {
  if (username_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete username_;
  }
  if (username) {
    set_has_username();
    username_ = username;
  } else {
    clear_has_username();
    username_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:DataLock.Log.UserLog.username)
}

// required int32 type = 3;
inline bool UserLog::has_type() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void UserLog::set_has_type() {
  _has_bits_[0] |= 0x00000004u;
}
inline void UserLog::clear_has_type() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void UserLog::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::google::protobuf::int32 UserLog::type() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.UserLog.type)
  return type_;
}
inline void UserLog::set_type(::google::protobuf::int32 value) {
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:DataLock.Log.UserLog.type)
}

// required int32 leve = 4;
inline bool UserLog::has_leve() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void UserLog::set_has_leve() {
  _has_bits_[0] |= 0x00000008u;
}
inline void UserLog::clear_has_leve() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void UserLog::clear_leve() {
  leve_ = 0;
  clear_has_leve();
}
inline ::google::protobuf::int32 UserLog::leve() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.UserLog.leve)
  return leve_;
}
inline void UserLog::set_leve(::google::protobuf::int32 value) {
  set_has_leve();
  leve_ = value;
  // @@protoc_insertion_point(field_set:DataLock.Log.UserLog.leve)
}

// required string content = 5;
inline bool UserLog::has_content() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void UserLog::set_has_content() {
  _has_bits_[0] |= 0x00000010u;
}
inline void UserLog::clear_has_content() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void UserLog::clear_content() {
  if (content_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_->clear();
  }
  clear_has_content();
}
inline const ::std::string& UserLog::content() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.UserLog.content)
  return *content_;
}
inline void UserLog::set_content(const ::std::string& value) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  content_->assign(value);
  // @@protoc_insertion_point(field_set:DataLock.Log.UserLog.content)
}
inline void UserLog::set_content(const char* value) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  content_->assign(value);
  // @@protoc_insertion_point(field_set_char:DataLock.Log.UserLog.content)
}
inline void UserLog::set_content(const char* value, size_t size) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  content_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:DataLock.Log.UserLog.content)
}
inline ::std::string* UserLog::mutable_content() {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:DataLock.Log.UserLog.content)
  return content_;
}
inline ::std::string* UserLog::release_content() {
  clear_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = content_;
    content_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void UserLog::set_allocated_content(::std::string* content) {
  if (content_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete content_;
  }
  if (content) {
    set_has_content();
    content_ = content;
  } else {
    clear_has_content();
    content_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:DataLock.Log.UserLog.content)
}

// required string time = 6;
inline bool UserLog::has_time() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void UserLog::set_has_time() {
  _has_bits_[0] |= 0x00000020u;
}
inline void UserLog::clear_has_time() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void UserLog::clear_time() {
  if (time_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_->clear();
  }
  clear_has_time();
}
inline const ::std::string& UserLog::time() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.UserLog.time)
  return *time_;
}
inline void UserLog::set_time(const ::std::string& value) {
  set_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_ = new ::std::string;
  }
  time_->assign(value);
  // @@protoc_insertion_point(field_set:DataLock.Log.UserLog.time)
}
inline void UserLog::set_time(const char* value) {
  set_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_ = new ::std::string;
  }
  time_->assign(value);
  // @@protoc_insertion_point(field_set_char:DataLock.Log.UserLog.time)
}
inline void UserLog::set_time(const char* value, size_t size) {
  set_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_ = new ::std::string;
  }
  time_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:DataLock.Log.UserLog.time)
}
inline ::std::string* UserLog::mutable_time() {
  set_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:DataLock.Log.UserLog.time)
  return time_;
}
inline ::std::string* UserLog::release_time() {
  clear_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = time_;
    time_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void UserLog::set_allocated_time(::std::string* time) {
  if (time_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete time_;
  }
  if (time) {
    set_has_time();
    time_ = time;
  } else {
    clear_has_time();
    time_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:DataLock.Log.UserLog.time)
}

// -------------------------------------------------------------------

// ClientLog

// required string deviceCode = 1;
inline bool ClientLog::has_devicecode() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ClientLog::set_has_devicecode() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ClientLog::clear_has_devicecode() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ClientLog::clear_devicecode() {
  if (devicecode_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicecode_->clear();
  }
  clear_has_devicecode();
}
inline const ::std::string& ClientLog::devicecode() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.ClientLog.deviceCode)
  return *devicecode_;
}
inline void ClientLog::set_devicecode(const ::std::string& value) {
  set_has_devicecode();
  if (devicecode_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicecode_ = new ::std::string;
  }
  devicecode_->assign(value);
  // @@protoc_insertion_point(field_set:DataLock.Log.ClientLog.deviceCode)
}
inline void ClientLog::set_devicecode(const char* value) {
  set_has_devicecode();
  if (devicecode_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicecode_ = new ::std::string;
  }
  devicecode_->assign(value);
  // @@protoc_insertion_point(field_set_char:DataLock.Log.ClientLog.deviceCode)
}
inline void ClientLog::set_devicecode(const char* value, size_t size) {
  set_has_devicecode();
  if (devicecode_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicecode_ = new ::std::string;
  }
  devicecode_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:DataLock.Log.ClientLog.deviceCode)
}
inline ::std::string* ClientLog::mutable_devicecode() {
  set_has_devicecode();
  if (devicecode_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicecode_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:DataLock.Log.ClientLog.deviceCode)
  return devicecode_;
}
inline ::std::string* ClientLog::release_devicecode() {
  clear_has_devicecode();
  if (devicecode_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = devicecode_;
    devicecode_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void ClientLog::set_allocated_devicecode(::std::string* devicecode) {
  if (devicecode_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete devicecode_;
  }
  if (devicecode) {
    set_has_devicecode();
    devicecode_ = devicecode;
  } else {
    clear_has_devicecode();
    devicecode_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:DataLock.Log.ClientLog.deviceCode)
}

// required string deviceName = 2;
inline bool ClientLog::has_devicename() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ClientLog::set_has_devicename() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ClientLog::clear_has_devicename() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ClientLog::clear_devicename() {
  if (devicename_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicename_->clear();
  }
  clear_has_devicename();
}
inline const ::std::string& ClientLog::devicename() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.ClientLog.deviceName)
  return *devicename_;
}
inline void ClientLog::set_devicename(const ::std::string& value) {
  set_has_devicename();
  if (devicename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicename_ = new ::std::string;
  }
  devicename_->assign(value);
  // @@protoc_insertion_point(field_set:DataLock.Log.ClientLog.deviceName)
}
inline void ClientLog::set_devicename(const char* value) {
  set_has_devicename();
  if (devicename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicename_ = new ::std::string;
  }
  devicename_->assign(value);
  // @@protoc_insertion_point(field_set_char:DataLock.Log.ClientLog.deviceName)
}
inline void ClientLog::set_devicename(const char* value, size_t size) {
  set_has_devicename();
  if (devicename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicename_ = new ::std::string;
  }
  devicename_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:DataLock.Log.ClientLog.deviceName)
}
inline ::std::string* ClientLog::mutable_devicename() {
  set_has_devicename();
  if (devicename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    devicename_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:DataLock.Log.ClientLog.deviceName)
  return devicename_;
}
inline ::std::string* ClientLog::release_devicename() {
  clear_has_devicename();
  if (devicename_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = devicename_;
    devicename_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void ClientLog::set_allocated_devicename(::std::string* devicename) {
  if (devicename_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete devicename_;
  }
  if (devicename) {
    set_has_devicename();
    devicename_ = devicename;
  } else {
    clear_has_devicename();
    devicename_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:DataLock.Log.ClientLog.deviceName)
}

// required int32 type = 3;
inline bool ClientLog::has_type() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ClientLog::set_has_type() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ClientLog::clear_has_type() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ClientLog::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::google::protobuf::int32 ClientLog::type() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.ClientLog.type)
  return type_;
}
inline void ClientLog::set_type(::google::protobuf::int32 value) {
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:DataLock.Log.ClientLog.type)
}

// required int32 leve = 4;
inline bool ClientLog::has_leve() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ClientLog::set_has_leve() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ClientLog::clear_has_leve() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ClientLog::clear_leve() {
  leve_ = 0;
  clear_has_leve();
}
inline ::google::protobuf::int32 ClientLog::leve() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.ClientLog.leve)
  return leve_;
}
inline void ClientLog::set_leve(::google::protobuf::int32 value) {
  set_has_leve();
  leve_ = value;
  // @@protoc_insertion_point(field_set:DataLock.Log.ClientLog.leve)
}

// required string content = 5;
inline bool ClientLog::has_content() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void ClientLog::set_has_content() {
  _has_bits_[0] |= 0x00000010u;
}
inline void ClientLog::clear_has_content() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void ClientLog::clear_content() {
  if (content_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_->clear();
  }
  clear_has_content();
}
inline const ::std::string& ClientLog::content() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.ClientLog.content)
  return *content_;
}
inline void ClientLog::set_content(const ::std::string& value) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  content_->assign(value);
  // @@protoc_insertion_point(field_set:DataLock.Log.ClientLog.content)
}
inline void ClientLog::set_content(const char* value) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  content_->assign(value);
  // @@protoc_insertion_point(field_set_char:DataLock.Log.ClientLog.content)
}
inline void ClientLog::set_content(const char* value, size_t size) {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  content_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:DataLock.Log.ClientLog.content)
}
inline ::std::string* ClientLog::mutable_content() {
  set_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    content_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:DataLock.Log.ClientLog.content)
  return content_;
}
inline ::std::string* ClientLog::release_content() {
  clear_has_content();
  if (content_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = content_;
    content_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void ClientLog::set_allocated_content(::std::string* content) {
  if (content_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete content_;
  }
  if (content) {
    set_has_content();
    content_ = content;
  } else {
    clear_has_content();
    content_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:DataLock.Log.ClientLog.content)
}

// required string time = 6;
inline bool ClientLog::has_time() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void ClientLog::set_has_time() {
  _has_bits_[0] |= 0x00000020u;
}
inline void ClientLog::clear_has_time() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void ClientLog::clear_time() {
  if (time_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_->clear();
  }
  clear_has_time();
}
inline const ::std::string& ClientLog::time() const {
  // @@protoc_insertion_point(field_get:DataLock.Log.ClientLog.time)
  return *time_;
}
inline void ClientLog::set_time(const ::std::string& value) {
  set_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_ = new ::std::string;
  }
  time_->assign(value);
  // @@protoc_insertion_point(field_set:DataLock.Log.ClientLog.time)
}
inline void ClientLog::set_time(const char* value) {
  set_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_ = new ::std::string;
  }
  time_->assign(value);
  // @@protoc_insertion_point(field_set_char:DataLock.Log.ClientLog.time)
}
inline void ClientLog::set_time(const char* value, size_t size) {
  set_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_ = new ::std::string;
  }
  time_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:DataLock.Log.ClientLog.time)
}
inline ::std::string* ClientLog::mutable_time() {
  set_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    time_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:DataLock.Log.ClientLog.time)
  return time_;
}
inline ::std::string* ClientLog::release_time() {
  clear_has_time();
  if (time_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = time_;
    time_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void ClientLog::set_allocated_time(::std::string* time) {
  if (time_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete time_;
  }
  if (time) {
    set_has_time();
    time_ = time;
  } else {
    clear_has_time();
    time_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:DataLock.Log.ClientLog.time)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Log
}  // namespace DataLock

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Log_2eproto__INCLUDED
