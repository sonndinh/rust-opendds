#include "rust-opendds/include/QosHelper.h"

#include <cstring>

// Helpers for converting between cxx-generated QoS data structures from the cxx bridge module
// and the original DDS QoS data structures
namespace {

// Helper functions to copy a sequence between the DDS and Rust_OpenDDS namespaces

// Used for a scalar type T
template <typename T>
void to_dds_value(TAO::unbounded_value_sequence<T>& dds_value, const rust::Vec<T>& cxx_value)
{
  const size_t len = cxx_value.size();
  dds_value.length(len);
  std::memcpy(dds_value.get_buffer(), cxx_value.data(), len);
}

template <typename T>
void to_cxx_value(rust::Vec<T>& cxx_value, const TAO::unbounded_value_sequence<T>& dds_value)
{
  const CORBA::ULong len = dds_value.length();
  cxx_value.reserve(len);
  for (CORBA::ULong i = 0; i < len; ++i) {
    cxx_value.push_back(dds_value[i]);
  }
}

// Helper functions for converting the nested types.

void to_dds_qos(DDS::TopicDataQosPolicy& dds_qos, const Rust_OpenDDS::TopicDataQosPolicy& cxx_qos)
{
  to_dds_value(dds_qos.value, cxx_qos.value);
}

void to_cxx_qos(Rust_OpenDDS::TopicDataQosPolicy& cxx_qos, const DDS::TopicDataQosPolicy& dds_qos)
{
  to_cxx_value(cxx_qos.value, dds_qos.value);
}

void to_dds_qos(DDS::DurabilityQosPolicyKind& dds_qos, const Rust_OpenDDS::DurabilityQosPolicyKind& cxx_qos)
{
  switch (cxx_qos) {
  case Rust_OpenDDS::DurabilityQosPolicyKind::VOLATILE_DURABILITY_QOS:
    dds_qos = DDS::DurabilityQosPolicyKind::VOLATILE_DURABILITY_QOS;
    break;
  case Rust_OpenDDS::DurabilityQosPolicyKind::TRANSIENT_LOCAL_DURABILITY_QOS:
    dds_qos = DDS::DurabilityQosPolicyKind::TRANSIENT_LOCAL_DURABILITY_QOS;
    break;
  case Rust_OpenDDS::DurabilityQosPolicyKind::TRANSIENT_DURABILITY_QOS:
    dds_qos = DDS::DurabilityQosPolicyKind::TRANSIENT_DURABILITY_QOS;
    break;
  default:
    dds_qos = DDS::DurabilityQosPolicyKind::PERSISTENT_DURABILITY_QOS;
    break;
  }
}

void to_cxx_qos(Rust_OpenDDS::DurabilityQosPolicyKind& cxx_qos, const DDS::DurabilityQosPolicyKind& dds_qos)
{
  switch (dds_qos) {
  case DDS::DurabilityQosPolicyKind::VOLATILE_DURABILITY_QOS:
    cxx_qos = Rust_OpenDDS::DurabilityQosPolicyKind::VOLATILE_DURABILITY_QOS;
    break;
  case DDS::DurabilityQosPolicyKind::TRANSIENT_LOCAL_DURABILITY_QOS:
    cxx_qos = Rust_OpenDDS::DurabilityQosPolicyKind::TRANSIENT_LOCAL_DURABILITY_QOS;
    break;
  case DDS::DurabilityQosPolicyKind::TRANSIENT_DURABILITY_QOS:
    cxx_qos = Rust_OpenDDS::DurabilityQosPolicyKind::TRANSIENT_DURABILITY_QOS;
    break;
  default:
    cxx_qos = Rust_OpenDDS::DurabilityQosPolicyKind::PERSISTENT_DURABILITY_QOS;
    break;
  }
}

void to_dds_qos(DDS::DurabilityQosPolicy& dds_qos, const Rust_OpenDDS::DurabilityQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.kind, cxx_qos.kind);
}

void to_cxx_qos(Rust_OpenDDS::DurabilityQosPolicy& cxx_qos, const DDS::DurabilityQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.kind, dds_qos.kind);
}

void to_dds_qos(DDS::Duration_t& dds_qos, const Rust_OpenDDS::Duration_t& cxx_qos)
{
  dds_qos.sec = cxx_qos.sec;
  dds_qos.nanosec = cxx_qos.nanosec;
}

void to_cxx_qos(Rust_OpenDDS::Duration_t& cxx_qos, const DDS::Duration_t& dds_qos)
{
  cxx_qos.sec = dds_qos.sec;
  cxx_qos.nanosec = dds_qos.nanosec;
}

void to_dds_qos(DDS::HistoryQosPolicyKind& dds_qos, const Rust_OpenDDS::HistoryQosPolicyKind& cxx_qos)
{
  switch (cxx_qos) {
  case Rust_OpenDDS::HistoryQosPolicyKind::KEEP_LAST_HISTORY_QOS:
    dds_qos = DDS::HistoryQosPolicyKind::KEEP_LAST_HISTORY_QOS;
    break;
  default:
    dds_qos = DDS::HistoryQosPolicyKind::KEEP_ALL_HISTORY_QOS;
    break;
  }
}

void to_cxx_qos(Rust_OpenDDS::HistoryQosPolicyKind& cxx_qos, const DDS::HistoryQosPolicyKind& dds_qos)
{
  switch (dds_qos) {
  case DDS::HistoryQosPolicyKind::KEEP_LAST_HISTORY_QOS:
    cxx_qos = Rust_OpenDDS::HistoryQosPolicyKind::KEEP_LAST_HISTORY_QOS;
    break;
  default:
    cxx_qos = Rust_OpenDDS::HistoryQosPolicyKind::KEEP_ALL_HISTORY_QOS;
    break;
  }
}

void to_dds_qos(DDS::DurabilityServiceQosPolicy& dds_qos, const Rust_OpenDDS::DurabilityServiceQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.service_cleanup_delay, cxx_qos.service_cleanup_delay);
  to_dds_qos(dds_qos.history_kind, cxx_qos.history_kind);
  dds_qos.history_depth = cxx_qos.history_depth;
  dds_qos.max_samples = cxx_qos.max_samples;
  dds_qos.max_instances = cxx_qos.max_instances;
  dds_qos.max_samples_per_instance = cxx_qos.max_samples_per_instance;
}

void to_cxx_qos(Rust_OpenDDS::DurabilityServiceQosPolicy& cxx_qos, const DDS::DurabilityServiceQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.service_cleanup_delay, dds_qos.service_cleanup_delay);
  to_cxx_qos(cxx_qos.history_kind, dds_qos.history_kind);
  cxx_qos.history_depth = dds_qos.history_depth;
  cxx_qos.max_samples = dds_qos.max_samples;
  cxx_qos.max_instances = dds_qos.max_instances;
  cxx_qos.max_samples_per_instance = dds_qos.max_samples_per_instance;
}

void to_dds_qos(DDS::DeadlineQosPolicy& dds_qos, const Rust_OpenDDS::DeadlineQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.period, cxx_qos.period);
}

void to_cxx_qos(Rust_OpenDDS::DeadlineQosPolicy& cxx_qos, const DDS::DeadlineQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.period, dds_qos.period);
}

void to_dds_qos(DDS::LatencyBudgetQosPolicy& dds_qos, const Rust_OpenDDS::LatencyBudgetQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.duration, cxx_qos.duration);
}

void to_cxx_qos(Rust_OpenDDS::LatencyBudgetQosPolicy& cxx_qos, const DDS::LatencyBudgetQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.duration, dds_qos.duration);
}

void to_dds_qos(DDS::LivelinessQosPolicyKind& dds_qos, const Rust_OpenDDS::LivelinessQosPolicyKind& cxx_qos)
{
  switch (cxx_qos) {
  case Rust_OpenDDS::LivelinessQosPolicyKind::AUTOMATIC_LIVELINESS_QOS:
    dds_qos = DDS::LivelinessQosPolicyKind::AUTOMATIC_LIVELINESS_QOS;
    break;
  case Rust_OpenDDS::LivelinessQosPolicyKind::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS:
    dds_qos = DDS::LivelinessQosPolicyKind::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS;
    break;
  default:
    dds_qos = DDS::LivelinessQosPolicyKind::MANUAL_BY_TOPIC_LIVELINESS_QOS;
    break;
  }
}

void to_cxx_qos(Rust_OpenDDS::LivelinessQosPolicyKind& cxx_qos, const DDS::LivelinessQosPolicyKind& dds_qos)
{
  switch (dds_qos) {
  case DDS::LivelinessQosPolicyKind::AUTOMATIC_LIVELINESS_QOS:
    cxx_qos = Rust_OpenDDS::LivelinessQosPolicyKind::AUTOMATIC_LIVELINESS_QOS;
    break;
  case DDS::LivelinessQosPolicyKind::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS:
    cxx_qos = Rust_OpenDDS::LivelinessQosPolicyKind::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS;
    break;
  default:
    cxx_qos = Rust_OpenDDS::LivelinessQosPolicyKind::MANUAL_BY_TOPIC_LIVELINESS_QOS;
    break;
  }
}

void to_dds_qos(DDS::LivelinessQosPolicy& dds_qos, const Rust_OpenDDS::LivelinessQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.kind, cxx_qos.kind);
  to_dds_qos(dds_qos.lease_duration, cxx_qos.lease_duration);
}

void to_cxx_qos(Rust_OpenDDS::LivelinessQosPolicy& cxx_qos, const DDS::LivelinessQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.kind, dds_qos.kind);
  to_cxx_qos(cxx_qos.lease_duration, dds_qos.lease_duration);
}

void to_dds_qos(DDS::ReliabilityQosPolicyKind& dds_qos, const Rust_OpenDDS::ReliabilityQosPolicyKind& cxx_qos)
{
  switch (cxx_qos) {
  case Rust_OpenDDS::ReliabilityQosPolicyKind::BEST_EFFORT_RELIABILITY_QOS:
    dds_qos = DDS::ReliabilityQosPolicyKind::BEST_EFFORT_RELIABILITY_QOS;
    break;
  default:
    dds_qos = DDS::ReliabilityQosPolicyKind::RELIABLE_RELIABILITY_QOS;
    break;
  }
}

void to_cxx_qos(Rust_OpenDDS::ReliabilityQosPolicyKind& cxx_qos, const DDS::ReliabilityQosPolicyKind& dds_qos)
{
  switch (dds_qos) {
  case DDS::ReliabilityQosPolicyKind::BEST_EFFORT_RELIABILITY_QOS:
    cxx_qos = Rust_OpenDDS::ReliabilityQosPolicyKind::BEST_EFFORT_RELIABILITY_QOS;
    break;
  default:
    cxx_qos = Rust_OpenDDS::ReliabilityQosPolicyKind::RELIABLE_RELIABILITY_QOS;
    break;
  }
}

void to_dds_qos(DDS::ReliabilityQosPolicy& dds_qos, const Rust_OpenDDS::ReliabilityQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.kind, cxx_qos.kind);
  to_dds_qos(dds_qos.max_blocking_time, cxx_qos.max_blocking_time);
}

void to_cxx_qos(Rust_OpenDDS::ReliabilityQosPolicy& cxx_qos, const DDS::ReliabilityQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.kind, dds_qos.kind);
  to_cxx_qos(cxx_qos.max_blocking_time, dds_qos.max_blocking_time);
}

void to_dds_qos(DDS::DestinationOrderQosPolicyKind& dds_qos, const Rust_OpenDDS::DestinationOrderQosPolicyKind& cxx_qos)
{
  switch (cxx_qos) {
  case Rust_OpenDDS::DestinationOrderQosPolicyKind::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS:
    dds_qos = DDS::DestinationOrderQosPolicyKind::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
    break;
  default:
    dds_qos = DDS::DestinationOrderQosPolicyKind::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
    break;
  }
}

void to_cxx_qos(Rust_OpenDDS::DestinationOrderQosPolicyKind& cxx_qos, const DDS::DestinationOrderQosPolicyKind& dds_qos)
{
  switch (dds_qos) {
  case DDS::DestinationOrderQosPolicyKind::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS:
    cxx_qos = Rust_OpenDDS::DestinationOrderQosPolicyKind::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;
    break;
  default:
    cxx_qos = Rust_OpenDDS::DestinationOrderQosPolicyKind::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS;
    break;
  }
}

void to_dds_qos(DDS::DestinationOrderQosPolicy& dds_qos, const Rust_OpenDDS::DestinationOrderQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.kind, cxx_qos.kind);
}

void to_cxx_qos(Rust_OpenDDS::DestinationOrderQosPolicy& cxx_qos, const DDS::DestinationOrderQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.kind, dds_qos.kind);
}

void to_dds_qos(DDS::HistoryQosPolicy& dds_qos, const Rust_OpenDDS::HistoryQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.kind, cxx_qos.kind);
  dds_qos.depth = cxx_qos.depth;
}

void to_cxx_qos(Rust_OpenDDS::HistoryQosPolicy& cxx_qos, const DDS::HistoryQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.kind, dds_qos.kind);
  cxx_qos.depth = dds_qos.depth;
}

void to_dds_qos(DDS::ResourceLimitsQosPolicy& dds_qos, const Rust_OpenDDS::ResourceLimitsQosPolicy& cxx_qos)
{
  dds_qos.max_samples = cxx_qos.max_samples;
  dds_qos.max_instances = cxx_qos.max_instances;
  dds_qos.max_samples_per_instance = cxx_qos.max_samples_per_instance;
}

void to_cxx_qos(Rust_OpenDDS::ResourceLimitsQosPolicy& cxx_qos, const DDS::ResourceLimitsQosPolicy& dds_qos)
{
  cxx_qos.max_samples = dds_qos.max_samples;
  cxx_qos.max_instances = dds_qos.max_instances;
  cxx_qos.max_samples_per_instance = dds_qos.max_samples_per_instance;
}

void to_dds_qos(DDS::TransportPriorityQosPolicy& dds_qos, const Rust_OpenDDS::TransportPriorityQosPolicy& cxx_qos)
{
  dds_qos.value = cxx_qos.value;
}

void to_cxx_qos(Rust_OpenDDS::TransportPriorityQosPolicy& cxx_qos, const DDS::TransportPriorityQosPolicy& dds_qos)
{
  cxx_qos.value = dds_qos.value;
}

void to_dds_qos(DDS::LifespanQosPolicy& dds_qos, const Rust_OpenDDS::LifespanQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.duration, cxx_qos.duration);
}

void to_cxx_qos(Rust_OpenDDS::LifespanQosPolicy& cxx_qos, const DDS::LifespanQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.duration, dds_qos.duration);
}

void to_dds_qos(DDS::OwnershipQosPolicyKind& dds_qos, const Rust_OpenDDS::OwnershipQosPolicyKind& cxx_qos)
{
  switch (cxx_qos) {
  case Rust_OpenDDS::OwnershipQosPolicyKind::SHARED_OWNERSHIP_QOS:
    dds_qos = DDS::OwnershipQosPolicyKind::SHARED_OWNERSHIP_QOS;
    break;
  default:
    dds_qos = DDS::OwnershipQosPolicyKind::EXCLUSIVE_OWNERSHIP_QOS;
    break;
  }
}

void to_cxx_qos(Rust_OpenDDS::OwnershipQosPolicyKind& cxx_qos, const DDS::OwnershipQosPolicyKind& dds_qos)
{
  switch (dds_qos) {
  case DDS::OwnershipQosPolicyKind::SHARED_OWNERSHIP_QOS:
    cxx_qos = Rust_OpenDDS::OwnershipQosPolicyKind::SHARED_OWNERSHIP_QOS;
    break;
  default:
    cxx_qos = Rust_OpenDDS::OwnershipQosPolicyKind::EXCLUSIVE_OWNERSHIP_QOS;
    break;
  }
}

void to_dds_qos(DDS::OwnershipQosPolicy& dds_qos, const Rust_OpenDDS::OwnershipQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.kind, cxx_qos.kind);
}

void to_cxx_qos(Rust_OpenDDS::OwnershipQosPolicy& cxx_qos, const DDS::OwnershipQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.kind, dds_qos.kind);
}

void to_dds_qos(DDS::DataRepresentationQosPolicy& dds_qos, const Rust_OpenDDS::DataRepresentationQosPolicy& cxx_qos)
{
  to_dds_value(dds_qos.value, cxx_qos.value);
}

void to_cxx_qos(Rust_OpenDDS::DataRepresentationQosPolicy& cxx_qos, const DDS::DataRepresentationQosPolicy& dds_qos)
{
  to_cxx_value(cxx_qos.value, dds_qos.value);
}

void to_dds_qos(DDS::PresentationQosPolicyAccessScopeKind& dds_qos, const Rust_OpenDDS::PresentationQosPolicyAccessScopeKind& cxx_qos)
{
  switch (cxx_qos) {
  case Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::INSTANCE_PRESENTATION_QOS:
    dds_qos = DDS::PresentationQosPolicyAccessScopeKind::INSTANCE_PRESENTATION_QOS;
    break;
  case Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::TOPIC_PRESENTATION_QOS:
    dds_qos = DDS::PresentationQosPolicyAccessScopeKind::TOPIC_PRESENTATION_QOS;
    break;
  default:
    dds_qos = DDS::PresentationQosPolicyAccessScopeKind::GROUP_PRESENTATION_QOS;
    break;
  }
}

void to_cxx_qos(Rust_OpenDDS::PresentationQosPolicyAccessScopeKind& cxx_qos, const DDS::PresentationQosPolicyAccessScopeKind& dds_qos)
{
  switch (dds_qos) {
  case DDS::PresentationQosPolicyAccessScopeKind::INSTANCE_PRESENTATION_QOS:
    cxx_qos = Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::INSTANCE_PRESENTATION_QOS;
    break;
  case DDS::PresentationQosPolicyAccessScopeKind::TOPIC_PRESENTATION_QOS:
    cxx_qos = Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::TOPIC_PRESENTATION_QOS;
    break;
  default:
    cxx_qos = Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::GROUP_PRESENTATION_QOS;
    break;
  }
}

void to_dds_qos(DDS::UserDataQosPolicy& dds_qos, const Rust_OpenDDS::UserDataQosPolicy& cxx_qos)
{
  to_dds_value(dds_qos.value, cxx_qos.value);
}

void to_cxx_qos(Rust_OpenDDS::UserDataQosPolicy& cxx_qos, const DDS::UserDataQosPolicy& dds_qos)
{
  to_cxx_value(cxx_qos.value, dds_qos.value);
}

void to_dds_qos(DDS::EntityFactoryQosPolicy& dds_qos, const Rust_OpenDDS::EntityFactoryQosPolicy& cxx_qos)
{
  dds_qos.autoenable_created_entities = cxx_qos.autoenable_created_entities;
}

void to_cxx_qos(Rust_OpenDDS::EntityFactoryQosPolicy& cxx_qos, const DDS::EntityFactoryQosPolicy& dds_qos)
{
  cxx_qos.autoenable_created_entities = dds_qos.autoenable_created_entities;
}

void to_dds_qos(TAO::String_Manager& dds_value, const rust::String& cxx_value)
{
  dds_value = const_cast<rust::String&>(cxx_value).c_str();
}

void to_cxx_qos(rust::String& cxx_value, const TAO::String_Manager& dds_value)
{
  cxx_value = rust::String(dds_value);
}

void to_dds_qos(DDS::Property_t& dds_value, const Rust_OpenDDS::Property_t& cxx_value)
{
  to_dds_qos(dds_value.name, cxx_value.name);
  to_dds_qos(dds_value.value, cxx_value.value);
  dds_value.propagate = cxx_value.propagate;
}

void to_cxx_qos(Rust_OpenDDS::Property_t& cxx_value, const DDS::Property_t& dds_value)
{
  to_cxx_qos(cxx_value.name, dds_value.name);
  to_cxx_qos(cxx_value.value, dds_value.value);
  cxx_value.propagate = dds_value.propagate;
}

void to_dds_qos(DDS::BinaryProperty_t& dds_value, const Rust_OpenDDS::BinaryProperty_t& cxx_value)
{
  to_dds_qos(dds_value.name, cxx_value.name);
  to_dds_value(dds_value.value, cxx_value.value);
  dds_value.propagate = cxx_value.propagate;
}

void to_cxx_qos(Rust_OpenDDS::BinaryProperty_t& cxx_value, const DDS::BinaryProperty_t& dds_value)
{
  to_cxx_qos(cxx_value.name, dds_value.name);
  to_cxx_value(cxx_value.value, dds_value.value);
  cxx_value.propagate = dds_value.propagate;
}

// T1 and T2 are two equivalent types from the DDS and Rust_OpenDDS namespaces.
// For example, T1 is DDS::Property_t, T2 is Rust_OpenDDS::Property_t, etc.
template <typename T1, typename T2>
void to_dds_value(TAO::unbounded_value_sequence<T1>& dds_value, const rust::Vec<T2>& cxx_value)
{
  const size_t len = cxx_value.size();
  dds_value.length(len);
  for (size_t i = 0; i < len; ++i) {
    to_dds_qos(dds_value[i], cxx_value[i]);
  }
}

template <typename T1, typename T2>
void to_cxx_value(rust::Vec<T1>& cxx_value, const TAO::unbounded_value_sequence<T2>& dds_value)
{
  const CORBA::ULong len = dds_value.length();
  cxx_value.reserve(len);
  for (CORBA::ULong i = 0; i < len; ++i) {
    T1 tmp;
    to_cxx_qos(tmp, dds_value[i]);
    cxx_value.push_back(tmp);
  }
}

void to_dds_qos(DDS::PropertyQosPolicy& dds_qos, const Rust_OpenDDS::PropertyQosPolicy& cxx_qos)
{
  to_dds_value(dds_qos.value, cxx_qos.value);
  to_dds_value(dds_qos.binary_value, cxx_qos.binary_value);
}

void to_cxx_qos(Rust_OpenDDS::PropertyQosPolicy& cxx_qos, const DDS::PropertyQosPolicy& dds_qos)
{
  to_cxx_value(cxx_qos.value, dds_qos.value);
  to_cxx_value(cxx_qos.binary_value, dds_qos.binary_value);
}

void to_dds_qos(DDS::PresentationQosPolicy& dds_qos, const Rust_OpenDDS::PresentationQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.access_scope, cxx_qos.access_scope);
  dds_qos.coherent_access = cxx_qos.coherent_access;
  dds_qos.ordered_access = cxx_qos.ordered_access;
}

void to_cxx_qos(Rust_OpenDDS::PresentationQosPolicy& cxx_qos, const DDS::PresentationQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.access_scope, dds_qos.access_scope);
  cxx_qos.coherent_access = dds_qos.coherent_access;
  cxx_qos.ordered_access = dds_qos.ordered_access;
}

void to_dds_qos(DDS::StringSeq& dds_value, const rust::Vec<rust::String>& cxx_value)
{
  const size_t len = cxx_value.size();
  dds_value.length(len);
  for (size_t i = 0; i < len; ++i) {
    dds_value[i] = const_cast<rust::String&>(cxx_value[i]).c_str();
  }
}

void to_cxx_qos(rust::Vec<rust::String>& cxx_value, const DDS::StringSeq& dds_value)
{
  const CORBA::ULong len = dds_value.length();
  cxx_value.reserve(len);
  for (CORBA::ULong i = 0; i < len; ++i) {
    cxx_value.emplace_back(dds_value[i]);
  }
}

void to_dds_qos(DDS::PartitionQosPolicy& dds_qos, const Rust_OpenDDS::PartitionQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.name, cxx_qos.name);
}

void to_cxx_qos(Rust_OpenDDS::PartitionQosPolicy& cxx_qos, const DDS::PartitionQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.name, dds_qos.name);
}

void to_dds_qos(DDS::GroupDataQosPolicy& dds_qos, const Rust_OpenDDS::GroupDataQosPolicy& cxx_qos)
{
  to_dds_value(dds_qos.value, cxx_qos.value);
}

void to_cxx_qos(Rust_OpenDDS::GroupDataQosPolicy& cxx_qos, const DDS::GroupDataQosPolicy& dds_qos)
{
  to_cxx_value(cxx_qos.value, dds_qos.value);
}

void to_dds_qos(DDS::TimeBasedFilterQosPolicy& dds_qos, const Rust_OpenDDS::TimeBasedFilterQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.minimum_separation, cxx_qos.minimum_separation);
}

void to_cxx_qos(Rust_OpenDDS::TimeBasedFilterQosPolicy& cxx_qos, const DDS::TimeBasedFilterQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.minimum_separation, dds_qos.minimum_separation);
}

void to_dds_qos(DDS::ReaderDataLifecycleQosPolicy& dds_qos, const Rust_OpenDDS::ReaderDataLifecycleQosPolicy& cxx_qos)
{
  to_dds_qos(dds_qos.autopurge_nowriter_samples_delay, cxx_qos.autopurge_nowriter_samples_delay);
  to_dds_qos(dds_qos.autopurge_disposed_samples_delay, cxx_qos.autopurge_disposed_samples_delay);
}

void to_cxx_qos(Rust_OpenDDS::ReaderDataLifecycleQosPolicy& cxx_qos, const DDS::ReaderDataLifecycleQosPolicy& dds_qos)
{
  to_cxx_qos(cxx_qos.autopurge_nowriter_samples_delay, dds_qos.autopurge_nowriter_samples_delay);
  to_cxx_qos(cxx_qos.autopurge_disposed_samples_delay, dds_qos.autopurge_disposed_samples_delay);
}

void to_dds_qos(DDS::TypeConsistencyEnforcementQosPolicy& dds_qos, const Rust_OpenDDS::TypeConsistencyEnforcementQosPolicy& cxx_qos)
{
  dds_qos.kind = cxx_qos.kind;
  dds_qos.ignore_sequence_bounds = cxx_qos.ignore_sequence_bounds;
  dds_qos.ignore_string_bounds = cxx_qos.ignore_string_bounds;
  dds_qos.ignore_member_names = cxx_qos.ignore_member_names;
  dds_qos.prevent_type_widening = cxx_qos.prevent_type_widening;
  dds_qos.force_type_validation = cxx_qos.force_type_validation;
}

void to_cxx_qos(Rust_OpenDDS::TypeConsistencyEnforcementQosPolicy& cxx_qos, const DDS::TypeConsistencyEnforcementQosPolicy& dds_qos)
{
  cxx_qos.kind = dds_qos.kind;
  cxx_qos.ignore_sequence_bounds = dds_qos.ignore_sequence_bounds;
  cxx_qos.ignore_string_bounds = dds_qos.ignore_string_bounds;
  cxx_qos.ignore_member_names = dds_qos.ignore_member_names;
  cxx_qos.prevent_type_widening = dds_qos.prevent_type_widening;
  cxx_qos.force_type_validation = dds_qos.force_type_validation;
}

void to_dds_qos(DDS::OwnershipStrengthQosPolicy& dds_qos, const Rust_OpenDDS::OwnershipStrengthQosPolicy& cxx_qos)
{
  dds_qos.value = cxx_qos.value;
}

void to_cxx_qos(Rust_OpenDDS::OwnershipStrengthQosPolicy& cxx_qos, const DDS::OwnershipStrengthQosPolicy& dds_qos)
{
  cxx_qos.value = dds_qos.value;
}

void to_dds_qos(DDS::WriterDataLifecycleQosPolicy& dds_qos, const Rust_OpenDDS::WriterDataLifecycleQosPolicy& cxx_qos)
{
  dds_qos.autodispose_unregistered_instances = cxx_qos.autodispose_unregistered_instances;
}

void to_cxx_qos(Rust_OpenDDS::WriterDataLifecycleQosPolicy& cxx_qos, const DDS::WriterDataLifecycleQosPolicy& dds_qos)
{
  cxx_qos.autodispose_unregistered_instances = dds_qos.autodispose_unregistered_instances;
}

} // namespace anonymous

void to_dds_qos(DDS::DomainParticipantQos& dds_qos, const Rust_OpenDDS::DomainParticipantQos& cxx_qos)
{
  to_dds_qos(dds_qos.user_data, cxx_qos.user_data);
  to_dds_qos(dds_qos.entity_factory, cxx_qos.entity_factory);
  to_dds_qos(dds_qos.property, cxx_qos.property);
}

void to_cxx_qos(Rust_OpenDDS::DomainParticipantQos& cxx_qos, const DDS::DomainParticipantQos& dds_qos)
{
  to_cxx_qos(cxx_qos.user_data, dds_qos.user_data);
  to_cxx_qos(cxx_qos.entity_factory, dds_qos.entity_factory);
  to_cxx_qos(cxx_qos.property, dds_qos.property);
}

void to_dds_qos(DDS::TopicQos& dds_qos, const Rust_OpenDDS::TopicQos& cxx_qos)
{
  to_dds_qos(dds_qos.topic_data, cxx_qos.topic_data);
  to_dds_qos(dds_qos.durability, cxx_qos.durability);
  to_dds_qos(dds_qos.durability_service, cxx_qos.durability_service);
  to_dds_qos(dds_qos.deadline, cxx_qos.deadline);
  to_dds_qos(dds_qos.latency_budget, cxx_qos.latency_budget);
  to_dds_qos(dds_qos.liveliness, cxx_qos.liveliness);
  to_dds_qos(dds_qos.reliability, cxx_qos.reliability);
  to_dds_qos(dds_qos.destination_order, cxx_qos.destination_order);
  to_dds_qos(dds_qos.history, cxx_qos.history);
  to_dds_qos(dds_qos.resource_limits, cxx_qos.resource_limits);
  to_dds_qos(dds_qos.transport_priority, cxx_qos.transport_priority);
  to_dds_qos(dds_qos.lifespan, cxx_qos.lifespan);
  to_dds_qos(dds_qos.ownership, cxx_qos.ownership);
  to_dds_qos(dds_qos.representation, cxx_qos.representation);
}

void to_cxx_qos(Rust_OpenDDS::TopicQos& cxx_qos, const DDS::TopicQos& dds_qos)
{
  to_cxx_qos(cxx_qos.topic_data, dds_qos.topic_data);
  to_cxx_qos(cxx_qos.durability, dds_qos.durability);
  to_cxx_qos(cxx_qos.durability_service, dds_qos.durability_service);
  to_cxx_qos(cxx_qos.deadline, dds_qos.deadline);
  to_cxx_qos(cxx_qos.latency_budget, dds_qos.latency_budget);
  to_cxx_qos(cxx_qos.liveliness, dds_qos.liveliness);
  to_cxx_qos(cxx_qos.reliability, dds_qos.reliability);
  to_cxx_qos(cxx_qos.destination_order, dds_qos.destination_order);
  to_cxx_qos(cxx_qos.history, dds_qos.history);
  to_cxx_qos(cxx_qos.resource_limits, dds_qos.resource_limits);
  to_cxx_qos(cxx_qos.transport_priority, dds_qos.transport_priority);
  to_cxx_qos(cxx_qos.lifespan, dds_qos.lifespan);
  to_cxx_qos(cxx_qos.ownership, dds_qos.ownership);
  to_cxx_qos(cxx_qos.representation, dds_qos.representation);
}

void to_dds_qos(DDS::SubscriberQos& dds_qos, const Rust_OpenDDS::SubscriberQos& cxx_qos)
{
  to_dds_qos(dds_qos.presentation, cxx_qos.presentation);
  to_dds_qos(dds_qos.partition, cxx_qos.partition);
  to_dds_qos(dds_qos.group_data, cxx_qos.group_data);
  to_dds_qos(dds_qos.entity_factory, cxx_qos.entity_factory);
}

void to_cxx_qos(Rust_OpenDDS::SubscriberQos& cxx_qos, const DDS::SubscriberQos& dds_qos)
{
  to_cxx_qos(cxx_qos.presentation, dds_qos.presentation);
  to_cxx_qos(cxx_qos.partition, dds_qos.partition);
  to_cxx_qos(cxx_qos.group_data, dds_qos.group_data);
  to_cxx_qos(cxx_qos.entity_factory, dds_qos.entity_factory);
}

void to_dds_qos(DDS::DataReaderQos& dds_qos, const Rust_OpenDDS::DataReaderQos& cxx_qos)
{
  to_dds_qos(dds_qos.durability, cxx_qos.durability);
  to_dds_qos(dds_qos.deadline, cxx_qos.deadline);
  to_dds_qos(dds_qos.latency_budget, cxx_qos.latency_budget);
  to_dds_qos(dds_qos.liveliness, cxx_qos.liveliness);
  to_dds_qos(dds_qos.reliability, cxx_qos.reliability);
  to_dds_qos(dds_qos.destination_order, cxx_qos.destination_order);
  to_dds_qos(dds_qos.history, cxx_qos.history);
  to_dds_qos(dds_qos.resource_limits, cxx_qos.resource_limits);
  to_dds_qos(dds_qos.user_data, cxx_qos.user_data);
  to_dds_qos(dds_qos.ownership, cxx_qos.ownership);
  to_dds_qos(dds_qos.time_based_filter, cxx_qos.time_based_filter);
  to_dds_qos(dds_qos.reader_data_lifecycle, cxx_qos.reader_data_lifecycle);
  to_dds_qos(dds_qos.representation, cxx_qos.representation);
  to_dds_qos(dds_qos.type_consistency, cxx_qos.type_consistency);
}

void to_cxx_qos(Rust_OpenDDS::DataReaderQos& cxx_qos, const DDS::DataReaderQos& dds_qos)
{
  to_cxx_qos(cxx_qos.durability, dds_qos.durability);
  to_cxx_qos(cxx_qos.deadline, dds_qos.deadline);
  to_cxx_qos(cxx_qos.latency_budget, dds_qos.latency_budget);
  to_cxx_qos(cxx_qos.liveliness, dds_qos.liveliness);
  to_cxx_qos(cxx_qos.reliability, dds_qos.reliability);
  to_cxx_qos(cxx_qos.destination_order, dds_qos.destination_order);
  to_cxx_qos(cxx_qos.history, dds_qos.history);
  to_cxx_qos(cxx_qos.resource_limits, dds_qos.resource_limits);
  to_cxx_qos(cxx_qos.user_data, dds_qos.user_data);
  to_cxx_qos(cxx_qos.ownership, dds_qos.ownership);
  to_cxx_qos(cxx_qos.time_based_filter, dds_qos.time_based_filter);
  to_cxx_qos(cxx_qos.reader_data_lifecycle, dds_qos.reader_data_lifecycle);
  to_cxx_qos(cxx_qos.representation, dds_qos.representation);
  to_cxx_qos(cxx_qos.type_consistency, dds_qos.type_consistency);
}

void to_dds_qos(DDS::PublisherQos& dds_qos, const Rust_OpenDDS::PublisherQos& cxx_qos)
{
  to_dds_qos(dds_qos.presentation, cxx_qos.presentation);
  to_dds_qos(dds_qos.partition, cxx_qos.partition);
  to_dds_qos(dds_qos.group_data, cxx_qos.group_data);
  to_dds_qos(dds_qos.entity_factory, cxx_qos.entity_factory);
}

void to_cxx_qos(Rust_OpenDDS::PublisherQos& cxx_qos, const DDS::PublisherQos& dds_qos)
{
  to_cxx_qos(cxx_qos.presentation, dds_qos.presentation);
  to_cxx_qos(cxx_qos.partition, dds_qos.partition);
  to_cxx_qos(cxx_qos.group_data, dds_qos.group_data);
  to_cxx_qos(cxx_qos.entity_factory, dds_qos.entity_factory);
}

void to_dds_qos(DDS::DataWriterQos& dds_qos, const Rust_OpenDDS::DataWriterQos& cxx_qos)
{
  to_dds_qos(dds_qos.durability, cxx_qos.durability);
  to_dds_qos(dds_qos.durability_service, cxx_qos.durability_service);
  to_dds_qos(dds_qos.deadline, cxx_qos.deadline);
  to_dds_qos(dds_qos.latency_budget, cxx_qos.latency_budget);
  to_dds_qos(dds_qos.liveliness, cxx_qos.liveliness);
  to_dds_qos(dds_qos.reliability, cxx_qos.reliability);
  to_dds_qos(dds_qos.destination_order, cxx_qos.destination_order);
  to_dds_qos(dds_qos.history, cxx_qos.history);
  to_dds_qos(dds_qos.resource_limits, cxx_qos.resource_limits);
  to_dds_qos(dds_qos.transport_priority, cxx_qos.transport_priority);
  to_dds_qos(dds_qos.lifespan, cxx_qos.lifespan);
  to_dds_qos(dds_qos.user_data, cxx_qos.user_data);
  to_dds_qos(dds_qos.ownership, cxx_qos.ownership);
  to_dds_qos(dds_qos.ownership_strength, cxx_qos.ownership_strength);
  to_dds_qos(dds_qos.writer_data_lifecycle, cxx_qos.writer_data_lifecycle);
  to_dds_qos(dds_qos.representation, cxx_qos.representation);
}

void to_cxx_qos(Rust_OpenDDS::DataWriterQos& cxx_qos, const DDS::DataWriterQos& dds_qos)
{
  to_cxx_qos(cxx_qos.durability, dds_qos.durability);
  to_cxx_qos(cxx_qos.durability_service, dds_qos.durability_service);
  to_cxx_qos(cxx_qos.deadline, dds_qos.deadline);
  to_cxx_qos(cxx_qos.latency_budget, dds_qos.latency_budget);
  to_cxx_qos(cxx_qos.liveliness, dds_qos.liveliness);
  to_cxx_qos(cxx_qos.reliability, dds_qos.reliability);
  to_cxx_qos(cxx_qos.destination_order, dds_qos.destination_order);
  to_cxx_qos(cxx_qos.history, dds_qos.history);
  to_cxx_qos(cxx_qos.resource_limits, dds_qos.resource_limits);
  to_cxx_qos(cxx_qos.transport_priority, dds_qos.transport_priority);
  to_cxx_qos(cxx_qos.lifespan, dds_qos.lifespan);
  to_cxx_qos(cxx_qos.user_data, dds_qos.user_data);
  to_cxx_qos(cxx_qos.ownership, dds_qos.ownership);
  to_cxx_qos(cxx_qos.ownership_strength, dds_qos.ownership_strength);
  to_cxx_qos(cxx_qos.writer_data_lifecycle, dds_qos.writer_data_lifecycle);
  to_cxx_qos(cxx_qos.representation, dds_qos.representation);
}
