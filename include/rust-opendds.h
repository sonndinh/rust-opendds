#include "rust/cxx.h"

#include <dds/DCPS/Service_Participant.h>

#include <memory>

namespace OpenDDS {

//typedef DDS::DomainParticipant_var DomainParticipantVar;

typedef unsigned int DomainParticipantHandle;

void initialize(int argc, rust::Vec<rust::String> argv);
void load(rust::String lib_path);
//const DDS::DomainParticipant_var& create_participant(int domain_id);

// Return a handle to the actual DomainParticipant object.
// This type is opaque to the caller. Caller is expected to pass the returned
// value as-is in the subsequent calls in the domain participant.
std::unique_ptr<DomainParticipantHandle> create_participant(int domain_id);

//void delete_participant(const DDS::DomainParticipant_var& dp);
void delete_participant(std::unique_ptr<DomainParticipantHandle> dp_handle);
void subscribe(const std::unique_ptr<DomainParticipantHandle>& dp_handle,
               rust::String topic_name, rust::String topic_type);
void unsubscribe();
void create_datawriter();
void write();
}
