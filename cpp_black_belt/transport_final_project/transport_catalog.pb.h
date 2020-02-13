// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: transport_catalog.proto

#ifndef PROTOBUF_INCLUDED_transport_5fcatalog_2eproto
#define PROTOBUF_INCLUDED_transport_5fcatalog_2eproto

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
#include "database.pb.h"
#include "router.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_transport_5fcatalog_2eproto

// Internal implementation detail -- do not use these members.
struct TableStruct_transport_5fcatalog_2eproto {
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
void AddDescriptors_transport_5fcatalog_2eproto();
namespace ProtoTransport {
class TransportCatalog;
class TransportCatalogDefaultTypeInternal;
extern TransportCatalogDefaultTypeInternal _TransportCatalog_default_instance_;
}  // namespace ProtoTransport
namespace google {
namespace protobuf {
template<> ::ProtoTransport::TransportCatalog* Arena::CreateMaybeMessage<::ProtoTransport::TransportCatalog>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace ProtoTransport {

// ===================================================================

class TransportCatalog :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ProtoTransport.TransportCatalog) */ {
 public:
  TransportCatalog();
  virtual ~TransportCatalog();

  TransportCatalog(const TransportCatalog& from);

  inline TransportCatalog& operator=(const TransportCatalog& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  TransportCatalog(TransportCatalog&& from) noexcept
    : TransportCatalog() {
    *this = ::std::move(from);
  }

  inline TransportCatalog& operator=(TransportCatalog&& from) noexcept {
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
  static const TransportCatalog& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const TransportCatalog* internal_default_instance() {
    return reinterpret_cast<const TransportCatalog*>(
               &_TransportCatalog_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(TransportCatalog* other);
  friend void swap(TransportCatalog& a, TransportCatalog& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline TransportCatalog* New() const final {
    return CreateMaybeMessage<TransportCatalog>(nullptr);
  }

  TransportCatalog* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<TransportCatalog>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const TransportCatalog& from);
  void MergeFrom(const TransportCatalog& from);
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
  void InternalSwap(TransportCatalog* other);
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

  // .ProtoTransport.Database db = 1;
  bool has_db() const;
  void clear_db();
  static const int kDbFieldNumber = 1;
  const ::ProtoTransport::Database& db() const;
  ::ProtoTransport::Database* release_db();
  ::ProtoTransport::Database* mutable_db();
  void set_allocated_db(::ProtoTransport::Database* db);

  // .ProtoTransport.Router router = 2;
  bool has_router() const;
  void clear_router();
  static const int kRouterFieldNumber = 2;
  const ::ProtoTransport::Router& router() const;
  ::ProtoTransport::Router* release_router();
  ::ProtoTransport::Router* mutable_router();
  void set_allocated_router(::ProtoTransport::Router* router);

  // @@protoc_insertion_point(class_scope:ProtoTransport.TransportCatalog)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::ProtoTransport::Database* db_;
  ::ProtoTransport::Router* router_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_transport_5fcatalog_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// TransportCatalog

// .ProtoTransport.Database db = 1;
inline bool TransportCatalog::has_db() const {
  return this != internal_default_instance() && db_ != nullptr;
}
inline const ::ProtoTransport::Database& TransportCatalog::db() const {
  const ::ProtoTransport::Database* p = db_;
  // @@protoc_insertion_point(field_get:ProtoTransport.TransportCatalog.db)
  return p != nullptr ? *p : *reinterpret_cast<const ::ProtoTransport::Database*>(
      &::ProtoTransport::_Database_default_instance_);
}
inline ::ProtoTransport::Database* TransportCatalog::release_db() {
  // @@protoc_insertion_point(field_release:ProtoTransport.TransportCatalog.db)
  
  ::ProtoTransport::Database* temp = db_;
  db_ = nullptr;
  return temp;
}
inline ::ProtoTransport::Database* TransportCatalog::mutable_db() {
  
  if (db_ == nullptr) {
    auto* p = CreateMaybeMessage<::ProtoTransport::Database>(GetArenaNoVirtual());
    db_ = p;
  }
  // @@protoc_insertion_point(field_mutable:ProtoTransport.TransportCatalog.db)
  return db_;
}
inline void TransportCatalog::set_allocated_db(::ProtoTransport::Database* db) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(db_);
  }
  if (db) {
    ::google::protobuf::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      db = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, db, submessage_arena);
    }
    
  } else {
    
  }
  db_ = db;
  // @@protoc_insertion_point(field_set_allocated:ProtoTransport.TransportCatalog.db)
}

// .ProtoTransport.Router router = 2;
inline bool TransportCatalog::has_router() const {
  return this != internal_default_instance() && router_ != nullptr;
}
inline const ::ProtoTransport::Router& TransportCatalog::router() const {
  const ::ProtoTransport::Router* p = router_;
  // @@protoc_insertion_point(field_get:ProtoTransport.TransportCatalog.router)
  return p != nullptr ? *p : *reinterpret_cast<const ::ProtoTransport::Router*>(
      &::ProtoTransport::_Router_default_instance_);
}
inline ::ProtoTransport::Router* TransportCatalog::release_router() {
  // @@protoc_insertion_point(field_release:ProtoTransport.TransportCatalog.router)
  
  ::ProtoTransport::Router* temp = router_;
  router_ = nullptr;
  return temp;
}
inline ::ProtoTransport::Router* TransportCatalog::mutable_router() {
  
  if (router_ == nullptr) {
    auto* p = CreateMaybeMessage<::ProtoTransport::Router>(GetArenaNoVirtual());
    router_ = p;
  }
  // @@protoc_insertion_point(field_mutable:ProtoTransport.TransportCatalog.router)
  return router_;
}
inline void TransportCatalog::set_allocated_router(::ProtoTransport::Router* router) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(router_);
  }
  if (router) {
    ::google::protobuf::Arena* submessage_arena = nullptr;
    if (message_arena != submessage_arena) {
      router = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, router, submessage_arena);
    }
    
  } else {
    
  }
  router_ = router;
  // @@protoc_insertion_point(field_set_allocated:ProtoTransport.TransportCatalog.router)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace ProtoTransport

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // PROTOBUF_INCLUDED_transport_5fcatalog_2eproto
