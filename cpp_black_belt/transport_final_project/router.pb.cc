// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: router.proto

#include "router.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

extern PROTOBUF_INTERNAL_EXPORT_router_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_Edge_router_2eproto;
namespace ProtoTransport {
class EdgeDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Edge> _instance;
} _Edge_default_instance_;
class RouterDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Router> _instance;
} _Router_default_instance_;
}  // namespace ProtoTransport
static void InitDefaultsEdge_router_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::ProtoTransport::_Edge_default_instance_;
    new (ptr) ::ProtoTransport::Edge();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::ProtoTransport::Edge::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_Edge_router_2eproto =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsEdge_router_2eproto}, {}};

static void InitDefaultsRouter_router_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::ProtoTransport::_Router_default_instance_;
    new (ptr) ::ProtoTransport::Router();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::ProtoTransport::Router::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<1> scc_info_Router_router_2eproto =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 1, InitDefaultsRouter_router_2eproto}, {
      &scc_info_Edge_router_2eproto.base,}};

void InitDefaults_router_2eproto() {
  ::google::protobuf::internal::InitSCC(&scc_info_Edge_router_2eproto.base);
  ::google::protobuf::internal::InitSCC(&scc_info_Router_router_2eproto.base);
}

::google::protobuf::Metadata file_level_metadata_router_2eproto[2];
constexpr ::google::protobuf::EnumDescriptor const** file_level_enum_descriptors_router_2eproto = nullptr;
constexpr ::google::protobuf::ServiceDescriptor const** file_level_service_descriptors_router_2eproto = nullptr;

const ::google::protobuf::uint32 TableStruct_router_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Edge, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Edge, from_),
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Edge, to_),
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Edge, type_),
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Edge, time_),
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Edge, item_name_),
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Edge, span_count_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Router, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Router, vertex_count_),
  PROTOBUF_FIELD_OFFSET(::ProtoTransport::Router, edges_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::ProtoTransport::Edge)},
  { 11, -1, sizeof(::ProtoTransport::Router)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::ProtoTransport::_Edge_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&::ProtoTransport::_Router_default_instance_),
};

::google::protobuf::internal::AssignDescriptorsTable assign_descriptors_table_router_2eproto = {
  {}, AddDescriptors_router_2eproto, "router.proto", schemas,
  file_default_instances, TableStruct_router_2eproto::offsets,
  file_level_metadata_router_2eproto, 2, file_level_enum_descriptors_router_2eproto, file_level_service_descriptors_router_2eproto,
};

const char descriptor_table_protodef_router_2eproto[] =
  "\n\014router.proto\022\016ProtoTransport\"c\n\004Edge\022\014"
  "\n\004from\030\001 \001(\r\022\n\n\002to\030\002 \001(\r\022\014\n\004type\030\003 \001(\010\022\014"
  "\n\004time\030\004 \001(\001\022\021\n\titem_name\030\005 \001(\t\022\022\n\nspan_"
  "count\030\006 \001(\005\"C\n\006Router\022\024\n\014vertex_count\030\001 "
  "\001(\005\022#\n\005edges\030\002 \003(\0132\024.ProtoTransport.Edge"
  "b\006proto3"
  ;
::google::protobuf::internal::DescriptorTable descriptor_table_router_2eproto = {
  false, InitDefaults_router_2eproto, 
  descriptor_table_protodef_router_2eproto,
  "router.proto", &assign_descriptors_table_router_2eproto, 208,
};

void AddDescriptors_router_2eproto() {
  static constexpr ::google::protobuf::internal::InitFunc deps[1] =
  {
  };
 ::google::protobuf::internal::AddDescriptors(&descriptor_table_router_2eproto, deps, 0);
}

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_router_2eproto = []() { AddDescriptors_router_2eproto(); return true; }();
namespace ProtoTransport {

// ===================================================================

void Edge::InitAsDefaultInstance() {
}
class Edge::HasBitSetters {
 public:
};

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Edge::kFromFieldNumber;
const int Edge::kToFieldNumber;
const int Edge::kTypeFieldNumber;
const int Edge::kTimeFieldNumber;
const int Edge::kItemNameFieldNumber;
const int Edge::kSpanCountFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Edge::Edge()
  : ::google::protobuf::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:ProtoTransport.Edge)
}
Edge::Edge(const Edge& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  item_name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.item_name().size() > 0) {
    item_name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.item_name_);
  }
  ::memcpy(&from_, &from.from_,
    static_cast<size_t>(reinterpret_cast<char*>(&span_count_) -
    reinterpret_cast<char*>(&from_)) + sizeof(span_count_));
  // @@protoc_insertion_point(copy_constructor:ProtoTransport.Edge)
}

void Edge::SharedCtor() {
  ::google::protobuf::internal::InitSCC(
      &scc_info_Edge_router_2eproto.base);
  item_name_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&from_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&span_count_) -
      reinterpret_cast<char*>(&from_)) + sizeof(span_count_));
}

Edge::~Edge() {
  // @@protoc_insertion_point(destructor:ProtoTransport.Edge)
  SharedDtor();
}

void Edge::SharedDtor() {
  item_name_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void Edge::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Edge& Edge::default_instance() {
  ::google::protobuf::internal::InitSCC(&::scc_info_Edge_router_2eproto.base);
  return *internal_default_instance();
}


void Edge::Clear() {
// @@protoc_insertion_point(message_clear_start:ProtoTransport.Edge)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  item_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&from_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&span_count_) -
      reinterpret_cast<char*>(&from_)) + sizeof(span_count_));
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* Edge::_InternalParse(const char* begin, const char* end, void* object,
                  ::google::protobuf::internal::ParseContext* ctx) {
  auto msg = static_cast<Edge*>(object);
  ::google::protobuf::int32 size; (void)size;
  int depth; (void)depth;
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::ParseFunc parser_till_end; (void)parser_till_end;
  auto ptr = begin;
  while (ptr < end) {
    ptr = ::google::protobuf::io::Parse32(ptr, &tag);
    GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
    switch (tag >> 3) {
      // uint32 from = 1;
      case 1: {
        if (static_cast<::google::protobuf::uint8>(tag) != 8) goto handle_unusual;
        msg->set_from(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // uint32 to = 2;
      case 2: {
        if (static_cast<::google::protobuf::uint8>(tag) != 16) goto handle_unusual;
        msg->set_to(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // bool type = 3;
      case 3: {
        if (static_cast<::google::protobuf::uint8>(tag) != 24) goto handle_unusual;
        msg->set_type(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // double time = 4;
      case 4: {
        if (static_cast<::google::protobuf::uint8>(tag) != 33) goto handle_unusual;
        msg->set_time(::google::protobuf::io::UnalignedLoad<double>(ptr));
        ptr += sizeof(double);
        break;
      }
      // string item_name = 5;
      case 5: {
        if (static_cast<::google::protobuf::uint8>(tag) != 42) goto handle_unusual;
        ptr = ::google::protobuf::io::ReadSize(ptr, &size);
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        ctx->extra_parse_data().SetFieldName("ProtoTransport.Edge.item_name");
        object = msg->mutable_item_name();
        if (size > end - ptr + ::google::protobuf::internal::ParseContext::kSlopBytes) {
          parser_till_end = ::google::protobuf::internal::GreedyStringParserUTF8;
          goto string_till_end;
        }
        GOOGLE_PROTOBUF_PARSER_ASSERT(::google::protobuf::internal::StringCheckUTF8(ptr, size, ctx));
        ::google::protobuf::internal::InlineGreedyStringParser(object, ptr, size, ctx);
        ptr += size;
        break;
      }
      // int32 span_count = 6;
      case 6: {
        if (static_cast<::google::protobuf::uint8>(tag) != 48) goto handle_unusual;
        msg->set_span_count(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->EndGroup(tag);
          return ptr;
        }
        auto res = UnknownFieldParse(tag, {_InternalParse, msg},
          ptr, end, msg->_internal_metadata_.mutable_unknown_fields(), ctx);
        ptr = res.first;
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr != nullptr);
        if (res.second) return ptr;
      }
    }  // switch
  }  // while
  return ptr;
string_till_end:
  static_cast<::std::string*>(object)->clear();
  static_cast<::std::string*>(object)->reserve(size);
  goto len_delim_till_end;
len_delim_till_end:
  return ctx->StoreAndTailCall(ptr, end, {_InternalParse, msg},
                               {parser_till_end, object}, size);
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool Edge::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:ProtoTransport.Edge)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // uint32 from = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (8 & 0xFF)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &from_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // uint32 to = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (16 & 0xFF)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &to_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool type = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (24 & 0xFF)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &type_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // double time = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (33 & 0xFF)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &time_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string item_name = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (42 & 0xFF)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_item_name()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->item_name().data(), static_cast<int>(this->item_name().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "ProtoTransport.Edge.item_name"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 span_count = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (48 & 0xFF)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &span_count_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:ProtoTransport.Edge)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:ProtoTransport.Edge)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

void Edge::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:ProtoTransport.Edge)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 from = 1;
  if (this->from() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->from(), output);
  }

  // uint32 to = 2;
  if (this->to() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->to(), output);
  }

  // bool type = 3;
  if (this->type() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(3, this->type(), output);
  }

  // double time = 4;
  if (this->time() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(4, this->time(), output);
  }

  // string item_name = 5;
  if (this->item_name().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->item_name().data(), static_cast<int>(this->item_name().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "ProtoTransport.Edge.item_name");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      5, this->item_name(), output);
  }

  // int32 span_count = 6;
  if (this->span_count() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(6, this->span_count(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:ProtoTransport.Edge)
}

::google::protobuf::uint8* Edge::InternalSerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:ProtoTransport.Edge)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 from = 1;
  if (this->from() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->from(), target);
  }

  // uint32 to = 2;
  if (this->to() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->to(), target);
  }

  // bool type = 3;
  if (this->type() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(3, this->type(), target);
  }

  // double time = 4;
  if (this->time() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(4, this->time(), target);
  }

  // string item_name = 5;
  if (this->item_name().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->item_name().data(), static_cast<int>(this->item_name().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "ProtoTransport.Edge.item_name");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->item_name(), target);
  }

  // int32 span_count = 6;
  if (this->span_count() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(6, this->span_count(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ProtoTransport.Edge)
  return target;
}

size_t Edge::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ProtoTransport.Edge)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string item_name = 5;
  if (this->item_name().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->item_name());
  }

  // uint32 from = 1;
  if (this->from() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->from());
  }

  // uint32 to = 2;
  if (this->to() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt32Size(
        this->to());
  }

  // double time = 4;
  if (this->time() != 0) {
    total_size += 1 + 8;
  }

  // bool type = 3;
  if (this->type() != 0) {
    total_size += 1 + 1;
  }

  // int32 span_count = 6;
  if (this->span_count() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->span_count());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Edge::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:ProtoTransport.Edge)
  GOOGLE_DCHECK_NE(&from, this);
  const Edge* source =
      ::google::protobuf::DynamicCastToGenerated<Edge>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:ProtoTransport.Edge)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:ProtoTransport.Edge)
    MergeFrom(*source);
  }
}

void Edge::MergeFrom(const Edge& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ProtoTransport.Edge)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.item_name().size() > 0) {

    item_name_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.item_name_);
  }
  if (from.from() != 0) {
    set_from(from.from());
  }
  if (from.to() != 0) {
    set_to(from.to());
  }
  if (from.time() != 0) {
    set_time(from.time());
  }
  if (from.type() != 0) {
    set_type(from.type());
  }
  if (from.span_count() != 0) {
    set_span_count(from.span_count());
  }
}

void Edge::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:ProtoTransport.Edge)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Edge::CopyFrom(const Edge& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ProtoTransport.Edge)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Edge::IsInitialized() const {
  return true;
}

void Edge::Swap(Edge* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Edge::InternalSwap(Edge* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  item_name_.Swap(&other->item_name_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(from_, other->from_);
  swap(to_, other->to_);
  swap(time_, other->time_);
  swap(type_, other->type_);
  swap(span_count_, other->span_count_);
}

::google::protobuf::Metadata Edge::GetMetadata() const {
  ::google::protobuf::internal::AssignDescriptors(&::assign_descriptors_table_router_2eproto);
  return ::file_level_metadata_router_2eproto[kIndexInFileMessages];
}


// ===================================================================

void Router::InitAsDefaultInstance() {
}
class Router::HasBitSetters {
 public:
};

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Router::kVertexCountFieldNumber;
const int Router::kEdgesFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Router::Router()
  : ::google::protobuf::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:ProtoTransport.Router)
}
Router::Router(const Router& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(nullptr),
      edges_(from.edges_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  vertex_count_ = from.vertex_count_;
  // @@protoc_insertion_point(copy_constructor:ProtoTransport.Router)
}

void Router::SharedCtor() {
  ::google::protobuf::internal::InitSCC(
      &scc_info_Router_router_2eproto.base);
  vertex_count_ = 0;
}

Router::~Router() {
  // @@protoc_insertion_point(destructor:ProtoTransport.Router)
  SharedDtor();
}

void Router::SharedDtor() {
}

void Router::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Router& Router::default_instance() {
  ::google::protobuf::internal::InitSCC(&::scc_info_Router_router_2eproto.base);
  return *internal_default_instance();
}


void Router::Clear() {
// @@protoc_insertion_point(message_clear_start:ProtoTransport.Router)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  edges_.Clear();
  vertex_count_ = 0;
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* Router::_InternalParse(const char* begin, const char* end, void* object,
                  ::google::protobuf::internal::ParseContext* ctx) {
  auto msg = static_cast<Router*>(object);
  ::google::protobuf::int32 size; (void)size;
  int depth; (void)depth;
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::ParseFunc parser_till_end; (void)parser_till_end;
  auto ptr = begin;
  while (ptr < end) {
    ptr = ::google::protobuf::io::Parse32(ptr, &tag);
    GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
    switch (tag >> 3) {
      // int32 vertex_count = 1;
      case 1: {
        if (static_cast<::google::protobuf::uint8>(tag) != 8) goto handle_unusual;
        msg->set_vertex_count(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // repeated .ProtoTransport.Edge edges = 2;
      case 2: {
        if (static_cast<::google::protobuf::uint8>(tag) != 18) goto handle_unusual;
        do {
          ptr = ::google::protobuf::io::ReadSize(ptr, &size);
          GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
          parser_till_end = ::ProtoTransport::Edge::_InternalParse;
          object = msg->add_edges();
          if (size > end - ptr) goto len_delim_till_end;
          ptr += size;
          GOOGLE_PROTOBUF_PARSER_ASSERT(ctx->ParseExactRange(
              {parser_till_end, object}, ptr - size, ptr));
          if (ptr >= end) break;
        } while ((::google::protobuf::io::UnalignedLoad<::google::protobuf::uint64>(ptr) & 255) == 18 && (ptr += 1));
        break;
      }
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->EndGroup(tag);
          return ptr;
        }
        auto res = UnknownFieldParse(tag, {_InternalParse, msg},
          ptr, end, msg->_internal_metadata_.mutable_unknown_fields(), ctx);
        ptr = res.first;
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr != nullptr);
        if (res.second) return ptr;
      }
    }  // switch
  }  // while
  return ptr;
len_delim_till_end:
  return ctx->StoreAndTailCall(ptr, end, {_InternalParse, msg},
                               {parser_till_end, object}, size);
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool Router::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:ProtoTransport.Router)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 vertex_count = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (8 & 0xFF)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &vertex_count_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated .ProtoTransport.Edge edges = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (18 & 0xFF)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
                input, add_edges()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:ProtoTransport.Router)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:ProtoTransport.Router)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

void Router::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:ProtoTransport.Router)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 vertex_count = 1;
  if (this->vertex_count() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->vertex_count(), output);
  }

  // repeated .ProtoTransport.Edge edges = 2;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->edges_size()); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2,
      this->edges(static_cast<int>(i)),
      output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:ProtoTransport.Router)
}

::google::protobuf::uint8* Router::InternalSerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:ProtoTransport.Router)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 vertex_count = 1;
  if (this->vertex_count() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->vertex_count(), target);
  }

  // repeated .ProtoTransport.Edge edges = 2;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->edges_size()); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        2, this->edges(static_cast<int>(i)), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:ProtoTransport.Router)
  return target;
}

size_t Router::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:ProtoTransport.Router)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .ProtoTransport.Edge edges = 2;
  {
    unsigned int count = static_cast<unsigned int>(this->edges_size());
    total_size += 1UL * count;
    for (unsigned int i = 0; i < count; i++) {
      total_size +=
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          this->edges(static_cast<int>(i)));
    }
  }

  // int32 vertex_count = 1;
  if (this->vertex_count() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->vertex_count());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Router::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:ProtoTransport.Router)
  GOOGLE_DCHECK_NE(&from, this);
  const Router* source =
      ::google::protobuf::DynamicCastToGenerated<Router>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:ProtoTransport.Router)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:ProtoTransport.Router)
    MergeFrom(*source);
  }
}

void Router::MergeFrom(const Router& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:ProtoTransport.Router)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  edges_.MergeFrom(from.edges_);
  if (from.vertex_count() != 0) {
    set_vertex_count(from.vertex_count());
  }
}

void Router::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:ProtoTransport.Router)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Router::CopyFrom(const Router& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:ProtoTransport.Router)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Router::IsInitialized() const {
  return true;
}

void Router::Swap(Router* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Router::InternalSwap(Router* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  CastToBase(&edges_)->InternalSwap(CastToBase(&other->edges_));
  swap(vertex_count_, other->vertex_count_);
}

::google::protobuf::Metadata Router::GetMetadata() const {
  ::google::protobuf::internal::AssignDescriptors(&::assign_descriptors_table_router_2eproto);
  return ::file_level_metadata_router_2eproto[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace ProtoTransport
namespace google {
namespace protobuf {
template<> PROTOBUF_NOINLINE ::ProtoTransport::Edge* Arena::CreateMaybeMessage< ::ProtoTransport::Edge >(Arena* arena) {
  return Arena::CreateInternal< ::ProtoTransport::Edge >(arena);
}
template<> PROTOBUF_NOINLINE ::ProtoTransport::Router* Arena::CreateMaybeMessage< ::ProtoTransport::Router >(Arena* arena) {
  return Arena::CreateInternal< ::ProtoTransport::Router >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
