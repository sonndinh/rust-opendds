#include "rust-opendds/src/lib.rs.h"

#include <dds/DdsDcpsCoreC.h>

void to_dds_qos(DDS::DomainParticipantQos& dds_qos, const Rust_OpenDDS::DomainParticipantQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::DomainParticipantQos& cxx_qos, const DDS::DomainParticipantQos& dds_qos);
void to_dds_qos(DDS::SubscriberQos& dds_qos, const Rust_OpenDDS::SubscriberQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::SubscriberQos& cxx_qos, const DDS::SubscriberQos& dds_qos);
void to_dds_qos(DDS::TopicQos& dds_qos, const Rust_OpenDDS::TopicQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::TopicQos& cxx_qos, const DDS::TopicQos& dds_qos);
