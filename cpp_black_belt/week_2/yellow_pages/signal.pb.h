// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: signal.proto

#ifndef PROTOBUF_INCLUDED_signal_2eproto
#define PROTOBUF_INCLUDED_signal_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3007000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3007001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "company.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_signal_2eproto

// Internal implementation detail -- do not use these members.
struct TableStruct_signal_2eproto {
  static const ::google::protobuf::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors_signal_2eproto();
namespace YellowPages {
class Signal;
class SignalDefaultTypeInternal;
extern SignalDefaultTypeInternal _Signal_default_instance_;
}  // namespace YellowPages
namespace google {
namespace protobuf {
template<> ::YellowPages::Signal* Arena::CreateMaybeMessage<::YellowPages::Signal>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace YellowPages {

// ===================================================================

class Signal :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:YellowPages.Signal) */ {
 public:
  Signal();
  virtual ~Signal();

  Signal(const Signal& from);

  inline Signal& operator=(const Signal& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Signal(Signal&& from) noexcept
    : Signal() {
    *this = ::std::move(from);
  }

  inline Signal& operator=(Signal&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor() {
    return default_instance().GetDescriptor();
  }
  static const Signal& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Signal* internal_default_instance() {
    return reinterpret_cast<const Signal*>(
               &_Signal_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Signal* other);
  friend void swap(Signal& a, Signal& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Signal* New() const final {
    return CreateMaybeMessage<Signal>(nullptr);
  }

  Signal* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Signal>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Signal& from);
  void MergeFrom(const Signal& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  static const char* _InternalParse(const char* begin, const char* end, void* object, ::google::protobuf::internal::ParseContext* ctx);
  ::google::protobuf::internal::ParseFunc _ParseFunc() const final { return _InternalParse; }
  #else
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Signal* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // .YellowPages.Company company = 3;
  bool has_company() const;
  void clear_company();
  static const int kCompanyFieldNumber = 3;
  const ::YellowPages::Company& company() const;
  ::YellowPages::Company* release_company();
  ::YellowPages::Company* mutable_company();
  void set_allocated_company(::YellowPages::Company* company);

  // uint64 provider_id = 1;
  void clear_provider_id();
  static const int kProviderIdFieldNumber = 1;
  ::google::protobuf::uint64 provider_id() const;
  void set_provider_id(::google::protobuf::uint64 value);

  // uint64 update_date = 2;
  void clear_update_date();
  static const int kUpdateDateFieldNumber = 2;
  ::google::protobuf::uint64 update_date() const;
  void set_update_date(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:YellowPages.Signal)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::YellowPages::Company* company_;
  ::google::protobuf::uint64 provider_id_;
  ::google::protobuf::uint64 update_date_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_signal_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Signal

// uint64 provider_id = 1;
inline void Signal::clear_provider_id() {
  provider_id_ = PROTOBUF_ULONGLONG(0);
}
inline ::google::protobuf::uint64 Signal::provider_id() const {
  // @@protoc_insertion_point(field_get:YellowPages.Signal.provider_id)
  return provider_id_;
}
inline void Signal::set_provider_id(::google::protobuf::uint64 value) {
  
  provider_id_ = value;
  // @@protoc_insertion_point(field_set:YellowPages.Signal.provider_id)
}

// uint64 update_date = 2;
inline void Signal::clear_update_date() {
  update_date_ = PROTOBUF_ULONGLONG(0);
}
inline ::google::protobuf::uint64 Signal::update_date() const {
  // @@protoc_insertion_point(field_get:YellowPages.Signal.update_date)
  return update_date_;
}
inline void Signal::set_update_date(::google::protobuf::uint64 value) {
  
  update_date_ = value;
  // @@protoc_insertion_point(field_set:YellowPages.Signal.update_date)
}

// .YellowPages.Company company = 3;
inline bool Signal::has_company() const {
  return this != internal_default_instance() && company_ != nullptr;
}
inline const ::YellowPages::Company& Signal::company() const {
  const ::YellowPages::Company* p = company_;
  // @@protoc_insertion_point(field_get:YellowPages.Signal.company)
  return p != nullptr ? *p : *reinterpret_cast<const ::YellowPages::Company*>(
      &::YellowPages::_Company_default_instance_);
}
inline ::YellowPages::Company* Signal::release_company() {
  // @@protoc_insertion_point(field_release:YellowPages.Signal.company)
  
  ::YellowPages::Company* temp = company_;
  company_ = nullptr;
  return temp;
}
inline ::YellowPages::Company* Signal::mutable_company() {
  
  if (company_ == nullptr) {
    auto* p = CreateMaybeMessage<::YellowPages::Company>(GetArenaNoVirtual());
    company_ = p;
  }
  // @@protoc_insertion_point(field_mutable:YellowPages.Signal.company)
  return company_;
}
inline void Signal::set_allocated_company(::YellowPages::Company* company) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(company_);
  }
  if (company) {
    ::google::protobuf::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      company = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, company, submessage_arena);
    }
    
  } else {
    
  }
  company_ = company;
  // @@protoc_insertion_point(field_set_allocated:YellowPages.Signal.company)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace YellowPages

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // PROTOBUF_INCLUDED_signal_2eproto
