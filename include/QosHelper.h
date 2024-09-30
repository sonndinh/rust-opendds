#include "rust-opendds/src/lib.rs.h"

#include <dds/DdsDcpsCoreC.h>

void to_dds_qos(DDS::DomainParticipantQos& dds_qos, const Rust_OpenDDS::DomainParticipantQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::DomainParticipantQos& cxx_qos, const DDS::DomainParticipantQos& dds_qos);
void to_dds_qos(DDS::TopicQos& dds_qos, const Rust_OpenDDS::TopicQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::TopicQos& cxx_qos, const DDS::TopicQos& dds_qos);
void to_dds_qos(DDS::SubscriberQos& dds_qos, const Rust_OpenDDS::SubscriberQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::SubscriberQos& cxx_qos, const DDS::SubscriberQos& dds_qos);
void to_dds_qos(DDS::DataReaderQos& dds_qos, const Rust_OpenDDS::DataReaderQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::DataReaderQos& cxx_qos, const DDS::DataReaderQos& dds_qos);
void to_dds_qos(DDS::PublisherQos& dds_qos, const Rust_OpenDDS::PublisherQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::PublisherQos& cxx_qos, const DDS::PublisherQos& dds_qos);
void to_dds_qos(DDS::DataWriterQos& dds_qos, const Rust_OpenDDS::DataWriterQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::DataWriterQos& cxx_qos, const DDS::DataWriterQos& dds_qos);
