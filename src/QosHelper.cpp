#include "rust-opendds/include/QosHelper.h"

#include <cstring>

namespace {
  DDS::PresentationQosPolicyAccessScopeKind
    to_dds_PresentationQosPolicyAccessScopeKind(const Rust_OpenDDS::PresentationQosPolicyAccessScopeKind& kind)
  {
    switch (kind) {
    case Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::INSTANCE_PRESENTATION_QOS:
      return DDS::PresentationQosPolicyAccessScopeKind::INSTANCE_PRESENTATION_QOS;
    case Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::TOPIC_PRESENTATION_QOS:
      return DDS::PresentationQosPolicyAccessScopeKind::TOPIC_PRESENTATION_QOS;
    default:
      return DDS::PresentationQosPolicyAccessScopeKind::GROUP_PRESENTATION_QOS;
    }
  }

  Rust_OpenDDS::PresentationQosPolicyAccessScopeKind
    to_cxx_PresentationQosPolicyAccessScopeKind(const DDS::PresentationQosPolicyAccessScopeKind& kind)
  {
    switch (kind) {
    case DDS::PresentationQosPolicyAccessScopeKind::INSTANCE_PRESENTATION_QOS:
      return Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::INSTANCE_PRESENTATION_QOS;
    case DDS::PresentationQosPolicyAccessScopeKind::TOPIC_PRESENTATION_QOS:
      return Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::TOPIC_PRESENTATION_QOS;
    default:
      return Rust_OpenDDS::PresentationQosPolicyAccessScopeKind::GROUP_PRESENTATION_QOS;
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
  dds_qos.presentation.access_scope = to_dds_PresentationQosPolicyAccessScopeKind(cxx_qos.presentation.access_scope);
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
  cxx_qos.presentation.access_scope = to_cxx_PresentationQosPolicyAccessScopeKind(dds_qos.presentation.access_scope);
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
