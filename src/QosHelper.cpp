#include "rust-opendds/include/QosHelper.h"

#include <cstring>

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
}
