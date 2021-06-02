// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MessagePack.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "MessagePack.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace DataLock {

namespace {

const ::google::protobuf::Descriptor* MessagePack_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MessagePack_reflection_ = NULL;
const ::google::protobuf::Descriptor* ManagerNotifyMessage_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ManagerNotifyMessage_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_MessagePack_2eproto() {
  protobuf_AddDesc_MessagePack_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "MessagePack.proto");
  GOOGLE_CHECK(file != NULL);
  MessagePack_descriptor_ = file->message_type(0);
  static const int MessagePack_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessagePack, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessagePack, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessagePack, cmd_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessagePack, len_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessagePack, body_),
  };
  MessagePack_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MessagePack_descriptor_,
      MessagePack::default_instance_,
      MessagePack_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessagePack, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MessagePack, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MessagePack));
  ManagerNotifyMessage_descriptor_ = file->message_type(1);
  static const int ManagerNotifyMessage_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ManagerNotifyMessage, ids_),
  };
  ManagerNotifyMessage_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ManagerNotifyMessage_descriptor_,
      ManagerNotifyMessage::default_instance_,
      ManagerNotifyMessage_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ManagerNotifyMessage, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ManagerNotifyMessage, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ManagerNotifyMessage));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_MessagePack_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MessagePack_descriptor_, &MessagePack::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ManagerNotifyMessage_descriptor_, &ManagerNotifyMessage::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_MessagePack_2eproto() {
  delete MessagePack::default_instance_;
  delete MessagePack_reflection_;
  delete ManagerNotifyMessage::default_instance_;
  delete ManagerNotifyMessage_reflection_;
}

void protobuf_AddDesc_MessagePack_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\021MessagePack.proto\022\010DataLock\"O\n\013Message"
    "Pack\022\n\n\002id\030\001 \002(\t\022\014\n\004type\030\002 \002(\005\022\013\n\003cmd\030\003 "
    "\002(\005\022\013\n\003len\030\004 \002(\005\022\014\n\004body\030\005 \002(\014\"#\n\024Manage"
    "rNotifyMessage\022\013\n\003ids\030\001 \002(\tB%\n\020com.dlp.d"
    "atalockB\021MessagePackProtos", 186);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "MessagePack.proto", &protobuf_RegisterTypes);
  MessagePack::default_instance_ = new MessagePack();
  ManagerNotifyMessage::default_instance_ = new ManagerNotifyMessage();
  MessagePack::default_instance_->InitAsDefaultInstance();
  ManagerNotifyMessage::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_MessagePack_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_MessagePack_2eproto {
  StaticDescriptorInitializer_MessagePack_2eproto() {
    protobuf_AddDesc_MessagePack_2eproto();
  }
} static_descriptor_initializer_MessagePack_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int MessagePack::kIdFieldNumber;
const int MessagePack::kTypeFieldNumber;
const int MessagePack::kCmdFieldNumber;
const int MessagePack::kLenFieldNumber;
const int MessagePack::kBodyFieldNumber;
#endif  // !_MSC_VER

MessagePack::MessagePack()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:DataLock.MessagePack)
}

void MessagePack::InitAsDefaultInstance() {
}

MessagePack::MessagePack(const MessagePack& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:DataLock.MessagePack)
}

void MessagePack::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  id_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  type_ = 0;
  cmd_ = 0;
  len_ = 0;
  body_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MessagePack::~MessagePack() {
  // @@protoc_insertion_point(destructor:DataLock.MessagePack)
  SharedDtor();
}

void MessagePack::SharedDtor() {
  if (id_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete id_;
  }
  if (body_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete body_;
  }
  if (this != default_instance_) {
  }
}

void MessagePack::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MessagePack::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MessagePack_descriptor_;
}

const MessagePack& MessagePack::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MessagePack_2eproto();
  return *default_instance_;
}

MessagePack* MessagePack::default_instance_ = NULL;

MessagePack* MessagePack::New() const {
  return new MessagePack;
}

void MessagePack::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<MessagePack*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  if (_has_bits_[0 / 32] & 31) {
    ZR_(type_, cmd_);
    if (has_id()) {
      if (id_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        id_->clear();
      }
    }
    len_ = 0;
    if (has_body()) {
      if (body_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        body_->clear();
      }
    }
  }

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MessagePack::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:DataLock.MessagePack)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string id = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_id()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->id().data(), this->id().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "id");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_type;
        break;
      }

      // required int32 type = 2;
      case 2: {
        if (tag == 16) {
         parse_type:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &type_)));
          set_has_type();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_cmd;
        break;
      }

      // required int32 cmd = 3;
      case 3: {
        if (tag == 24) {
         parse_cmd:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &cmd_)));
          set_has_cmd();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(32)) goto parse_len;
        break;
      }

      // required int32 len = 4;
      case 4: {
        if (tag == 32) {
         parse_len:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &len_)));
          set_has_len();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(42)) goto parse_body;
        break;
      }

      // required bytes body = 5;
      case 5: {
        if (tag == 42) {
         parse_body:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_body()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:DataLock.MessagePack)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:DataLock.MessagePack)
  return false;
#undef DO_
}

void MessagePack::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:DataLock.MessagePack)
  // required string id = 1;
  if (has_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->id().data(), this->id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "id");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->id(), output);
  }

  // required int32 type = 2;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->type(), output);
  }

  // required int32 cmd = 3;
  if (has_cmd()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->cmd(), output);
  }

  // required int32 len = 4;
  if (has_len()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->len(), output);
  }

  // required bytes body = 5;
  if (has_body()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytesMaybeAliased(
      5, this->body(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:DataLock.MessagePack)
}

::google::protobuf::uint8* MessagePack::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:DataLock.MessagePack)
  // required string id = 1;
  if (has_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->id().data(), this->id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "id");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->id(), target);
  }

  // required int32 type = 2;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->type(), target);
  }

  // required int32 cmd = 3;
  if (has_cmd()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->cmd(), target);
  }

  // required int32 len = 4;
  if (has_len()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->len(), target);
  }

  // required bytes body = 5;
  if (has_body()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        5, this->body(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:DataLock.MessagePack)
  return target;
}

int MessagePack::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string id = 1;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->id());
    }

    // required int32 type = 2;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->type());
    }

    // required int32 cmd = 3;
    if (has_cmd()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->cmd());
    }

    // required int32 len = 4;
    if (has_len()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->len());
    }

    // required bytes body = 5;
    if (has_body()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->body());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void MessagePack::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MessagePack* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MessagePack*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MessagePack::MergeFrom(const MessagePack& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_id()) {
      set_id(from.id());
    }
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_cmd()) {
      set_cmd(from.cmd());
    }
    if (from.has_len()) {
      set_len(from.len());
    }
    if (from.has_body()) {
      set_body(from.body());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MessagePack::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MessagePack::CopyFrom(const MessagePack& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MessagePack::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;

  return true;
}

void MessagePack::Swap(MessagePack* other) {
  if (other != this) {
    std::swap(id_, other->id_);
    std::swap(type_, other->type_);
    std::swap(cmd_, other->cmd_);
    std::swap(len_, other->len_);
    std::swap(body_, other->body_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MessagePack::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MessagePack_descriptor_;
  metadata.reflection = MessagePack_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int ManagerNotifyMessage::kIdsFieldNumber;
#endif  // !_MSC_VER

ManagerNotifyMessage::ManagerNotifyMessage()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:DataLock.ManagerNotifyMessage)
}

void ManagerNotifyMessage::InitAsDefaultInstance() {
}

ManagerNotifyMessage::ManagerNotifyMessage(const ManagerNotifyMessage& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:DataLock.ManagerNotifyMessage)
}

void ManagerNotifyMessage::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  ids_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ManagerNotifyMessage::~ManagerNotifyMessage() {
  // @@protoc_insertion_point(destructor:DataLock.ManagerNotifyMessage)
  SharedDtor();
}

void ManagerNotifyMessage::SharedDtor() {
  if (ids_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete ids_;
  }
  if (this != default_instance_) {
  }
}

void ManagerNotifyMessage::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ManagerNotifyMessage::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ManagerNotifyMessage_descriptor_;
}

const ManagerNotifyMessage& ManagerNotifyMessage::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MessagePack_2eproto();
  return *default_instance_;
}

ManagerNotifyMessage* ManagerNotifyMessage::default_instance_ = NULL;

ManagerNotifyMessage* ManagerNotifyMessage::New() const {
  return new ManagerNotifyMessage;
}

void ManagerNotifyMessage::Clear() {
  if (has_ids()) {
    if (ids_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
      ids_->clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ManagerNotifyMessage::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:DataLock.ManagerNotifyMessage)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string ids = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ids()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->ids().data(), this->ids().length(),
            ::google::protobuf::internal::WireFormat::PARSE,
            "ids");
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:DataLock.ManagerNotifyMessage)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:DataLock.ManagerNotifyMessage)
  return false;
#undef DO_
}

void ManagerNotifyMessage::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:DataLock.ManagerNotifyMessage)
  // required string ids = 1;
  if (has_ids()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->ids().data(), this->ids().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "ids");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->ids(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:DataLock.ManagerNotifyMessage)
}

::google::protobuf::uint8* ManagerNotifyMessage::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:DataLock.ManagerNotifyMessage)
  // required string ids = 1;
  if (has_ids()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->ids().data(), this->ids().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "ids");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->ids(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:DataLock.ManagerNotifyMessage)
  return target;
}

int ManagerNotifyMessage::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string ids = 1;
    if (has_ids()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->ids());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ManagerNotifyMessage::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ManagerNotifyMessage* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ManagerNotifyMessage*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ManagerNotifyMessage::MergeFrom(const ManagerNotifyMessage& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ids()) {
      set_ids(from.ids());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ManagerNotifyMessage::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ManagerNotifyMessage::CopyFrom(const ManagerNotifyMessage& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ManagerNotifyMessage::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void ManagerNotifyMessage::Swap(ManagerNotifyMessage* other) {
  if (other != this) {
    std::swap(ids_, other->ids_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ManagerNotifyMessage::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ManagerNotifyMessage_descriptor_;
  metadata.reflection = ManagerNotifyMessage_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace DataLock

// @@protoc_insertion_point(global_scope)
