// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: database.proto

#ifndef PROTOBUF_INCLUDED_database_2eproto
#define PROTOBUF_INCLUDED_database_2eproto

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
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_database_2eproto

// Internal implementation detail -- do not use these members.
struct TableStruct_database_2eproto {
  static const ::google::protobuf::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors_database_2eproto();
namespace ProtoTransport {
class Bus;
class BusDefaultTypeInternal;
extern BusDefaultTypeInternal _Bus_default_instance_;
class Database;
class DatabaseDefaultTypeInternal;
extern DatabaseDefaultTypeInternal _Database_default_instance_;
class Stop;
class StopDefaultTypeInternal;
extern StopDefaultTypeInternal _Stop_default_instance_;
}  // namespace ProtoTransport
namespace google {
namespace protobuf {
template<> ::ProtoTransport::Bus* Arena::CreateMaybeMessage<::ProtoTransport::Bus>(Arena*);
template<> ::ProtoTransport::Database* Arena::CreateMaybeMessage<::ProtoTransport::Database>(Arena*);
template<> ::ProtoTransport::Stop* Arena::CreateMaybeMessage<::ProtoTransport::Stop>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace ProtoTransport {

// ===================================================================

class Stop :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ProtoTransport.Stop) */ {
 public:
  Stop();
  virtual ~Stop();

  Stop(const Stop& from);

  inline Stop& operator=(const Stop& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Stop(Stop&& from) noexcept
    : Stop() {
    *this = ::std::move(from);
  }

  inline Stop& operator=(Stop&& from) noexcept {
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
  static const Stop& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Stop* internal_default_instance() {
    return reinterpret_cast<const Stop*>(
               &_Stop_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Stop* other);
  friend void swap(Stop& a, Stop& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Stop* New() const final {
    return CreateMaybeMessage<Stop>(nullptr);
  }

  Stop* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Stop>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Stop& from);
  void MergeFrom(const Stop& from);
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
  void InternalSwap(Stop* other);
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

  // repeated string buses = 1;
  int buses_size() const;
  void clear_buses();
  static const int kBusesFieldNumber = 1;
  const ::std::string& buses(int index) const;
  ::std::string* mutable_buses(int index);
  void set_buses(int index, const ::std::string& value);
  #if LANG_CXX11
  void set_buses(int index, ::std::string&& value);
  #endif
  void set_buses(int index, const char* value);
  void set_buses(int index, const char* value, size_t size);
  ::std::string* add_buses();
  void add_buses(const ::std::string& value);
  #if LANG_CXX11
  void add_buses(::std::string&& value);
  #endif
  void add_buses(const char* value);
  void add_buses(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField<::std::string>& buses() const;
  ::google::protobuf::RepeatedPtrField<::std::string>* mutable_buses();

  // @@protoc_insertion_point(class_scope:ProtoTransport.Stop)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField<::std::string> buses_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_database_2eproto;
};
// -------------------------------------------------------------------

class Bus :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ProtoTransport.Bus) */ {
 public:
  Bus();
  virtual ~Bus();

  Bus(const Bus& from);

  inline Bus& operator=(const Bus& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Bus(Bus&& from) noexcept
    : Bus() {
    *this = ::std::move(from);
  }

  inline Bus& operator=(Bus&& from) noexcept {
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
  static const Bus& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Bus* internal_default_instance() {
    return reinterpret_cast<const Bus*>(
               &_Bus_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Bus* other);
  friend void swap(Bus& a, Bus& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Bus* New() const final {
    return CreateMaybeMessage<Bus>(nullptr);
  }

  Bus* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Bus>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Bus& from);
  void MergeFrom(const Bus& from);
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
  void InternalSwap(Bus* other);
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

  // uint32 stops = 1;
  void clear_stops();
  static const int kStopsFieldNumber = 1;
  ::google::protobuf::uint32 stops() const;
  void set_stops(::google::protobuf::uint32 value);

  // uint32 unique_stops = 2;
  void clear_unique_stops();
  static const int kUniqueStopsFieldNumber = 2;
  ::google::protobuf::uint32 unique_stops() const;
  void set_unique_stops(::google::protobuf::uint32 value);

  // double linear_route = 4;
  void clear_linear_route();
  static const int kLinearRouteFieldNumber = 4;
  double linear_route() const;
  void set_linear_route(double value);

  // uint32 road_route = 3;
  void clear_road_route();
  static const int kRoadRouteFieldNumber = 3;
  ::google::protobuf::uint32 road_route() const;
  void set_road_route(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:ProtoTransport.Bus)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 stops_;
  ::google::protobuf::uint32 unique_stops_;
  double linear_route_;
  ::google::protobuf::uint32 road_route_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_database_2eproto;
};
// -------------------------------------------------------------------

class Database :
    public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ProtoTransport.Database) */ {
 public:
  Database();
  virtual ~Database();

  Database(const Database& from);

  inline Database& operator=(const Database& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Database(Database&& from) noexcept
    : Database() {
    *this = ::std::move(from);
  }

  inline Database& operator=(Database&& from) noexcept {
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
  static const Database& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Database* internal_default_instance() {
    return reinterpret_cast<const Database*>(
               &_Database_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  void Swap(Database* other);
  friend void swap(Database& a, Database& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Database* New() const final {
    return CreateMaybeMessage<Database>(nullptr);
  }

  Database* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Database>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Database& from);
  void MergeFrom(const Database& from);
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
  void InternalSwap(Database* other);
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

  // repeated string stop_names = 1;
  int stop_names_size() const;
  void clear_stop_names();
  static const int kStopNamesFieldNumber = 1;
  const ::std::string& stop_names(int index) const;
  ::std::string* mutable_stop_names(int index);
  void set_stop_names(int index, const ::std::string& value);
  #if LANG_CXX11
  void set_stop_names(int index, ::std::string&& value);
  #endif
  void set_stop_names(int index, const char* value);
  void set_stop_names(int index, const char* value, size_t size);
  ::std::string* add_stop_names();
  void add_stop_names(const ::std::string& value);
  #if LANG_CXX11
  void add_stop_names(::std::string&& value);
  #endif
  void add_stop_names(const char* value);
  void add_stop_names(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField<::std::string>& stop_names() const;
  ::google::protobuf::RepeatedPtrField<::std::string>* mutable_stop_names();

  // repeated .ProtoTransport.Stop stops = 2;
  int stops_size() const;
  void clear_stops();
  static const int kStopsFieldNumber = 2;
  ::ProtoTransport::Stop* mutable_stops(int index);
  ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Stop >*
      mutable_stops();
  const ::ProtoTransport::Stop& stops(int index) const;
  ::ProtoTransport::Stop* add_stops();
  const ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Stop >&
      stops() const;

  // repeated string bus_names = 3;
  int bus_names_size() const;
  void clear_bus_names();
  static const int kBusNamesFieldNumber = 3;
  const ::std::string& bus_names(int index) const;
  ::std::string* mutable_bus_names(int index);
  void set_bus_names(int index, const ::std::string& value);
  #if LANG_CXX11
  void set_bus_names(int index, ::std::string&& value);
  #endif
  void set_bus_names(int index, const char* value);
  void set_bus_names(int index, const char* value, size_t size);
  ::std::string* add_bus_names();
  void add_bus_names(const ::std::string& value);
  #if LANG_CXX11
  void add_bus_names(::std::string&& value);
  #endif
  void add_bus_names(const char* value);
  void add_bus_names(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField<::std::string>& bus_names() const;
  ::google::protobuf::RepeatedPtrField<::std::string>* mutable_bus_names();

  // repeated .ProtoTransport.Bus buses = 4;
  int buses_size() const;
  void clear_buses();
  static const int kBusesFieldNumber = 4;
  ::ProtoTransport::Bus* mutable_buses(int index);
  ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Bus >*
      mutable_buses();
  const ::ProtoTransport::Bus& buses(int index) const;
  ::ProtoTransport::Bus* add_buses();
  const ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Bus >&
      buses() const;

  // @@protoc_insertion_point(class_scope:ProtoTransport.Database)
 private:
  class HasBitSetters;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField<::std::string> stop_names_;
  ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Stop > stops_;
  ::google::protobuf::RepeatedPtrField<::std::string> bus_names_;
  ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Bus > buses_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_database_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Stop

// repeated string buses = 1;
inline int Stop::buses_size() const {
  return buses_.size();
}
inline void Stop::clear_buses() {
  buses_.Clear();
}
inline const ::std::string& Stop::buses(int index) const {
  // @@protoc_insertion_point(field_get:ProtoTransport.Stop.buses)
  return buses_.Get(index);
}
inline ::std::string* Stop::mutable_buses(int index) {
  // @@protoc_insertion_point(field_mutable:ProtoTransport.Stop.buses)
  return buses_.Mutable(index);
}
inline void Stop::set_buses(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:ProtoTransport.Stop.buses)
  buses_.Mutable(index)->assign(value);
}
#if LANG_CXX11
inline void Stop::set_buses(int index, ::std::string&& value) {
  // @@protoc_insertion_point(field_set:ProtoTransport.Stop.buses)
  buses_.Mutable(index)->assign(std::move(value));
}
#endif
inline void Stop::set_buses(int index, const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  buses_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:ProtoTransport.Stop.buses)
}
inline void Stop::set_buses(int index, const char* value, size_t size) {
  buses_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ProtoTransport.Stop.buses)
}
inline ::std::string* Stop::add_buses() {
  // @@protoc_insertion_point(field_add_mutable:ProtoTransport.Stop.buses)
  return buses_.Add();
}
inline void Stop::add_buses(const ::std::string& value) {
  buses_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:ProtoTransport.Stop.buses)
}
#if LANG_CXX11
inline void Stop::add_buses(::std::string&& value) {
  buses_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:ProtoTransport.Stop.buses)
}
#endif
inline void Stop::add_buses(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  buses_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:ProtoTransport.Stop.buses)
}
inline void Stop::add_buses(const char* value, size_t size) {
  buses_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:ProtoTransport.Stop.buses)
}
inline const ::google::protobuf::RepeatedPtrField<::std::string>&
Stop::buses() const {
  // @@protoc_insertion_point(field_list:ProtoTransport.Stop.buses)
  return buses_;
}
inline ::google::protobuf::RepeatedPtrField<::std::string>*
Stop::mutable_buses() {
  // @@protoc_insertion_point(field_mutable_list:ProtoTransport.Stop.buses)
  return &buses_;
}

// -------------------------------------------------------------------

// Bus

// uint32 stops = 1;
inline void Bus::clear_stops() {
  stops_ = 0u;
}
inline ::google::protobuf::uint32 Bus::stops() const {
  // @@protoc_insertion_point(field_get:ProtoTransport.Bus.stops)
  return stops_;
}
inline void Bus::set_stops(::google::protobuf::uint32 value) {
  
  stops_ = value;
  // @@protoc_insertion_point(field_set:ProtoTransport.Bus.stops)
}

// uint32 unique_stops = 2;
inline void Bus::clear_unique_stops() {
  unique_stops_ = 0u;
}
inline ::google::protobuf::uint32 Bus::unique_stops() const {
  // @@protoc_insertion_point(field_get:ProtoTransport.Bus.unique_stops)
  return unique_stops_;
}
inline void Bus::set_unique_stops(::google::protobuf::uint32 value) {
  
  unique_stops_ = value;
  // @@protoc_insertion_point(field_set:ProtoTransport.Bus.unique_stops)
}

// uint32 road_route = 3;
inline void Bus::clear_road_route() {
  road_route_ = 0u;
}
inline ::google::protobuf::uint32 Bus::road_route() const {
  // @@protoc_insertion_point(field_get:ProtoTransport.Bus.road_route)
  return road_route_;
}
inline void Bus::set_road_route(::google::protobuf::uint32 value) {
  
  road_route_ = value;
  // @@protoc_insertion_point(field_set:ProtoTransport.Bus.road_route)
}

// double linear_route = 4;
inline void Bus::clear_linear_route() {
  linear_route_ = 0;
}
inline double Bus::linear_route() const {
  // @@protoc_insertion_point(field_get:ProtoTransport.Bus.linear_route)
  return linear_route_;
}
inline void Bus::set_linear_route(double value) {
  
  linear_route_ = value;
  // @@protoc_insertion_point(field_set:ProtoTransport.Bus.linear_route)
}

// -------------------------------------------------------------------

// Database

// repeated string stop_names = 1;
inline int Database::stop_names_size() const {
  return stop_names_.size();
}
inline void Database::clear_stop_names() {
  stop_names_.Clear();
}
inline const ::std::string& Database::stop_names(int index) const {
  // @@protoc_insertion_point(field_get:ProtoTransport.Database.stop_names)
  return stop_names_.Get(index);
}
inline ::std::string* Database::mutable_stop_names(int index) {
  // @@protoc_insertion_point(field_mutable:ProtoTransport.Database.stop_names)
  return stop_names_.Mutable(index);
}
inline void Database::set_stop_names(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:ProtoTransport.Database.stop_names)
  stop_names_.Mutable(index)->assign(value);
}
#if LANG_CXX11
inline void Database::set_stop_names(int index, ::std::string&& value) {
  // @@protoc_insertion_point(field_set:ProtoTransport.Database.stop_names)
  stop_names_.Mutable(index)->assign(std::move(value));
}
#endif
inline void Database::set_stop_names(int index, const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  stop_names_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:ProtoTransport.Database.stop_names)
}
inline void Database::set_stop_names(int index, const char* value, size_t size) {
  stop_names_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ProtoTransport.Database.stop_names)
}
inline ::std::string* Database::add_stop_names() {
  // @@protoc_insertion_point(field_add_mutable:ProtoTransport.Database.stop_names)
  return stop_names_.Add();
}
inline void Database::add_stop_names(const ::std::string& value) {
  stop_names_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:ProtoTransport.Database.stop_names)
}
#if LANG_CXX11
inline void Database::add_stop_names(::std::string&& value) {
  stop_names_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:ProtoTransport.Database.stop_names)
}
#endif
inline void Database::add_stop_names(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  stop_names_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:ProtoTransport.Database.stop_names)
}
inline void Database::add_stop_names(const char* value, size_t size) {
  stop_names_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:ProtoTransport.Database.stop_names)
}
inline const ::google::protobuf::RepeatedPtrField<::std::string>&
Database::stop_names() const {
  // @@protoc_insertion_point(field_list:ProtoTransport.Database.stop_names)
  return stop_names_;
}
inline ::google::protobuf::RepeatedPtrField<::std::string>*
Database::mutable_stop_names() {
  // @@protoc_insertion_point(field_mutable_list:ProtoTransport.Database.stop_names)
  return &stop_names_;
}

// repeated .ProtoTransport.Stop stops = 2;
inline int Database::stops_size() const {
  return stops_.size();
}
inline void Database::clear_stops() {
  stops_.Clear();
}
inline ::ProtoTransport::Stop* Database::mutable_stops(int index) {
  // @@protoc_insertion_point(field_mutable:ProtoTransport.Database.stops)
  return stops_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Stop >*
Database::mutable_stops() {
  // @@protoc_insertion_point(field_mutable_list:ProtoTransport.Database.stops)
  return &stops_;
}
inline const ::ProtoTransport::Stop& Database::stops(int index) const {
  // @@protoc_insertion_point(field_get:ProtoTransport.Database.stops)
  return stops_.Get(index);
}
inline ::ProtoTransport::Stop* Database::add_stops() {
  // @@protoc_insertion_point(field_add:ProtoTransport.Database.stops)
  return stops_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Stop >&
Database::stops() const {
  // @@protoc_insertion_point(field_list:ProtoTransport.Database.stops)
  return stops_;
}

// repeated string bus_names = 3;
inline int Database::bus_names_size() const {
  return bus_names_.size();
}
inline void Database::clear_bus_names() {
  bus_names_.Clear();
}
inline const ::std::string& Database::bus_names(int index) const {
  // @@protoc_insertion_point(field_get:ProtoTransport.Database.bus_names)
  return bus_names_.Get(index);
}
inline ::std::string* Database::mutable_bus_names(int index) {
  // @@protoc_insertion_point(field_mutable:ProtoTransport.Database.bus_names)
  return bus_names_.Mutable(index);
}
inline void Database::set_bus_names(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:ProtoTransport.Database.bus_names)
  bus_names_.Mutable(index)->assign(value);
}
#if LANG_CXX11
inline void Database::set_bus_names(int index, ::std::string&& value) {
  // @@protoc_insertion_point(field_set:ProtoTransport.Database.bus_names)
  bus_names_.Mutable(index)->assign(std::move(value));
}
#endif
inline void Database::set_bus_names(int index, const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  bus_names_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:ProtoTransport.Database.bus_names)
}
inline void Database::set_bus_names(int index, const char* value, size_t size) {
  bus_names_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:ProtoTransport.Database.bus_names)
}
inline ::std::string* Database::add_bus_names() {
  // @@protoc_insertion_point(field_add_mutable:ProtoTransport.Database.bus_names)
  return bus_names_.Add();
}
inline void Database::add_bus_names(const ::std::string& value) {
  bus_names_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:ProtoTransport.Database.bus_names)
}
#if LANG_CXX11
inline void Database::add_bus_names(::std::string&& value) {
  bus_names_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:ProtoTransport.Database.bus_names)
}
#endif
inline void Database::add_bus_names(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  bus_names_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:ProtoTransport.Database.bus_names)
}
inline void Database::add_bus_names(const char* value, size_t size) {
  bus_names_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:ProtoTransport.Database.bus_names)
}
inline const ::google::protobuf::RepeatedPtrField<::std::string>&
Database::bus_names() const {
  // @@protoc_insertion_point(field_list:ProtoTransport.Database.bus_names)
  return bus_names_;
}
inline ::google::protobuf::RepeatedPtrField<::std::string>*
Database::mutable_bus_names() {
  // @@protoc_insertion_point(field_mutable_list:ProtoTransport.Database.bus_names)
  return &bus_names_;
}

// repeated .ProtoTransport.Bus buses = 4;
inline int Database::buses_size() const {
  return buses_.size();
}
inline void Database::clear_buses() {
  buses_.Clear();
}
inline ::ProtoTransport::Bus* Database::mutable_buses(int index) {
  // @@protoc_insertion_point(field_mutable:ProtoTransport.Database.buses)
  return buses_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Bus >*
Database::mutable_buses() {
  // @@protoc_insertion_point(field_mutable_list:ProtoTransport.Database.buses)
  return &buses_;
}
inline const ::ProtoTransport::Bus& Database::buses(int index) const {
  // @@protoc_insertion_point(field_get:ProtoTransport.Database.buses)
  return buses_.Get(index);
}
inline ::ProtoTransport::Bus* Database::add_buses() {
  // @@protoc_insertion_point(field_add:ProtoTransport.Database.buses)
  return buses_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ProtoTransport::Bus >&
Database::buses() const {
  // @@protoc_insertion_point(field_list:ProtoTransport.Database.buses)
  return buses_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace ProtoTransport

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // PROTOBUF_INCLUDED_database_2eproto
