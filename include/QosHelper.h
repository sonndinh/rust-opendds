#include "rust-opendds/src/lib.rs.h"

#include <dds/DdsDcpsCoreC.h>

void to_dds_qos(DDS::DomainParticipantQos& dds_qos, const Rust_OpenDDS::DomainParticipantQos& cxx_qos);
void to_cxx_qos(Rust_OpenDDS::DomainParticipantQos& cxx_qos, const DDS::DomainParticipantQos& dds_qos);
