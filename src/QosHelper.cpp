#include "rust-opendds/include/QosHelper.h"

#include <cstring>

namespace {

void to_dds_qos(DDS::TopicDataQosPolicy& dds_qos, const Rust_OpenDDS::TopicDataQosPolicy& cxx_qos)
{
  const size_t len = cxx_qos.value.size();
  dds_qos.value.length(len);
  std::memcpy(dds_qos.value.get_buffer(), cxx_qos.value.data(), len);
}

void to_cxx_qos(Rust_OpenDDS::TopicDataQosPolicy& cxx_qos, const DDS::TopicDataQosPolicy& dds_qos)
{
  const CORBA::ULong len = dds_qos.value.length();
  cxx_qos.value.reserve(len);
  for (CORBA::ULong i = 0; i < len; ++i) {
    cxx_qos.value.push_back(dds_qos.value[i]);
  }
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

void to_dds_qos(DDS::ReliabilityQosPolicy& dds_qos, const Rust_OpenDDS::ReliabilityQosPolicy& cxx_qos)
{
}

void to_cxx_qos(Rust_OpenDDS::ReliabilityQosPolicy& cxx_qos, const DDS::ReliabilityQosPolicy& dds_qos)
{
}

void to_dds_qos(DDS::DestinationOrderQosPolicy& dds_qos, const Rust_OpenDDS::DestinationOrderQosPolicy& cxx_qos)
{
}

void to_cxx_qos(Rust_OpenDDS::DestinationOrderQosPolicy& cxx_qos, const DDS::DestinationOrderQosPolicy& dds_qos)
{
}

void to_dds_qos(DDS::HistoryQosPolicy& dds_qos, const Rust_OpenDDS::HistoryQosPolicy& cxx_qos)
{
}

void to_cxx_qos(Rust_OpenDDS::HistoryQosPolicy& cxx_qos, const DDS::HistoryQosPolicy& dds_qos)
{
}

void to_dds_qos(DDS::ResourceLimitsQosPolicy& dds_qos, const Rust_OpenDDS::ResourceLimitsQosPolicy& cxx_qos)
{
}

void to_cxx_qos(Rust_OpenDDS::ResourceLimitsQosPolicy& cxx_qos, const DDS::ResourceLimitsQosPolicy& dds_qos)
{
}

void to_dds_qos(DDS::TransportPriorityQosPolicy& dds_qos, const Rust_OpenDDS::TransportPriorityQosPolicy& cxx_qos)
{
}

void to_cxx_qos(Rust_OpenDDS::TransportPriorityQosPolicy& cxx_qos, const DDS::TransportPriorityQosPolicy& dds_qos)
{
}

void to_dds_qos(DDS::LifespanQosPolicy& dds_qos, const Rust_OpenDDS::LifespanQosPolicy& cxx_qos)
{
}

void to_cxx_qos(Rust_OpenDDS::LifespanQosPolicy& cxx_qos, const DDS::LifespanQosPolicy& dds_qos)
{
}

void to_dds_qos(DDS::OwnershipQosPolicy& dds_qos, const Rust_OpenDDS::OwnershipQosPolicy& cxx_qos)
{
}

void to_cxx_qos(Rust_OpenDDS::OwnershipQosPolicy& cxx_qos, const DDS::OwnershipQosPolicy& dds_qos)
{
}

void to_dds_qos(DDS::DataRepresentationQosPolicy& dds_qos, const Rust_OpenDDS::DataRepresentationQosPolicy& cxx_qos)
{
}

void to_cxx_qos(Rust_OpenDDS::DataRepresentationQosPolicy& cxx_qos, const DDS::DataRepresentationQosPolicy& dds_qos)
{
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
}

// Helpers for converting between cxx-generated QoS data structures from the cxx bridge module
// and the original DDS QoS data structures
void to_dds_qos(DDS::DomainParticipantQos& dds_qos, const Rust_OpenDDS::DomainParticipantQos& cxx_qos)
{
  {
    const size_t len = cxx_qos.user_data.value.size();
    dds_qos.user_data.value.length(len);
    std::memcpy(dds_qos.user_data.value.get_buffer(), cxx_qos.user_data.value.data(), len);
  }

  dds_qos.entity_factory.autoenable_created_entities = cxx_qos.entity_factory.autoenable_created_entities;

  {
    const size_t len = cxx_qos.property.value.size();
    dds_qos.property.value.length(len);
    for (size_t i = 0; i < len; ++i) {
      dds_qos.property.value[i].name = cxx_qos.property.value[i].name.data();
      dds_qos.property.value[i].value = cxx_qos.property.value[i].value.data();
      dds_qos.property.value[i].propagate = cxx_qos.property.value[i].propagate;
    }
  }

  {
    const size_t len = cxx_qos.property.binary_value.size();
    dds_qos.property.binary_value.length(len);
    for (size_t i = 0; i < len; ++i) {
      dds_qos.property.binary_value[i].name = cxx_qos.property.binary_value[i].name.data();
      dds_qos.property.binary_value[i].value.length(cxx_qos.property.binary_value[i].value.size());
      std::memcpy(dds_qos.property.binary_value[i].value.get_buffer(),
                  cxx_qos.property.binary_value[i].value.data(),
                  cxx_qos.property.binary_value[i].value.size());
      dds_qos.property.binary_value[i].propagate = cxx_qos.property.binary_value[i].propagate;
    }
  }
}

void to_cxx_qos(Rust_OpenDDS::DomainParticipantQos& cxx_qos, const DDS::DomainParticipantQos& dds_qos)
{
  {
    const CORBA::ULong len = dds_qos.user_data.value.length();
    cxx_qos.user_data.value.reserve(len);
    for (CORBA::ULong i = 0; i < len; ++i) {
      cxx_qos.user_data.value.push_back(dds_qos.user_data.value[i]);
    }
  }

  cxx_qos.entity_factory.autoenable_created_entities = dds_qos.entity_factory.autoenable_created_entities;

  {
    const CORBA::ULong len = dds_qos.property.value.length();
    cxx_qos.property.value.reserve(len);
    for (CORBA::ULong i = 0; i < len; ++i) {
      Rust_OpenDDS::Property_t tmp;
      tmp.name = rust::String(dds_qos.property.value[i].name);
      tmp.value = rust::String(dds_qos.property.value[i].value);
      tmp.propagate = dds_qos.property.value[i].propagate;
      cxx_qos.property.value.push_back(tmp);
    }
  }

  {
    const CORBA::ULong len = dds_qos.property.binary_value.length();
    cxx_qos.property.binary_value.reserve(len);
    for (CORBA::ULong i = 0; i < len; ++i) {
      Rust_OpenDDS::BinaryProperty_t tmp;
      tmp.name = rust::String(dds_qos.property.binary_value[i].name);
      const CORBA::ULong bin_len = dds_qos.property.binary_value[i].value.length();
      tmp.value.reserve(bin_len);
      for (CORBA::ULong j = 0; j < bin_len; ++j) {
        tmp.value.push_back(dds_qos.property.binary_value[i].value[j]);
      }
      tmp.propagate = dds_qos.property.binary_value[i].propagate;

      cxx_qos.property.binary_value.push_back(tmp);
    }
  }
}

void to_dds_qos(DDS::SubscriberQos& dds_qos, const Rust_OpenDDS::SubscriberQos& cxx_qos)
{
  // PresentationQosPolicy
  to_dds_qos(dds_qos.presentation.access_scope, cxx_qos.presentation.access_scope);
  dds_qos.presentation.coherent_access = cxx_qos.presentation.coherent_access;
  dds_qos.presentation.ordered_access = cxx_qos.presentation.ordered_access;

  {
    // PartitionQosPolicy
    const size_t len = cxx_qos.partition.name.size();
    dds_qos.partition.name.length(len);
    for (size_t i = 0; i < len; ++i) {
      dds_qos.partition.name[i] = cxx_qos.partition.name[i].data();
    }
  }

  {
    // GroupDataQosPolicy
    const size_t len = cxx_qos.group_data.value.size();
    dds_qos.group_data.value.length(len);
    std::memcpy(dds_qos.group_data.value.get_buffer(), cxx_qos.group_data.value.data(), len);
  }

  // EntityFactoryQosPolicy
  dds_qos.entity_factory.autoenable_created_entities = cxx_qos.entity_factory.autoenable_created_entities;
}

void to_cxx_qos(Rust_OpenDDS::SubscriberQos& cxx_qos, const DDS::SubscriberQos& dds_qos)
{
  to_cxx_qos(cxx_qos.presentation.access_scope, dds_qos.presentation.access_scope);
  cxx_qos.presentation.coherent_access = dds_qos.presentation.coherent_access;
  cxx_qos.presentation.ordered_access = dds_qos.presentation.ordered_access;

  {
    const CORBA::ULong len = dds_qos.partition.name.length();
    cxx_qos.partition.name.reserve(len);
    for (CORBA::ULong i = 0; i < len; ++i) {
      cxx_qos.partition.name.emplace_back(rust::String(dds_qos.partition.name[i]));
    }
  }

  {
    const CORBA::ULong len = dds_qos.group_data.value.length();
    cxx_qos.group_data.value.reserve(len);
    for (CORBA::ULong i = 0; i < len; ++i) {
      cxx_qos.group_data.value.push_back(dds_qos.group_data.value[i]);
    }
  }

  cxx_qos.entity_factory.autoenable_created_entities = dds_qos.entity_factory.autoenable_created_entities;
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
