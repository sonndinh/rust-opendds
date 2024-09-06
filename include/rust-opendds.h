#ifndef OPENDDS_RUST_OPENDDS_H
#define OPENDDS_RUST_OPENDDS_H

#include "rust/cxx.h"

#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/TypeSupportImpl.h>

#include <memory>

namespace Rust_OpenDDS {

typedef DDS::DomainParticipant_var DomainParticipantVar;

void initialize(int argc, rust::Vec<rust::String> argv);
void load(rust::String lib_path);
std::unique_ptr<DDS::DomainParticipant_var> create_participant(int domain_id);
void delete_participant(std::unique_ptr<DDS::DomainParticipant_var> dp_ptr);
void subscribe(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr,
               rust::String topic_name, rust::String type_name);

struct DataWriterInfo {
  DataWriterInfo() : dw_ptr(0), ts_ptr(0) {}

  DDS::DataWriter* dw_ptr;
  OpenDDS::DCPS::TypeSupport* ts_ptr;
};

std::unique_ptr<DataWriterInfo> create_datawriter(const std::unique_ptr<DDS::DomainParticipant_var>& dp_ptr,
                                                    rust::String topic_name, rust::String type_name);
void wait_for_readers(const std::unique_ptr<DataWriterInfo>& dwi_ptr);
void write(const std::unique_ptr<DataWriterInfo>& dwi_ptr, rust::String sample,
           DDS::InstanceHandle_t instance = DDS::HANDLE_NIL);
}

#endif
